CC = gcc
# -nostartfiles -nodefaultlibs -nostdlib -ffreestanding -fno-builtin -fdata-sections -ffunction-sections -Wl,--gc-sections

# -nostartfiles removes interface for main() that OS uses
# -freestanding tells compiler there is no standard library
# -fno-builtin prevents compiler from using built-in functions that may not be available

# gcmasher recommended flags:
# gcc -O -ffast-math -fomit-frame-pointer -fauto-inc-dec -mpush-args -mno-red-zone -mstackrealign -fno-inline -nostartfiles -o ../frogger ../src/frogger.c

CFLAGS = -Os -Wall -Wextra -s -Iinclude -nostartfiles
LDFLAGS =

SRC = src/app.c src/renderer.c src/input.c src/lanes.c src/custom_unistd.c
OUT = frogger

# Always start with a clean build
all: clean $(OUT) size

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(SRC)
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

# Tiny, size-optimized build profile
tiny: clean
	$(CC) -Os -s -fdata-sections -ffunction-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-plt -fmerge-constants -fno-ident -fomit-frame-pointer -fvisibility=hidden -nostartfiles -nostartfiles -ffreestanding -Iinclude \
		-Wl,--gc-sections -Wl,-s -o $(OUT) $(SRC)
	strip $(OUT)
	$(MAKE) size
