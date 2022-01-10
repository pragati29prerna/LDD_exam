#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>

int main()
{
	int fd;
	char Ubuff[50];
	char *Kbuff="The first user space says HELLO KERNEL"; //msg for kernel
	
	fd=open("/dev/MY222",O_RDWR,0777);
	
	if(fd<0)
	{
		perror("user space: error\n"); //gives the exact error
		exit(1);
	}
 	
	
	read(fd,Ubuff,50);//read functionality "read from kernel"
        write(fd,Kbuff,50); //write functionality "write in kernel"
        
	printf("the data read from kernel is %s\n",Ubuff); //printd the data from kernel
	close(fd);
	return 0;
}

