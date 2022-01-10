#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "ioctl.h"

struct stats
{
	int size;
	char buff[8];
	int r_w;
};

int main()
	{
	int fd,result;
	struct stats *stats_app;
	
	stats_app = (struct stats *)malloc(sizeof(struct stats));
	
	printf("opening the driver\n");
	fd=open("/dev/MYdev",O_RDWR,0777);
	if(fd<0)
	{
		printf("cannot open\n");
		exit(1);
	}
	
	result = ioctl (fd,GETSTATS,&stats_app);
		if(result<0)
	{
		printf("IOCTL Error\n");
		return(-1);
	}
		if(stats_app->r_w == 0)
		{
		printf("recent actvity was Read\n");
		printf("the string sent by Application 1 is : %s\n",stats_app->buff);
		
		}
		else if(stats_app->r_w ==1)
		{
			printf("recent activity was write\n");
		}

        printf("size of data received: %d\n",stats_app->size);
	printf("data recieved: %s\n",stats_app->buff);
	printf("status is : %d\n",stats_app->r_w);
	close(fd);
	}
	
