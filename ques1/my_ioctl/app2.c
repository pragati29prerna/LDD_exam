#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>


int main()
{
	int fd;
	char Ubuff[8];
	
	fd=open("/dev/MYdev",O_RDONLY,0777);
	printf("opening for read \n");
	
	if(fd<0)
	{
		perror("user space: error\n"); //gives the exact error
		exit(1);
	}
 	
	
	read(fd,Ubuff,50);//read functionality "read from kernel"
       
	printf("data: %s\n",Ubuff);
	
	close(fd);
	return 0;
}

