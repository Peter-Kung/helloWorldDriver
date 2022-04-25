
#include <linux/ioctl.h>

#define IOC_MAGIC 'k'
#define IOC_MAXNR 3
#define SETBREAKFIRST _IOW(IOC_MAGIC, 1, int *)
#define SETDINNER     _IOW(IOC_MAGIC, 2, int *)
