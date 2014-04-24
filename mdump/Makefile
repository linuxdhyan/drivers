#follwing command  states that there is one module to be built from the object
#file dump.o. 

obj-m+=dump.o
dump-objs :=mdump.o file_write.o

#The following command  starts  by  changing  its  directory  to  the one 
#provided with  the  -C option (that is, your kernel source directory). 
#There it finds the kernel’s top-level makefile. The M= option causes that 
#makefile to move back into your module source directory before trying to build 
#the modules target. This target, in turn, refers to the list of modules found 
#in the obj-m variable, which we’ve set to module.o in our examples.
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# cross compile option
#all:
#	make CROSS_COMPILE=/PATH_TO_ROSS_COMPILER/ppc_83xx-  -C /home/abah/proj/linux-2.6.18_pro500/ M=$(PWD) modules
#clean:	
#	make CROSS_COMPILE=/PATH_TO_CROSS_COMPILER/ppc_83xx- -C /home/abah/linux-2.6.18_pro500/ M=$(PWD) clean
	

