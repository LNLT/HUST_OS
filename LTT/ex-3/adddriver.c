#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define DRIVER_NUM 0 //随即分配主设备号传入参数

int driver_num = 0; //系统分配的主设备号
int sem = 0;        //互斥信号
char name[30] = "tt'driver";
char buffer_kernel[1024] = "Hello! Welcome to ltt's Drive!";  //打开设备时会显示的消息

ssize_t to_read(struct file *file_read, char __user *buf, size_t size, loff_t *p);
ssize_t to_write(struct file *file_write, const char __user *buf, size_t size, loff_t *p);
int to_open(struct inode *inode, struct file *file_open);
int to_release(struct inode *inode, struct file *file_release);

struct file_operations ope =
    {
        .read = to_read,
        .write = to_write,
        .release = to_release,
        .open = to_open};

int init_module()//注册
{
    int back = 0;
    printk("begin to init a driver.\n");
    back = register_chrdev(DRIVER_NUM, name, &ope);
    if (back < 0)
    {
        printk("register failed!\n");
        return -1;
    }
    if (driver_num == 0)
    {
        driver_num = back;
        printk("the driver has been registered!\n");
        printk("the id is %d:\n",driver_num);   
    }
    return 0;
}

void cleanup_module()//注销
{
    unregister_chrdev(driver_num, name);
    printk("the driver has been cleaned!\n");
}

ssize_t to_read(struct file *file_read, char __user *buf, size_t size, loff_t *p)
{//读取
    int back = copy_to_user(buf, buffer_kernel, sizeof(buffer_kernel));
    if(back == 0)
    {
        printk("read success!\n");
        return 0;
    }
    else
    {
        printk("can't read!\n");
        return -1;
    }
}
ssize_t to_write(struct file *file_write, const char __user *buf, size_t size, loff_t *p)
{//写入
    int back = copy_from_user(buffer_kernel, buf, sizeof(buffer_kernel));
    if(back == 0)
    {
        printk("write success!\n");
        return 0;
    }
    else
    {
        printk("can't write!\n");
        return -1;
    }
}
int to_open(struct inode *inode, struct file *file_open)
{//打开
    printk("main device is %d, and the slave device is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); 
    if (sem == 0)
    { 
        sem++; 
        try_module_get(THIS_MODULE);
        printk("the device is opened!\n");
        return 0; 
    }  
    else 
    { 
        printk("another process open the char device!\n");
        return -1; 
    }  
}
int to_release(struct inode *inode, struct file *file_release)
{//释放
    sem--;
    module_put(THIS_MODULE);
    printk("the device is released!\n");
    return 0;
}