#ifndef _MY_DEVICE_DRIVER_H_
#define _MY_DEVICE_DRIVER_H_

#include <linux/ioctl.h>

struct mydevice_values {
	int val1;
	int val2;
};

#define MYDEVICE_IOC_TYPE 'M'

#define MYDEVICE_SET_VALUES _IOW(MYDEVICE_IOC_TYPE, 1, struct mydevice_values)


#define MYDEVICE_GET_VALUES _IOW(MYDEVICE_IOC_TYPE, 2, struct mydevice_values)


#endif  /* _MY_DEVICE_DRIVER_H_ */

