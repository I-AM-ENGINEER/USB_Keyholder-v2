/////////////////////////////////////////////////////////////
//// Auto generate by TeenyDT, http://dt.tusb.org
/////////////////////////////////////////////////////////////
#include "teeny_usb_desc.h"

#define  KB_DEVICE_DESCRIPTOR_SIZE  (18)
__ALIGN_BEGIN  const uint8_t KB_DeviceDescriptor [18] __ALIGN_END = {
  ///////////////////////////////////////
  /// device descriptor
  ///////////////////////////////////////
  0x12,                                             /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,                       /* bDescriptorType */
  0x00, 0x02,                                       /* bcdUSB */
  0x00,                                             /* bDeviceClass */
  0x00,                                             /* bDeviceSubClass */
  0x00,                                             /* bDeviceProtocol */
  0x40,                                             /* bMaxPacketSize */
  0x83, 0x04,                                       /* idVendor */
  0x11, 0x00,                                       /* idProduct */
  0x00, 0x01,                                       /* bcdDevice */
  0x01,                                             /* iManufacturer */
  0x02,                                             /* iProduct */
  0x03,                                             /* iSerial */
  0x01,                                             /* bNumConfigurations */
};
#define  KB_CONFIG1_DESCRIPTOR_SIZE  (41)
__ALIGN_BEGIN  const uint8_t KB_ConfigDescriptor1 [41] __ALIGN_END = {
  ///////////////////////////////////////
  /// config descriptor
  ///////////////////////////////////////
  0x09,                                             /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,                /* bDescriptorType */
  0x29, 0x00,                                       /* wTotalLength */
  0x01,                                             /* bNumInterfaces */
  0x01,                                             /* bConfigurationValue */
  0x00,                                             /* iConfiguration */
  0x80,                                             /* bmAttributes */
  0x64,                                             /* bMaxPower */
  
  ///////////////////////////////////////
  /// interface descriptor
  ///////////////////////////////////////
  0x09,                                             /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,                    /* bDescriptorType */
  0x00,                                             /* bInterfaceNumber */
  0x00,                                             /* bAlternateSetting */
  0x02,                                             /* bNumEndpoints */
  0x03,                                             /* bInterfaceClass */
  0x01,                                             /* bInterfaceSubClass */
  0x01,                                             /* bInterfaceProtocol */
  0x00,                                             /* iInterface */
  
  ///////////////////////////////////////
  /// hid descriptor
  ///////////////////////////////////////
  0x09,                                             /* bLength */
  0x21,                                             /* bDescriptorType */
  0x11, 0x01,                                       /* bcdHID */
  0x00,                                             /* bCountryCode */
  0x01,                                             /* bNumDescriptors */
  0x22,                                             /* bDescriptorType1 */
  0x3f, 0x00,                                       /* wDescriptorLength1 */
  
  ///////////////////////////////////////
  /// endpoint descriptor
  ///////////////////////////////////////
  0x07,                                             /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,                     /* bDescriptorType */
  0x81,                                             /* bEndpointAddress */
  0x03,                                             /* bmAttributes */
  0x08, 0x00,                                       /* wMaxPacketSize */
  0x01,                                             /* bInterval */
  
  ///////////////////////////////////////
  /// endpoint descriptor
  ///////////////////////////////////////
  0x07,                                             /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,                     /* bDescriptorType */
  0x01,                                             /* bEndpointAddress */
  0x03,                                             /* bmAttributes */
  0x08, 0x00,                                       /* wMaxPacketSize */
  0x01,                                             /* bInterval */
};
#define KB_CONFIG_COUNT 1
const desc_t KB_ConfigDescriptors[KB_CONFIG_COUNT] = {
  KB_ConfigDescriptor1,
};
#define  KB_STRING_DESCRIPTOR0_STR   "\x09\x04"
#define  KB_STRING_DESCRIPTOR0_SIZE  (4)
WEAK __ALIGN_BEGIN  const uint8_t KB_StringDescriptor0 [4] __ALIGN_END = {
  0x04,                                         /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
  0x09, 0x04,                                   /* wLangID0 */
};
#define  KB_STRING_DESCRIPTOR1_STR   "TeenyUSB"
#define  KB_STRING_DESCRIPTOR1_SIZE   (18)
WEAK __ALIGN_BEGIN  const uint8_t KB_StringDescriptor1 [18] __ALIGN_END = {
  0x12,                                             /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,                       /* bDescriptorType */
  'T', 0x00,                                        /* wcChar0 */
  'e', 0x00,                                        /* wcChar1 */
  'e', 0x00,                                        /* wcChar2 */
  'n', 0x00,                                        /* wcChar3 */
  'y', 0x00,                                        /* wcChar4 */
  'U', 0x00,                                        /* wcChar5 */
  'S', 0x00,                                        /* wcChar6 */
  'B', 0x00,                                        /* wcChar7 */
};
#define  KB_STRING_DESCRIPTOR2_STR   "TeenyUSB Boot Keyboard DEMO"
#define  KB_STRING_DESCRIPTOR2_SIZE   (56)
WEAK __ALIGN_BEGIN  const uint8_t KB_StringDescriptor2 [56] __ALIGN_END = {
  0x38,                                             /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,                       /* bDescriptorType */
  'T', 0x00,                                        /* wcChar0 */
  'e', 0x00,                                        /* wcChar1 */
  'e', 0x00,                                        /* wcChar2 */
  'n', 0x00,                                        /* wcChar3 */
  'y', 0x00,                                        /* wcChar4 */
  'U', 0x00,                                        /* wcChar5 */
  'S', 0x00,                                        /* wcChar6 */
  'B', 0x00,                                        /* wcChar7 */
  ' ', 0x00,                                        /* wcChar8 */
  'B', 0x00,                                        /* wcChar9 */
  'o', 0x00,                                        /* wcChar10 */
  'o', 0x00,                                        /* wcChar11 */
  't', 0x00,                                        /* wcChar12 */
  ' ', 0x00,                                        /* wcChar13 */
  'K', 0x00,                                        /* wcChar14 */
  'e', 0x00,                                        /* wcChar15 */
  'y', 0x00,                                        /* wcChar16 */
  'b', 0x00,                                        /* wcChar17 */
  'o', 0x00,                                        /* wcChar18 */
  'a', 0x00,                                        /* wcChar19 */
  'r', 0x00,                                        /* wcChar20 */
  'd', 0x00,                                        /* wcChar21 */
  ' ', 0x00,                                        /* wcChar22 */
  'D', 0x00,                                        /* wcChar23 */
  'E', 0x00,                                        /* wcChar24 */
  'M', 0x00,                                        /* wcChar25 */
  'O', 0x00,                                        /* wcChar26 */
};
#define  KB_STRING_DESCRIPTOR3_STR   "TUSB123456"
#define  KB_STRING_DESCRIPTOR3_SIZE   (22)
WEAK __ALIGN_BEGIN  const uint8_t KB_StringDescriptor3 [22] __ALIGN_END = {
  0x16,                                             /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,                       /* bDescriptorType */
  'T', 0x00,                                        /* wcChar0 */
  'U', 0x00,                                        /* wcChar1 */
  'S', 0x00,                                        /* wcChar2 */
  'B', 0x00,                                        /* wcChar3 */
  '1', 0x00,                                        /* wcChar4 */
  '2', 0x00,                                        /* wcChar5 */
  '3', 0x00,                                        /* wcChar6 */
  '4', 0x00,                                        /* wcChar7 */
  '5', 0x00,                                        /* wcChar8 */
  '6', 0x00,                                        /* wcChar9 */
};
#define KB_STRING_COUNT    (4)
const uint8_t* const KB_StringDescriptors[4] = {
  KB_StringDescriptor0,
  KB_StringDescriptor1,
  KB_StringDescriptor2,
  KB_StringDescriptor3,
};
#define HAS_WCID
#define HAS_WCID_20
#define WCID_VENDOR_CODE 0x17
#define KB_REPORT_DESCRIPTOR_SIZE_IF0  (63)
WEAK __ALIGN_BEGIN const uint8_t KB_ReportDescriptor_if0[KB_REPORT_DESCRIPTOR_SIZE_IF0] __ALIGN_END = {
        //  Boot keyboad report descriptor
        0x05, 0x01,     // USAGE_PAGE (Generic Desktop)
        0x09, 0x06,     // USAGE (Keyboard)
        0xa1, 0x01,     // COLLECTION (Application)
        0x05, 0x07,     //   USAGE_PAGE (Keyboard)
        0x19, 0xe0,     //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7,     //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00,     //   LOGICAL_MINIMUM (0)
        0x25, 0x01,     //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,     //   REPORT_SIZE (1)
        0x95, 0x08,     //   REPORT_COUNT (8)
        0x81, 0x02,     //   INPUT (Data,Var,Abs)
        0x95, 0x01,     //   REPORT_COUNT (1)
        0x75, 0x08,     //   REPORT_SIZE (8)
        0x81, 0x03,     //   INPUT (Cnst,Var,Abs)
        0x95, 0x05,     //   REPORT_COUNT (5)
        0x75, 0x01,     //   REPORT_SIZE (1)
        0x05, 0x08,     //   USAGE_PAGE (LEDs)
        0x19, 0x01,     //   USAGE_MINIMUM (Num Lock)
        0x29, 0x05,     //   USAGE_MAXIMUM (Kana)
        0x91, 0x02,     //   OUTPUT (Data,Var,Abs)
        0x95, 0x01,     //   REPORT_COUNT (1)
        0x75, 0x03,     //   REPORT_SIZE (3)
        0x91, 0x03,     //   OUTPUT (Cnst,Var,Abs)
        0x95, 0x06,     //   REPORT_COUNT (6)
        0x75, 0x08,     //   REPORT_SIZE (8)
        0x15, 0x00,     //   LOGICAL_MINIMUM (0)
        0x25, 0x65,     //   LOGICAL_MAXIMUM (101)
        0x05, 0x07,     //   USAGE_PAGE (Keyboard)
        0x19, 0x00,     //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65,     //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00,     //   INPUT (Data,Ary,Abs)
        0xc0            // END_COLLECTION
    };


