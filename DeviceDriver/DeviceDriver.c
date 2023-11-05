#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include "vchar_device_driver.h"
#include <linux/slab.h>           // To have fully access to kmalloc and kfree
#define DEMO_VERSION "0.1"


typedef struct vchar_dev
{
    unsigned char* control_regs;
    unsigned char* status_regs;
    unsigned char* data_regs;
}vchar_dev_t;
/**
 * @brief This struct defines the properties needed for Device Driver
 * 
 */
struct DeviceInfo
{
    dev_t devNum;                 // Major Device Number
    struct class* devClass;       // Device Class to communicate with Kernel
    struct device* dev;           // Device File to read write open close
    vchar_dev_t*   vchar_hw;
}devIf;

int vchar_hw_init(vchar_dev_t* hw);

void vchar_hw_exit(vchar_dev_t* hw);

static int __init initState(void)
{
    int ret = 0;
    ret = alloc_chrdev_region(&devIf.devNum,1,0,"Demo");   // Dynamic allocation Device Number (major)
    if (ret < 0)
    {
        printk(KERN_ERR "Register Device Num failed\n");
        goto register_device_num_failed;
    }
    devIf.devClass = class_create(THIS_MODULE,"Demo");    // Regsiter Class with Owner and Name
    if (devIf.devClass == NULL)
    {
        printk(KERN_ERR "Register Class failed\n");
        goto register_class_failed;
    }
    // Register device file with Device Class, Parent, Device Number, Data and Name
    devIf.dev = device_create(devIf.devClass, NULL, devIf.devNum, NULL, "Demo");
    if (devIf.dev == NULL)
    {
        printk(KERN_ERR "Register Device failed\n");
        goto register_device_failed;
    }
    devIf.vchar_hw = kmalloc(sizeof(vchar_dev_t), GFP_KERNEL);
    if (devIf.vchar_hw == NULL)
    {
        printk(KERN_ERR "Allocation register pointer failed\n");
        ret=-ENOMEM;
        goto register_allocation_failed;
    }
    ret = vchar_hw_init(devIf.vchar_hw);
    if (ret < 0)
    {
        printk(KERN_ERR "Allocation register HW failed\n");
        goto allocation_hw_failed;
    }
    printk(KERN_INFO "Register device successully\n");
    return ret;
allocation_hw_failed:
    kfree(devIf.vchar_hw);
register_allocation_failed:
    device_destroy(devIf.devClass, devIf.devNum);
register_device_failed:
    class_destroy(devIf.devClass);
register_class_failed:
    unregister_chrdev_region(devIf.devNum,1);
register_device_num_failed:
    return ret;
}


static void __exit exitState(void)
{
    vchar_hw_exit(devIf.vchar_hw);
    kfree(devIf.vchar_hw);
    device_destroy(devIf.devClass, devIf.devNum);
    class_destroy(devIf.devClass);
    unregister_chrdev_region(devIf.devNum,1);
    printk(KERN_INFO "Unload Module\n");
}


int vchar_hw_init(vchar_dev_t* hw)
{
    char* buf = NULL;
    // GFP_KERNEL: usually used in process context. When caller
    // calls but the kernel does not have enough space to assign
    // it puts caller into sleep mode and trynna collect
    buf = kzalloc(NUM_DEVICE_REGS * REG_SIZE, GFP_KERNEL);
    if (buf == NULL)
    {
        return -ENOMEM;
    }

    hw->control_regs = buf;
    hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
    hw->data_regs = hw->status_regs + NUM_STS_REGS;

    hw->control_regs[CONTROL_ACCESS_REG] = 0x03; // Able to read and write data to data register
    hw->status_regs[DEVICE_STATUS_REG]   = 0x03;

    return 0;

}

void vchar_hw_exit(vchar_dev_t* hw)
{
    kfree(hw->control_regs);
}

module_init(initState);
module_exit(exitState);

MODULE_AUTHOR("Nguyen");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is only a test module");
MODULE_VERSION(DEMO_VERSION);

