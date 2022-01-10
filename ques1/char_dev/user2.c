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
	char *Kbuff="The second user space says BYE KERNEL"; //msg for kernel
	
	fd=open("/dev/MY111",O_RDWR,0777);
	
	if(fd<0)
	{
		perror("user space: error\n"); //gives the exact error
		exit(1);
	}
 	
 	
	
	write(fd,Kbuff,50); //write functionality "write in kernel"
	read(fd,Ubuff,50);//read functionality "read from kernel"
	
	printf("the data read from kernel is %s\n",Ubuff); //printd the data from kernel
	close(fd);
	return 0;
}