#if defined(KB_WCID_DESCRIPTOR_SIZE) || \
    defined(KB_WCID_PROPERTIES_SIZE) || \
    defined(KB_WCID_BOS_SIZE) || \
    defined(KB_WCID_DESC_SET_SIZE)

const tusb_extra_desc_t KB_extra_descriptor = {
#if defined(KB_WCID_DESCRIPTOR_SIZE)
    .wcid_desc = KB_WCIDDescriptor,
#else
    .wcid_desc = 0,
#endif // KB_WCID_DESCRIPTOR_SIZE)

#if defined(KB_WCID_PROPERTIES_SIZE)
    .wcid_properties = KB_WCIDProperties,
#else
    .wcid_properties = 0,
#endif // KB_WCID_PROPERTIES_SIZE

#if defined(KB_WCID_BOS_SIZE)
    .wcid_bos = KB_WCIDBOS,
#else
    .wcid_bos = 0,  
#endif // KB_WCID_BOS_SIZE)

#if defined(KB_WCID_DESC_SET_SIZE)
    .wcid_desc_set = KB_WCIDDescriptorSet,
#else
    .wcid_desc_set = 0,  
#endif // KB_WCID_DESC_SET_SIZE
};
#define KB_EXT_DESC  (&KB_extra_descriptor)
#else
#define KB_EXT_DESC  (0)
#endif


//KB_TXEP_MAX_SIZE
//KB_RXEP_MAX_SIZE
//  Device descriptors
const tusb_descriptors_t KB_descriptors = {
  .device = KB_DeviceDescriptor,
  .configs = KB_ConfigDescriptors,
  .config_count = KB_CONFIG_COUNT,
  .strings = KB_StringDescriptors,
  .string_cnt = KB_STRING_COUNT,
  .extra = KB_EXT_DESC,
};
