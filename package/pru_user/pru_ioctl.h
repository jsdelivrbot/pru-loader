#ifndef PRU_IOCTL_H
#define PRU_IOCTL_H
#include <linux/ioctl.h>
#define PRU_IOC_MAGIC  'k'


#define PRU_RESET                _IOW(PRU_IOC_MAGIC, 1, int)
#define PRU_CLEAR_MEMS           _IOW(PRU_IOC_MAGIC, 2, int)
#define PRU_COPY_INSTRUCTIONS    _IOW(PRU_IOC_MAGIC, 3, int)
#define PRU_COPY_DATA            _IOW(PRU_IOC_MAGIC, 4, int)
#define PRU_START_EXECUTING      _IO (PRU_IOC_MAGIC, 5)

#endif
