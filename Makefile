# --- CẤU HÌNH ---
TARGET = NAZE
OPTIMIZE = -Os -ffunction-sections -fdata-sections
ARCH_FLAGS = -mthumb -mcpu=cortex-m3

# CÁC ĐƯỜNG DẪN INCLUDE (Phải khớp với thư mục của bạn)
INCLUDE_DIRS = -I. -Isrc \
               -Ilib/CMSIS/CM3/CoreSupport \
               -Ilib/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
               -Ilib/STM32F10x_StdPeriph_Driver/inc

CFLAGS = $(ARCH_FLAGS) $(OPTIMIZE) $(INCLUDE_DIRS) \
         -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -D$(TARGET) -std=gnu99
LDFLAGS = $(ARCH_FLAGS) -Wl,--gc-sections -specs=nano.specs -Tstm32_flash.ld

# CÁC FILE NGUỒN CỐT LÕI
# Đảm bảo các file này thực sự nằm trong thư mục src/
SRC = src/main.c src/sensors.c src/drv_mpu.c src/drv_spi.c src/drv_pwm.c src/drv_system.c src/utils.c
ASRC = src/startup_stm32f10x_md_gcc.S

OBJS = $(SRC:.c=.o) $(ASRC:.S=.o)

# --- QUY TẮC BIÊN DỊCH ---
all: baseflight.hex

baseflight.hex: baseflight.elf
	arm-none-eabi-objcopy -O ihex $< $@

baseflight.elf: $(OBJS)
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.S
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) *.elf *.hex
