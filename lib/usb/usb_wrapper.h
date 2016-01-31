#include "usb/include/linux_wrapper.h"
#define __KERNEL__
#undef CONFIG_PCI
#define CONFIG_PCI

#include "usb/include/linux/usb.h"

#include <hal/input.h>
