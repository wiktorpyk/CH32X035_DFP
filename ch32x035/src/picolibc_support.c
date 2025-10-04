/*
 * Minimal picolibc support for embedded targets.
 * Provides global FILE objects and pointers for stdin/stdout/stderr so
 * printf/puts from picolibc can link. Low-risk; I/O is routed via
 * the existing _write implementation in debug.c.
 */


#include <stdio.h>

/*
 * Define actual FILE objects and const pointers for stdin/stdout/stderr.
 * Picolibc declares these as `extern FILE *const stdin;` etc, so the
 * definitions must match that type (const pointer to FILE).
 */
__attribute__((used))
static FILE __stdin_FILE;
__attribute__((used))
static FILE __stdout_FILE;
__attribute__((used))
static FILE __stderr_FILE;

/* Provide const pointers as required by picolibc headers. */
__attribute__((used))
FILE *const stdin  = &__stdin_FILE;
__attribute__((used))
FILE *const stdout = &__stdout_FILE;
__attribute__((used))
FILE *const stderr = &__stderr_FILE;
