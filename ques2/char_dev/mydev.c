//header for character device
#include <linux/cdev.h>
//header for unregistering device
#include <linux/fs.h>
//headers for device number
#include <linux/kdev_t.h>
#include <linux/types.h>
//header for modules
#include <linux/init.h>
#include <linux/module.h>
//header for accessing user buffer
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
//**************************************************function prototype user1***************************************************

int open_user1(struct inode *inode,struct file *filp);
ssize_t read_user1(struct file *filp,char __user *Ubuff, size_t count,loff_t *offp);
ssize_t write_user1(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp);
int release_user1(struct inode *inode,struct file *filp);

struct file_operations fops_user1={
	.open = open_user1,
	.read = read_user1,
	.write = write_user1,
	.release = release_user1,
};
//**************************************************function prototype user2***************************************************

int open_user2(struct inode *inode,struct file *filp);
ssize_t read_user2(struct file *filp,char __user *Ubuff, size_t count,loff_t *offp);
ssize_t write_user2(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp);
int release_user2(struct inode *inode,struct file *filp);

struct file_operations fops_user2={
	.open = open_user2,
	.read = read_user2,
	.write = write_user2,
	.release = release_user2,
};
char kernel_buff[50];

struct semaphore sema;
//struct semaphore sema2;

DECLARE_WAIT_QUEUE_HEAD(wq1);
int flag1 = 0;
DECLARE_WAIT_QUEUE_HEAD(wq2);
int flag2 = 0;
//********************************************user1****************************************************

//open system call
int open_user1(struct inode *inode,struct file *filp)
{
	printk("In kernel: open call system\n");
	return 0;
}
//read system call

ssize_t read_user1(struct file *filp, char __user *Ubuff, size_t count,loff_t *offp)
{
    int result;
    const char *Kbuff="HAY USER1";
    ssize_t sdata;
    //down(&sema1);
    result=copy_to_user((char*)Ubuff,(char*)Kbuff,sizeof(Kbuff));
    //up(&sema1);
    
    if(result==0)
    {

    printk("......successfully send the result to user1.......\n");
    sdata=count;
    
    flag1 = 1;
    printk(KERN_DEBUG " user1 awakening the readers...\n");
    wake_up_interruptible(&wq1);
    
    return sdata;
    }

    else if(result>0)
    {
        printk("user1: remaining data to read\n");
	sdata= (count - result);
        return sdata;
    }
    else 
    {
    printk("user1:.......error while sending data....\n");
    return -1;
    }
}
// write system call

ssize_t write_user1(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp)
{
int result;
ssize_t rdata;
if(flag1 ==0)
{
printk("process going to sleep\n");
wait_event_interruptible(wq1,flag1== 1 );
printk("awoken\n");
}

if(down_interruptible(&sema))
{
		return -ERESTARTSYS;
}
printk(KERN_ALERT "USER1 critical section started\n");
result=copy_from_user((char*)kernel_buff,(char*)Ubuff,count);
printk(KERN_ALERT "USER1 data from user: %s\n",kernel_buff);
up(&sema);
printk(KERN_ALERT "USER1 critical section ends\n");

if(result==0)
{
printk("user1:.......successfully write from user1....\n");

rdata=count;
return rdata;
}

else if(result>0)
{
printk("user1:.......part of data written.....\n");
rdata=(count-result);
return rdata;
}

else 
{
printk("user1:.......error while writing data....\n");
return -1;
}
}

//close system call
int release_user1(struct inode *inode,struct file *filp)
{
        printk("In kernel: close call system of user1\n");
        return 0;
}
//********************************************user2****************************************************
//open system call

int open_user2(struct inode *inode,struct file *filp)
{
	printk("In kernel: open call system user2\n");
	return 0;
}
//read system call

ssize_t read_user2(struct file *filp, char __user *Ubuff, size_t count,loff_t *offp)
{
    int result;
    ssize_t sdata;

    const char *Kbuff="HAY USER2";
    //down(&sema2);
    result=copy_to_user((char*)Ubuff,(char*)Kbuff,sizeof(Kbuff));
    //up(&sema2);
    
    if(result==0)
    {

    printk("user2:......successfully send the result to user.......\n");
    sdata=count;
    flag2 = 1;
    printk(KERN_ALERT "user2 awakening the readers...\n");
    wake_up_interruptible(&wq2);
    return sdata;
    }

else if(result>0)
{

printk("user2:.......part of data read.....\n");
sdata=(count-result);
return sdata;
}
else 
{
printk(".......error while writing data....\n");
return -1;
}

}
// write system call

