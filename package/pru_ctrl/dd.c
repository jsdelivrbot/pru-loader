#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>
 
#include "dd.h"
#include "pru_offsets.h"
#include "pru_ctrl.h"
#include "hw_cm_per.h"
#include "soc_AM335x.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

 


static int my_open(struct inode *i, struct file *f)
{  
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    void __iomem * c1; 
    void __iomem * c2;  
    void __iomem * c3;


    void __iomem * CLKSTCTRL; 
    void __iomem * CLKCTRL;  
    void __iomem * PRU0_DRAM_OFFSET;
    void __iomem * PRU1_DRAM_OFFSET;
    void __iomem * PRU0_IRAM_OFFSET;
    void __iomem * PRU1_IRAM_OFFSET; 
    
    void __iomem * PRU0_CTRL; 
    
    u32 c;
    u32 s;
    char * buf;
    u32 t;
    
    switch (cmd)
    {
        case PRU_RESET:   
            printk("starting resetting PRU block \n");
            /*                
            CLKSTCTRL = ioremap( (SOC_PRM_PER_REGS + CM_PER_L4LS_CLKSTCTRL ), sizeof(u32));
            CLKCTRL   = ioremap( (SOC_CM_PER_REGS  + CM_PER_ICSS_CLKCTRL   ), sizeof(u32));

            // *((unsigned int*) 0x44E00C00 ) |= 0x2; // reset PRU
            c = readl_relaxed(CLKSTCTRL);
            writel_relaxed(c |= 0x2, CLKSTCTRL);
            
             // *((unsigned int*) 0x44E00C00 ) &= 0xFFFFFFFD;
            c = readl_relaxed(CLKSTCTRL);
            writel_relaxed(c &= 0xFFFFFFFD, CLKSTCTRL);
            
            ///*((unsigned int*) 0x44E000E8 ) |= 0x02; // enabling the PRU-ICSS module taking it out of idle
            s = readl_relaxed(CLKCTRL);
            writel_relaxed(s |= 0x2, CLKCTRL);
            
            // *((unsigned int*) 0x44E00C00 ) |= 0x2; // reset PRU
            c = readl_relaxed(CLKSTCTRL);
            writel_relaxed(c |= 0x2, CLKSTCTRL);
            
            // *((unsigned int*) 0x44E00C00 ) &= 0xFFFFFFFD;
            c = readl_relaxed(CLKSTCTRL);
            writel_relaxed(c &= 0xFFFFFFFD, CLKSTCTRL);
            */
            c1   = ioremap( (0x44e00140 ), sizeof(u32));
            c2   = ioremap( (0x44E000E8 ), sizeof(u32));
            

            c = readl_relaxed(c1);
            writel_relaxed(0x2, c1);
            
            c = readl_relaxed(c2);
            writel_relaxed(0x2, c2);
            
            c3   = ioremap( (0x44E00C00 ), sizeof(u32));
            
            c = readl_relaxed(c3);
            writel_relaxed(c |= 0x2, c3);
  
            c = readl_relaxed(c3);
            writel_relaxed(c &= ~0x2, c3);
                
            iounmap(c1);
            iounmap(c2);
            iounmap(c3);
            printk("finished resetting PRU block \n");
            break;
            
        case PRU_CLEAR_MEMS:
                          
            PRU0_DRAM_OFFSET = ioremap( (SOC_PRUICSS1_REGS + SOC_PRUICSS_PRU0_DRAM_OFFSET), 8*1024);
            PRU1_DRAM_OFFSET = ioremap( (SOC_PRUICSS1_REGS + SOC_PRUICSS_PRU1_DRAM_OFFSET), 8*1024);
            PRU0_IRAM_OFFSET = ioremap( (SOC_PRUICSS1_REGS + SOC_PRUICSS_PRU0_IRAM_OFFSET), 8*1024);
            PRU1_IRAM_OFFSET = ioremap( (SOC_PRUICSS1_REGS + SOC_PRUICSS_PRU1_IRAM_OFFSET), 8*1024);
            
            memset_io(PRU0_DRAM_OFFSET, 0, 8*1024);
            memset_io(PRU1_DRAM_OFFSET, 0, 8*1024);
            
            memset_io(PRU0_IRAM_OFFSET, 0, 8*1024);
            memset_io(PRU1_IRAM_OFFSET, 0, 8*1024);
            
            iounmap(PRU0_DRAM_OFFSET);
            iounmap(PRU1_DRAM_OFFSET);
  
            iounmap(PRU0_IRAM_OFFSET);
            iounmap(PRU1_IRAM_OFFSET);        
              
            break;
        
        case PRU_COPY_INSTRUCTIONS:
            printk("starting coppying PRU-I block \n");
            
            buf = (char*)kmalloc(8*1024, GFP_KERNEL);
            if(buf == NULL)
                return ENOMEM;
            
                       
            t = copy_from_user(buf, (char *)arg, 2*1024);
            
            if(t != 0)
                printk("failed to copy some stuff from userspace into kernal space ;( \n");

            PRU0_IRAM_OFFSET = ioremap(0x4A334000, 8*1024);
            
            memcpy(PRU0_IRAM_OFFSET, (void __force *) buf, 2*1024);
                        
            iounmap(PRU0_IRAM_OFFSET);
            
            printk("done coppying PRU-I block \n");
            
            break; 
        
       case PRU_START_EXECUTING:
            printk("Starting Execution \n");
            PRU0_CTRL = ioremap( (SOC_PRUICSS1_REGS + SOC_PRUICSS_PRU0_CTRL_OFFSET), sizeof(u32));
            writel_relaxed(0xb, PRU0_CTRL);
            iounmap(PRU0_IRAM_OFFSET);
            printk("Done execution \n");
            break;
            
        default:
            return -EINVAL;
    }
 
    return 0;
}
 
static struct file_operations pru_ctrl_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};
 
static int __init pru_ctrl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "pru_ctrl")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &pru_ctrl_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "pru_ctrl")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
 
    return 0;
}
 
static void __exit pru_ctrl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(pru_ctrl_init);
module_exit(pru_ctrl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sam Hoshyar");
MODULE_DESCRIPTION("AM335x PRU Driver");
