cmake_minimum_required(VERSION 3.15)

# Set compiler before project()
set(CMAKE_C_COMPILER "clang")
set(CMAKE_ASM_COMPILER "clang")

project(CH32X035 LANGUAGES C ASM)

if(NOT CMAKE_TOOLCHAIN_FILE)
  message(STATUS "No toolchain file set. You can pass -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-riscv-llvm.cmake")
endif()

# Picolibc paths
set(PICOLIBC_ROOT "/usr/lib/picolibc/riscv64-unknown-elf")
set(PICOLIBC_LIB_DIR "${PICOLIBC_ROOT}/lib/rv32imac/ilp32")
set(PICOLIBC_INCLUDE_DIR "${PICOLIBC_ROOT}/include")

# GCC libgcc for compiler runtime support
set(LIBGCC_DIR "/usr/lib/gcc/riscv64-unknown-elf/13.2.0/rv32imac/ilp32")

set(MCU_FLAGS "--target=riscv32-unknown-elf -march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8 -Os -g -ffunction-sections -fdata-sections -fno-common -Wall")
set(LINKER_FLAGS "--target=riscv32-unknown-elf -march=rv32imac_zicsr -mabi=ilp32 -fuse-ld=lld -nodefaultlibs -Wl,--gc-sections -Wl,-T${CMAKE_SOURCE_DIR}/ch32x035/Link.ld -L${PICOLIBC_LIB_DIR} -L${LIBGCC_DIR}")

# Apply flags
set(CMAKE_C_FLAGS "${MCU_FLAGS}")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS}")

include_directories(
  ${PICOLIBC_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/ch32x035
  ${CMAKE_SOURCE_DIR}/ch32x035/include
)

# Collect sources: MCU drivers + user-provided sources
file(GLOB_RECURSE MCU_C_SOURCES
  ${CMAKE_SOURCE_DIR}/ch32x035/src/*.c
  # ${CMAKE_SOURCE_DIR}/ch32x035/src/Udisk_Lib/*.c
)

# If the user didn't provide SOURCES via -DSOURCES=..., default to src/main.c
if(NOT DEFINED SOURCES OR "${SOURCES}" STREQUAL "")
  set(SOURCES "${CMAKE_SOURCE_DIR}/src/main.c")
endif()

set(SOURCES ${SOURCES} "${CMAKE_SOURCE_DIR}/ch32x035/startup_ch32x035.S")

add_executable(ch32x035f8u6.elf ${MCU_C_SOURCES} ${SOURCES})

# Link picolibc libraries
target_link_directories(ch32x035f8u6.elf PRIVATE ${PICOLIBC_LIB_DIR} ${LIBGCC_DIR})
target_link_libraries(ch32x035f8u6.elf 
  -Wl,--start-group
  c 
  m
  gcc
  -Wl,--end-group
)

# Ensure startup file is treated as assembly
set_source_files_properties(${CMAKE_SOURCE_DIR}/ch32x035/startup_ch32x035.S PROPERTIES LANGUAGE ASM)

target_compile_options(ch32x035f8u6.elf PRIVATE ${MCU_FLAGS})

add_custom_command(TARGET ch32x035f8u6.elf POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/build
  COMMAND llvm-objcopy -O ihex $<TARGET_FILE:ch32x035f8u6.elf> ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.hex
  COMMAND llvm-objcopy --only-section=.vector --only-section=.init --only-section=.text --only-section=.rodata --only-section=.srodata --only-section=.data --only-section=.preinit_array --only-section=.init_array --only-section=.fini_array --only-section=.ctors --only-section=.dtors -O binary $<TARGET_FILE:ch32x035f8u6.elf> ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.rawbin
  COMMAND ${CMAKE_COMMAND} -E echo "Trimming trailing 0x00 from raw binary..."
  COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_SOURCE_DIR}/cmake/trim_bin.py ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.rawbin ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.bin
  COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.rawbin
  COMMAND llvm-size --format=berkeley $<TARGET_FILE:ch32x035f8u6.elf> > ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.size || true
  COMMENT "Generating HEX/BIN and size"
)

add_custom_target(flash
  COMMAND wchisp flash ${CMAKE_BINARY_DIR}/build/ch32x035f8u6.bin
  DEPENDS ch32x035f8u6.elf
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Flashing CH32X035 with wchisp"
)

message(STATUS "Configured to build with C compiler: ${CMAKE_C_COMPILER}")
