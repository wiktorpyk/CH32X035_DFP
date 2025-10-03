#ifndef __CH32X035_IT_H
#define __CH32X035_IT_H

/* Define one of these before including to enable RTOS-aware handlers:
 *  - CH32X035_USE_RTTHREAD  : RT-Thread hooks (rtthread.h)
 *  - CH32X035_USE_LOS       : Huawei LiteOS hooks (los_interrupt.h)
 */

#ifdef CH32X035_USE_RTTHREAD
#include <rtthread.h>
#endif
#ifdef CH32X035_USE_LOS
#include "los_interrupt.h"
#endif

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void) __attribute__((interrupt));
void HardFault_Handler(void) __attribute__((interrupt));
void SW_Handler(void) __attribute__((interrupt));

#ifdef __cplusplus
}
#endif

#endif /* __CH32X035_IT_H */
