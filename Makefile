export DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
ARCH = -march=armv5te -mtune=arm946e-s -mthumb -mthumb-interwork
CFLAGS = -O2 -Wall $(ARCH) -DARM9 -I$(DEVKITPRO)/libnds/include
LDFLAGS = -specs=ds_arm9.specs -g $(ARCH) -L$(DEVKITPRO)/libnds/lib

quintuplas.nds: quintuplas.elf
	ndstool -c $@ -7 $(DEVKITPRO)/libnds/default.elf -9 $<

quintuplas.elf: main.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lfat -lnds9

clean:
	rm -f quintuplas.elf quintuplas.nds
