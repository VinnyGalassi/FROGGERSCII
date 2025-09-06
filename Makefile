
CC = gcc
# TODO: find flags to minimize binary size
# -nostartfiles -nodefaultlibs -nostdlib -ffreestanding -fno-builtin -fdata-sections -ffunction-sections -Wl,--gc-sections

# -nostartfiles removes interface for main() that OS uses
# -freestanding tells compiler there is no standard library
# -fno-builtin prevents compiler from using built-in functions that may not be available

# gcmasher recommended flags:
# gcc -O -ffast-math -fomit-frame-pointer -fauto-inc-dec -mpush-args -mno-red-zone -mstackrealign -fno-inline -nostartfiles -o ../frogger ../src/frogger.c

# CFLAGS = -O3 -Wall -s -Iinclude -nostartfiles
CFLAGS = -O -Wall -s -Iinclude -ffast-math -fomit-frame-pointer -fauto-inc-dec -mpush-args -mno-red-zone -mstackrealign -fno-inline -nostartfiles

SRC = src/frogger.c
OUT = frogger

# Always start with a clean build
all: clean $(OUT) size

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)
# Strip binary
	strip $(OUT)

size: $(OUT)
# Display info about executable size
	size $(OUT)
	@echo "Stripped binary size in bytes:"
	@wc -c $(OUT)
	@file $(OUT)

clean:
	rm -f $(OUT)