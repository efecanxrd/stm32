CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0 -g
LDFLAGS= -nostdlib -T stm32_ls.ld -Wl,-Map=final.map 

all: final.elf

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^
	
stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^
	
final.elf: main.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf
	
load:
	openocd -f board/stm32f4discovery.cfg