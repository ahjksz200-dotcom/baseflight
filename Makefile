# --- CẤU HÌNH ---
TARGET = NAZE
OPTIMIZE = -Os -ffunction-sections -fdata-sections
ARCH_FLAGS = -mthumb -mcpu=cortex-m3

# Đường dẫn (Include paths)
INC = -I. -Isrc \
      -Ilib/CMSIS/CM3/CoreSupport \
      -Ilib/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
      -Ilib/STM32F10x_StdPeriph_Driver/inc

CFLAGS = $(ARCH_FLAGS) $(OPTIMIZE) $(INC) \
         -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -D$(TARGET) -std=gnu99 -Wall
LDFLAGS = $(ARCH_FLAGS) -Wl,--gc-sections -specs=nano.specs -Tstm32_flash.ld

# TỰ ĐỘNG TÌM KIẾM CÁC FILE NGUỒN (.c và .S)
SRC  = $(wildcard src/*.c)
ASRC = $(wildcard src/baseflight_startups/*.S) $(wildcard src/*.S)

# Chuyển đổi .c và .S thành .o (đặt hết vào obj/)
OBJ = $(SRC:src/%.c=obj/%.o) $(ASRC:src/baseflight_startups/%.S=obj/%.o) $(ASRC:src/%.S=obj/%.o)

# --- QUY TẮC BIÊN DỊCH ---
all: obj baseflight.hex

obj:
	@mkdir -p obj

baseflight.hex: baseflight.elf
	arm-none-eabi-objcopy -O ihex $< $@

baseflight.elf: $(OBJ)
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

# Biên dịch file C
obj/%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

# Biên dịch file S
obj/%.o: src/baseflight_startups/%.S
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/%.o: src/%.S
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj *.elf *.hex
