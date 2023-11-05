#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>

#define DEMO_VERSION "0.1"

/**
 * @brief This struct defines the properties needed for Device Driver
 * 
 */
struct DeviceInfo
{
    dev_t devNum;                 // Major Device Number
    struct class* devClass;       // Device Class to communicate with Kernel
    struct device* dev;           // Device File to read write open close
}devIf;

static int __init initState(void)
{
    int ret = 0;
    ret = alloc_chrdev_region(&devIf.devNum,1,0,"Demo");   // Dynamic allocation Device Number (major)
    if (ret < 0)
    {
        goto register_device_num_failed;
    }
    devIf.devClass = class_create(THIS_MODULE,"Demo");    // Regsiter Class with Owner and Name
    if (devIf.devClass == NULL)
    {
        goto register_class_failed;
    }
    // Register device file with Device Class, Parent, Device Number, Data and Name
    devIf.dev = device_create(devIf.devClass, NULL, devIf.devNum, NULL, "Demo");
    if (devIf.dev == NULL)
    {
        goto register_device_failed;
    }
    printk(KERN_INFO "Register device successully\n");
    return ret;
register_device_failed:
    class_destroy(devIf.devClass);
register_class_failed:
    unregister_chrdev_region(devIf.devNum,1);
register_device_num_failed:
    return ret;
}

static void __exit exitState(void)
{
    device_destroy(devIf.devClass, devIf.devNum);
    class_destroy(devIf.devClass);
    unregister_chrdev_region(devIf.devNum,1);
    printk(KERN_INFO "Unload Module\n");
}

module_init(initState);
module_exit(exitState);

MODULE_AUTHOR("Nguyen");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is only a test module");
MODULE_VERSION(DEMO_VERSION);

