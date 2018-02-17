PROJECT=can-node-firmware

SRC = start.s main.c
INC = 

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size


CFLAGS = -Wall -g -std=c11 -O0 -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb -ffunction-sections -fdata-sections -Wl,--gc-sections

all: $(PROJECT)

$(PROJECT): $(PROJECT).elf

$(PROJECT).elf:  $(SRC)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ -Tstm32f072rb.ld
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin
	$(OBJDUMP) -St $(PROJECT).elf > $(PROJECT).lst
	$(SIZE) $(PROJECT).elf

program: $(PROJECT).bin
	openocd -f /usr/share/openocd/scripts/board/st_nucleo_f0.cfg st_nucleo_f0.cfg -f flash_stm.cfg -c "flash_stm $(PROJECT).bin"

clean:
	rm -f *.o
	rm -f $(PROJECT).elf
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).map
	rm -f $(PROJECT).lst
	rm -f $(PROJECT).map
