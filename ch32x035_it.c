#include "ch32x035_it.h"

#ifdef CH32X035_USE_RTTHREAD
#include "board.h"
#endif

void NMI_Handler(void) __attribute__((interrupt));
void HardFault_Handler(void) __attribute__((interrupt));
void SW_Handler(void) __attribute__((interrupt));

void NMI_Handler(void)
{
#ifdef CH32X035_USE_RTTHREAD
    GET_INT_SP();
    rt_interrupt_enter();
    rt_kprintf("NMI Handler\n");
    rt_interrupt_leave();
    FREE_INT_SP();
#elif defined(CH32X035_USE_LOS)
    GET_INT_SP();
    HalIntEnter();
    while(1) { }
    HalIntExit();
    FREE_INT_SP();
#else
    /* Bare metal: hang */
    while(1) { }
#endif
}

void HardFault_Handler(void)
{
#ifdef CH32X035_USE_RTTHREAD
    GET_INT_SP();
    rt_interrupt_enter();
    rt_kprintf("HardFault\n");
    rt_interrupt_leave();
    FREE_INT_SP();
    NVIC_SystemReset();
    while(1) { }
#elif defined(CH32X035_USE_LOS)
    GET_INT_SP();
    HalIntEnter();
    /* Print low-level fault info if available */
    printf("mcause:%08x\n",__get_MCAUSE());
    printf("mtval:%08x\n",__get_MTVAL());
    printf("mepc:%08x\n",__get_MEPC());
    NVIC_SystemReset();
    while(1) { }
    HalIntExit();
    FREE_INT_SP();
#else
    /* Bare metal: attempt soft reset */
    NVIC_SystemReset();
    while (1) { }
#endif
}

void SW_Handler(void)
{
#ifdef CH32X035_USE_RTTHREAD
    /* Example: jump to software handler address used on some platforms */
    __asm__("li  a6, 0x06000");
    __asm__("jr  a6");
    while(1);
#else
    while(1) { }
#endif
}
