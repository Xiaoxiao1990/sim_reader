#SIM Cards Pool Manager Program 
#Date:2016.11.14
#Author:Zhou Linlin
#E-mail:461146760@qq.com
#GCC = arm-none-linux-gnueabi-gcc
objects = sim_reader.o spi.o simcard.o funcs.o
target = sim
mygcc = arm-none-linux-gnueabi-gcc -static
$(target):$(objects)
	$(mygcc) -o $(target) $(objects)
sim_reader.o:inc/includes.h
	arm-none-linux-gnueabi-gcc -c sim_reader.c -static
simcard.o:inc/includes.h simcard.c
	arm-none-linux-gnueabi-gcc -c simcard.c -static
spi.o:spi.c inc/includes.h
	arm-none-linux-gnueabi-gcc -c spi.c -static
funcs.o:funcs.c inc/includes.h
	arm-none-linux-gnueabi-gcc -c funcs.c -static
.PHONY:clean
clean:
	rm -rf *.o $(target)
