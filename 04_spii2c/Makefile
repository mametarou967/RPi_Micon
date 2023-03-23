CROSS= arm-unknown-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)ld
OBJCOPY= $(CROSS)objcopy

STARTUP= startup.o
OBJS= main.o

OBJS+= rpi_lib/rpi_init.o
OBJS+= rpi_lib/bss/clearbss.o
OBJS+= rpi_lib/gpio/gpio.o
OBJS+= rpi_lib/timer/timer.o
OBJS+= rpi_lib/delay/delay.o
OBJS+= rpi_lib/uart/uart.o
OBJS+= syscall.o

CFLAGS = -mfloat-abi=soft -mlittle-endian
CFLAGS+= -fno-builtin
CFLAGS+= -O0
CFLAGS+= -nostartfiles
CFLAGS+= -nodefaultlibs

LIBS = -L/home/mametarou963/cross/rpi/arm-unknown-eabi/lib/gcc/arm-unknown-eabi/12.2.0
LIBS+= -L/home/mametarou963/cross/rpi/arm-unknown-eabi/arm-unknown-eabi/lib

all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -T rpiMicon.lds $^ -o $@ $(LIBS) -lc -lgcc

.SUFFIXES: .elf .img

.elf.img:
	$(OBJCOPY) -O binary $< $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(RM) -f *.o *.img *.elf */*.o */*/*.o
	$(RM) -f tags *~

tags::
	ctags *.[chS]
