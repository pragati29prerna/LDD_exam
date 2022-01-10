#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>

int main()
{
	int fd;
	char *Ubuff="H"; //msg for kernel
	
	fd=open("/dev/MYdev",O_WRONLY,0777);
	printf("opening for write \n");
	if(fd<0)
	{
		perror("user space: error\n"); //gives the exact error
		exit(1);
	}
 	printf("opened \n");
        write(fd,Ubuff,sizeof(Ubuff));//read functionality "read from kernel"
        printf("close \n");
	close(fd);
	return 0;
}

