.RECIPEPREFIX = >
export DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
ARM7 = $(firstword $(wildcard $(DEVKITPRO)/calico/bin/ds7_maxmod.elf $(DEVKITPRO)/calico/bin/ds7*.elf $(DEVKITPRO)/libnds/default.elf))
ARCH = -march=armv5te -mtune=arm946e-s -mthumb -mthumb-interwork
CFLAGS = -O2 -Wall $(ARCH) -specs=ds_arm9.specs -DARM9 -I$(DEVKITPRO)/libnds/include -I$(DEVKITPRO)/calico/include
LDFLAGS = -specs=ds_arm9.specs -g $(ARCH) -L$(DEVKITPRO)/libnds/lib -L$(DEVKITPRO)/calico/lib

quintuplas.nds: quintuplas.elf
> ndstool -c $@ -7 $(ARM7) -9 $<

quintuplas.elf: main.c
> -ls $(DEVKITPRO)/calico/bin $(DEVKITPRO)/calico/lib
> $(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lfat -lnds9 -lcalico_ds9
