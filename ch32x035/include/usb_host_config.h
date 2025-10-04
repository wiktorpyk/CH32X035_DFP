#ifndef __USB_HOST_CONFIG_H
#define __USB_HOST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch32x035_usb.h>
#include <ch32x035_usbfs_host.h>

/* Debug macro (enable to print debug messages) */
#ifndef DEF_DEBUG_PRINTF
#define DEF_DEBUG_PRINTF            1
#endif
#if ( DEF_DEBUG_PRINTF == 1 )
#define DUG_PRINTF( format, arg... )    printf( format, ##arg )
#else
#define DUG_PRINTF( format, arg... )    do{ if( 0 )printf( format, ##arg ); }while( 0 );
#endif

/******************************************************************************/
/* USB Host Communication Related Macro Definition (sane defaults taken from variants)
   Adjust these to your project needs or override via compiler defines.
*/

/* USB Host Port General Control */
#ifndef DEF_TOTAL_ROOT_HUB
#define DEF_TOTAL_ROOT_HUB          1
#endif
#ifndef DEF_USBFS_PORT_EN
#define DEF_USBFS_PORT_EN           1
#endif
#ifndef DEF_USBFS_PORT_INDEX
#define DEF_USBFS_PORT_INDEX        0x00
#endif
#ifndef DEF_ONE_USB_SUP_DEV_TOTAL
#define DEF_ONE_USB_SUP_DEV_TOTAL   1
#endif
#ifndef DEF_NEXT_HUB_PORT_NUM_MAX
#define DEF_NEXT_HUB_PORT_NUM_MAX   4
#endif
#ifndef DEF_INTERFACE_NUM_MAX
#define DEF_INTERFACE_NUM_MAX       4
#endif

/* USB Root Device Status */
#define ROOT_DEV_DISCONNECT         0
#define ROOT_DEV_CONNECTED          1
#define ROOT_DEV_FAILED             2
#define ROOT_DEV_SUCCESS            3

/* USB Device Address (default assigned during enumeration) */
#define USB_DEVICE_ADDR             0x02

/* USB Speed */
#define USB_LOW_SPEED               0x00
#define USB_FULL_SPEED              0x01
#define USB_HIGH_SPEED              0x02
#define USB_SPEED_CHECK_ERR         0xFF

/* Configuration Descriptor Type */
#define DEF_DECR_CONFIG             0x02
#define DEF_DECR_INTERFACE          0x04
#define DEF_DECR_ENDPOINT           0x05
#define DEF_DECR_HID                0x21

/* USB Communication Status Code */
#define ERR_SUCCESS                 0x00
#define ERR_USB_CONNECT             0x15
#define ERR_USB_DISCON              0x16
#define ERR_USB_BUF_OVER            0x17
#define ERR_USB_DISK_ERR            0x1F
#define ERR_USB_TRANSFER            0x20
#define ERR_USB_UNSUPPORT           0xFB
#define ERR_USB_UNAVAILABLE         0xFC
#define ERR_USB_UNKNOWN             0xFE

/* USB Device Enumeration Status Code */
#define DEF_DEV_DESCR_GETFAIL       0x45
#define DEF_DEV_ADDR_SETFAIL        0x46
#define DEF_CFG_DESCR_GETFAIL       0x47
#define DEF_REP_DESCR_GETFAIL       0x48
#define DEF_CFG_VALUE_SETFAIL       0x49
#define DEF_DEV_TYPE_UNKNOWN        0xFF

/* USB Communication Timeouts (ms or unit as used in project) */
#ifndef DEF_BUS_RESET_TIME
#define DEF_BUS_RESET_TIME          11          /* USB bus reset time */
#endif
#ifndef DEF_RE_ATTACH_TIMEOUT
#define DEF_RE_ATTACH_TIMEOUT       100         /* Wait for device reconnect (ms) */
#endif
#ifndef DEF_WAIT_USB_TRANSFER_CNT
#define DEF_WAIT_USB_TRANSFER_CNT   1000        /* Poll count for transfers */
#endif
#ifndef DEF_CTRL_TRANS_TIMEOVER_CNT
#define DEF_CTRL_TRANS_TIMEOVER_CNT 200000/20   /* Default control transfer timeout unit from samples */
#endif

/* Useful constants used by host code */
#ifndef DEF_USBD_FS_PACK_SIZE
#define DEF_USBD_FS_PACK_SIZE       64
#endif

/*******************************************************************************/
/* Struct Definition (common across variants) */

/* HUB Port Device  */
typedef struct _HUB_DEVICE
{
    uint8_t  bStatus;
    uint8_t  bType;
    uint8_t  bAddress;
    uint8_t  bSpeed;
    uint8_t  bEp0MaxPks;
    uint8_t  DeviceIndex;
}HUB_DEVICE, *PHUB_DEVICE;

/* Root HUB Device Structure */
typedef struct _ROOT_HUB_DEVICE
{
    uint8_t  bStatus;
    uint8_t  bType;
    uint8_t  bAddress;
    uint8_t  bSpeed;
    uint8_t  bEp0MaxPks;
    uint8_t  DeviceIndex;
    uint8_t  bPortNum;
    HUB_DEVICE Device[ DEF_NEXT_HUB_PORT_NUM_MAX ];
} ROOT_HUB_DEVICE, *PROOT_HUB_DEVICE;

/* USB Host Control Structure (trimmed common fields)
   The full project variants include additional fields per-interface; adapt as needed. */
typedef struct __HOST_CTL
{
    uint8_t  InterfaceNum;
    uint8_t  ErrorCount;

    struct interface
    {
        uint8_t  Type;

        uint8_t  InEndpNum;
        uint8_t  InEndpAddr[ 4 ];
        uint8_t  InEndpType[ 4 ];
        uint16_t InEndpSize[ 4 ];
        uint8_t  InEndpTog[ 4 ];
        uint8_t  InEndpInterval[ 4 ];
        uint8_t  InEndpTimeCount[ 4 ];

        uint8_t  OutEndpNum;
        uint8_t  OutEndpAddr[ 4 ];
        uint8_t  OutEndpType[ 4 ];
        uint16_t OutEndpSize[ 4 ];
        uint8_t  OutEndpTog[ 4 ];

    } Interface[ DEF_INTERFACE_NUM_MAX ];
} HOST_CTL, *PHOST_CTL;

/*******************************************************************************/
/* External variable declarations used by host examples (define these in your app)
   If your project already provides variants of these files, you can remove these
   externs or change them to match your project layout. */
extern ROOT_HUB_DEVICE RootHubDev;
extern HOST_CTL HostCtl[];
extern uint8_t DevDesc_Buf[];
extern uint8_t Com_Buf[];

#ifdef __cplusplus
}
#endif

#endif /* __USB_HOST_CONFIG_H */