ssize_t write_user2(struct file *filp,const char __user *Ubuff, size_t count,loff_t *offp)
{

int result;
ssize_t rdata;
if(flag2 == 0)
{
printk(KERN_ALERT "user2: process going to sleep\n");
printk(KERN_ALERT "awoken\n");
}

wait_event_interruptible(wq2,flag2== 1 );
if(down_interruptible(&sema))
{
		return -ERESTARTSYS;
}
printk(KERN_ALERT "USER2 critical section started\n");
result=copy_from_user((char*)kernel_buff,(char *)Ubuff,count);
up(&sema);
printk(KERN_ALERT "USER2 critical section ends\n");

if(result==0)
{
printk(".......successfully write user2...\n");
printk(KERN_ALERT " user2 data from user : %s\n",kernel_buff);

rdata=count;
return rdata;
}
else if(result>0)
{

printk("user2:.......part of data written.....\n");
rdata=(count-result);
return rdata;
}
else 
{
printk("user2:.......error while writing data....\n");
return -1;
}

}

//close system call
int release_user2(struct inode *inode,struct file *filp)
{
        printk("In kernel: close call system user2\n");
        return 0;
}      

//character device initilization

struct cdev *mycdev_user1;
struct cdev *mycdev_user2;

dev_t user1_dev;
dev_t user2_dev;
//dev_t mydev;

int res1,res2;

//***********************************************init module*************************************************************************

static int __init chardevice_init(void)
{
	
	int result1,result2;
	int MAJOR,MINOR1, MINOR2;

      /*result= alloc_chrdev_region(&mydev,0,2,"MyCharacterDevice");
	
	if(result<0)
	{
		printk(KERN_ALERT "not registerd\n");
		return(-1);
	}

	MAJOR=MAJOR(mydev);
	user1_dev=MKDEV(MAJOR,0);
	user2_dev=MKDEV(MAJOR,1);*/
	
	user1_dev=MKDEV(003,0);
	user2_dev=MKDEV(003,1);
	MAJOR=MAJOR(user1_dev);
	MINOR1=MINOR(user1_dev);
	MINOR2=MINOR(user2_dev);
	
	printk(" major=%d and minor= %d\n",MAJOR,MINOR1);
	printk(" major=%d and minor= %d\n",MAJOR,MINOR2);
	result1= register_chrdev_region(user1_dev,1,"MyCharDevice1");
	result2= register_chrdev_region(user2_dev,1,"MyCharDevice2");
	if(result1 || result2<0)
	{
		printk(KERN_ALERT "not registerd\n");
		return(-1);
	}

	
       // printk(" major=%d and minor= %d\n",MAJOR,MINOR);
				
mycdev_user1 = cdev_alloc();
mycdev_user1->ops = &fops_user1;
res1=cdev_add(mycdev_user1,user1_dev,1);

mycdev_user2 = cdev_alloc();
mycdev_user2->ops = &fops_user2;
res2=cdev_add(mycdev_user2,user2_dev,1);
sema_init(&sema,1);
//sema_init(&sema2,1);

	if(res1<0||res2<0)
        {
                printk("not created by kernel\n");
           
		return(-1);
        }

	return 0;
}

//*************************************************************cleanup module**********************************************************

static void __exit chardevice_exit(void)
{
	
        int MAJOR,MINOR1,MINOR2;
        user1_dev=MKDEV(003,0);
        user2_dev=MKDEV(003,1);
        MAJOR=MAJOR(user1_dev);
        MINOR1=MINOR(user1_dev);
        MINOR2=MINOR(user2_dev);
        

       // printk(" major=%d and minor= %d\n",MAJOR,MINOR);
        unregister_chrdev_region(user1_dev,1);
        unregister_chrdev_region(user2_dev,1);
        
	
	cdev_del(mycdev_user1);
	cdev_del(mycdev_user2);
	
	printk("unregistered\n");	 
}

module_init(chardevice_init);
module_exit(chardevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRERNA");
MODULE_DESCRIPTION("character driver linux kerner module");

