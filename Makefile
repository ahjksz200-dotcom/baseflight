# Dùng cho chip 32KB
TARGET = NAZE
OPTIMIZE = -Os -ffunction-sections -fdata-sections
ARCH_FLAGS = -mthumb -mcpu=cortex-m3
CFLAGS = $(ARCH_FLAGS) $(OPTIMIZE) -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -D$(TARGET) -std=gnu99
LDFLAGS = $(ARCH_FLAGS) -Wl,--gc-sections -specs=nano.specs -T$(ROOT)/stm32_flash.ld

# CHỈ GIỮ LẠI CÁC FILE CỐT LÕI
NAZE_SRC = main.c sensors.c drv_mpu.c drv_spi.c drv_pwm.c drv_system.c utils.c startup_stm32f10x_md_gcc.S
