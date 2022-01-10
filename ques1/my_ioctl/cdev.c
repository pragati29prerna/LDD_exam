#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include "ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(" PRAGATI PRERNA");
MODULE_DESCRIPTION("ioctl question 1");

//function prototype

int myopen(struct inode *inode,struct file *filp);
ssize_t myread(struct file *filp,char __user *Ubuff, size_t count,loff_t *offp);
ssize_t mywrite(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp);
int myrelease(struct inode *inode,struct file *filp);
long myioctl(struct file *filp,unsigned int cmd, unsigned long arg);

//file operations
struct file_operations fops ={
	.open = myopen,
	.read = myread,
	.write = mywrite,
	.unlocked_ioctl= myioctl,
	.release = myrelease,
};

struct stats
{
	int size;
	char buff[8];
	int r_w;
	
	
};
struct stats *mystruct;

//open system call
int myopen(struct inode *inode,struct file *filp)
{
	printk("In kernel: open call system\n");
	return 0;
}

// read system call

ssize_t myread(struct file *filp,char __user *Ubuff, size_t count,loff_t *offp)
{
printk("In kernel: read call system\n");
      
	unsigned long result;
	printk("read command called :\n");
	mystruct->r_w = 0;
	result = copy_to_user((char *)Ubuff,(char *)mystruct->buff,count);

	if(result == 0)
	{
		printk("data succesfully read\n");
		mystruct->size = count;
		return count;
	}
	else if(result>0)
	{
		printk("data still remaining to be read\n");
		return (count - result);
	}
	else
	{
		printk("error reading data\n");
		
		return (-EFAULT);
	}
	return 0;
}
//write system call
ssize_t mywrite(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp)
{
printk("In kernel: Write call system\n");

       unsigned long result;
	
	printk("\nwriting data: \n");
	mystruct->r_w = 1;
	result = copy_from_user((char *)mystruct->buff,(char *)Ubuff,count);
	if(result == 0)
	{
		printk("data successfully read from user : %s",mystruct->buff);
		mystruct->size = count;
		return count;
	}
	else if(result>0)
	{
		printk("data partially read from user : %s",mystruct->buff);
		return (count-result);
	}
	else
	{
		printk("error writing data\n");
		return -EFAULT;
	}
	
        return 0;
        
}
//close system call
int myrelease(struct inode *inode,struct file *filp)
{
        printk("In kernel: close call system\n");
        return 0;
        
}

long myioctl(struct file *filp,unsigned int cmd, unsigned long arg)
{
	int result;

	//struct sent *mystruct;
	printk("IOCTL FUNCTION\n");
switch (cmd){

case GETSTATS:
	result=copy_to_user((struct stats*)&mystruct,(struct stats *)arg,sizeof(struct stats));
	if(result==0)
	{
	printk("successfully send");
	}
	else
	{
	printk("incomplete send");
	}
	break;
	
default:
	printk("\n command not found");
	return (-EINVAL);
	break;
}
return 0;
}
struct cdev *mycdev;

//init module
static int __init chardevice_init(void)
{
	dev_t mydev;
	int result;
	int MAJOR,MINOR;

	mydev=MKDEV(255,0);
	MAJOR=MAJOR(mydev);
	MINOR=MINOR(mydev);

	printk(" major=%d and minor= %d\n",MAJOR,MINOR);
	int res= register_chrdev_region(mydev,1,"ioctl");
	if(res<0)
	{
		printk(KERN_ALERT "not registerd\n");
		return(-1);
	}

mycdev = cdev_alloc();
mycdev->ops = &fops;
result=cdev_add(mycdev,mydev,1);
	if(result<0)
        {
                printk("not created\n");
                unregister_chrdev_region(mydev,1);
		return(-1);
        }

	return 0;
}

//cleanup module

static void __exit chardevice_exit(void)
{
	dev_t mydev;
        int MAJOR,MINOR;

        mydev=MKDEV(255,0);
        MAJOR=MAJOR(mydev);
        MINOR=MINOR(mydev);

        printk(" major=%d and minor= %d\n",MAJOR,MINOR);
        unregister_chrdev_region(mydev,1);
	cdev_del(mycdev);
	printk("unregistered\n");	 
}

module_init(chardevice_init);
module_exit(chardevice_exit);



