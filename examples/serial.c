#define DEBUG  DEBUG_USB_CDC  // Enable USB CDC debug mode

#include "debug.h"
#include "ch32x035_usbfs_device.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    
    RCC_Configuration();
    USBFS_RCC_Init();
    USBFS_Device_Init(ENABLE, PWR_VDD_SupplyVoltage());
    
    USB_CDC_Printf_Init();
    
    Delay_Ms(500);
    
    printf("USB CDC Debug Example\r\n");
    printf("System Clock: %d Hz\r\n", SystemCoreClock);
    printf("Chip ID: %08x\r\n", DBGMCU_GetCHIPID());
    
    uint32_t counter = 0;
    
    while(1)
    {
        /* Your application code here */
        printf("Counter: %lu\r\n", counter++);
        
        /* Process USB CDC output (optional but recommended) */
        USB_CDC_Process();
        
        Delay_Ms(1000);
    }
}