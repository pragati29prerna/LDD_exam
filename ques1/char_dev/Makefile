obj-m:=mydev.o
KDIR=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o user1 user1.c
	gcc -o user2 user2.c


clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf user1
	rm -rf user2
