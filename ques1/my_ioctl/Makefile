obj-m:=cdev.o
KDIR=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o app1 app1.c
	gcc -o app2 app2.c
	gcc -o app3 app3.c

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf app1
	rm -rf app2
	rm -rf app3
