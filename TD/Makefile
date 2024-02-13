PREFIX=arm-none-eabi-
AS	= $(PREFIX)as
CC 	= $(PREFIX)gcc
LD  = $(PREFIX)ld
GDB = $(PREFIX)gdb
OBJ	= $(PREFIX)objdump
OCP = $(PREFIX)objcopy

ASFLAGS 	= -g
CFLAGS		= -g -O2 -ffreestanding -I./CMSIS/Device/ST/STM32L4xx/Include -I./CMSIS/Include -Wall -Werror
CFLAGS_CLOCK= -g -Og -ffreestanding -I./CMSIS/Device/ST/STM32L4xx/Include -I./CMSIS/Include -Wall -Werror -mcpu=cortex-m4 -mthumb $(FPU)
#LDFLAGS		+= -Wl,--print-map
LDFLAGS		= -nostdlib -T ld_ram.lds
GDBFLAGS	= -x se203.gdb
TARGET_ARCH	= -mcpu=cortex-m4 -mthumb $(FPU)
FPU			= -mfpu=fpv4-sp-d16 -mfloat-abi=hard
EXE 	 = main
ASFILES  = $(wildcard *.s)
OBJFILES = crt0.o main.o init.o memfuncs.o led.o uart.o matrix.o irq.o button.o timer.o clocks.o image.o

all : $(EXE)

$(EXE) : $(OBJFILES)
	$(LD) $(LDFLAGS)  $^ -o $@

%.o : %.raw
	$(OCP) -I binary -O elf32-littlearm -B arm $^ $@

clocks.o: clocks.c
	$(CC) $(CFLAGS_CLOCK) -c $< -o $@

connect::
	JLinkGDBServer -device STM32L475VG -endian little -if SWD -speed auto -ir -LocalhostOnly

debug::
	$(GDB) $(GDBFLAGS) $(EXE)

disassembly::
	$(OBJ) -d $(EXE)

clean::
# rm -rf $(filter-out clocks.o, $(wildcard *.o)) $(EXE)
	rm -rf $(OBJFILES) $(EXE)
