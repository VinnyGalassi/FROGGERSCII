CC = gcc
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
# -nostartfiles removes interface for main() that OS uses
# -nodefaultlibs -nostdlib -fno-builtin are not possible because we need some libraries like <termios.h>
	$(CC) -Os -s \
	-fdata-sections -ffunction-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-plt -fmerge-constants -fno-ident -fomit-frame-pointer -fvisibility=hidden -ffast-math -fomit-frame-pointer -fauto-inc-dec \
	-mpush-args -mno-red-zone -mstackrealign \
	-Wl,--gc-sections -Wl,-s \
	-ffreestanding -nostartfiles \
	-Iinclude -o $(OUT) $(SRC)

	strip $(OUT)
	$(MAKE) size
