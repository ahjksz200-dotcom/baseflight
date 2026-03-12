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

# TỰ ĐỘNG TÌM FILE NGUỒN
# Lấy tất cả file .c và .S trong src/ và các thư mục con
SRC  = $(shell find src -name "*.c")
ASRC = $(shell find src -name "*.S")

# Chuyển đổi sang đường dẫn object tương ứng trong obj/
OBJ = $(SRC:src/%.c=obj/%.o) $(ASRC:src/%.S=obj/%.o)

# --- QUY TẮC BIÊN DỊCH ---
all: baseflight.hex

baseflight.hex: baseflight.elf
	arm-none-eabi-objcopy -O ihex $< $@

baseflight.elf: $(OBJ)
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

# Quy tắc tự động tạo thư mục con khi biên dịch
obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/%.o: src/%.S
	@mkdir -p $(dir $@)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj *.elf *.hex
