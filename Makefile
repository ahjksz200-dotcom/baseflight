# --- CẤU HÌNH BIÊN DỊCH ---
TARGET = NAZE
ROOT = .
OPTIMIZE = -Os -ffunction-sections -fdata-sections
ARCH_FLAGS = -mthumb -mcpu=cortex-m3
CFLAGS = $(ARCH_FLAGS) $(OPTIMIZE) -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -D$(TARGET) -std=gnu99 -I.
LDFLAGS = $(ARCH_FLAGS) -Wl,--gc-sections -specs=nano.specs -T$(ROOT)/stm32_flash.ld

# Danh sách nguồn và object
NAZE_SRC = main.c sensors.c drv_mpu.c drv_spi.c drv_pwm.c drv_system.c utils.c startup_stm32f10x_md_gcc.S
NAZE_OBJS = $(NAZE_SRC:.c=.o)
NAZE_OBJS := $(NAZE_OBJS:.S=.o)

# --- QUY TẮC MẶC ĐỊNH (Cái này giúp hết lỗi No targets) ---
all: baseflight.hex

# Quy tắc tạo file HEX
baseflight.hex: baseflight.elf
	arm-none-eabi-objcopy -O ihex $< $@

# Quy tắc liên kết ELF
baseflight.elf: $(NAZE_OBJS)
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

# Quy tắc biên dịch file C
%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

# Quy tắc biên dịch file S
%.o: src/%.S
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.hex
