/*
 * This file is part of baseflight - Optimized for Blue Pill (128KB)
 * Licensed under GPL V3
 */
#pragma once

#define __USE_C99_MATH

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "stm32f10x_conf.h"
#include "core_cm3.h"

#ifndef __CC_ARM
#define USE_LAME_PRINTF
#include "printf.h"
#endif

#include "drv_system.h"
#include "drv_gpio.h"
#include "utils.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif

#define RADX10 (M_PI / 1800.0f)
#define RAD    (M_PI / 180.0f)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))

// --- 1. ĐỊNH NGHĨA CÁC ENUM VÀ STRUCT TRƯỚC (QUAN TRỌNG ĐỂ KHÔNG LỖI) ---

typedef enum HardwareRevision {
    NAZE32 = 1,
    NAZE32_REV5,
    NAZE32_SP,
    NAZE32_REV6,
} HardwareRevision;

typedef enum {
    ALIGN_DEFAULT = 0,
    CW0_DEG = 1,
    CW90_DEG = 2,
    CW180_DEG = 3,
    CW270_DEG = 4,
    CW0_DEG_FLIP = 5,
    CW90_DEG_FLIP = 6,
    CW180_DEG_FLIP = 7,
    CW270_DEG_FLIP = 8
} sensor_align_e;

typedef struct sensor_t {
    void (*init)(sensor_align_e align);
    void (*read)(int16_t *data);
    void (*temperature)(int16_t *data);
    float scale;
} sensor_t;

typedef struct baro_t {
    uint16_t ut_delay;
    uint16_t up_delay;
    void (*start_ut)(void);
    void (*get_ut)(void);
    void (*start_up)(void);
    void (*get_up)(void);
    void (*calculate)(int32_t *pressure, int32_t *temperature);
} baro_t;

typedef void (*serialReceiveCallbackPtr)(uint16_t data);

typedef enum {
    SENSOR_GYRO = 1 << 0,
    SENSOR_ACC = 1 << 1,
    SENSOR_BARO = 1 << 2,
    SENSOR_MAG = 1 << 3,
    SENSOR_SONAR = 1 << 4,
    SENSOR_GPS = 1 << 5,
    SENSOR_GPSMAG = 1 << 6,
} AvailableSensors;

// --- 2. CẤU HÌNH PINOUT CHO BLUE PILL ---
#if defined(NAZE)

#define LED0_GPIO   GPIOC
#define LED0_PIN    Pin_13      // LED Blue Pill (PC13)
#define LED1_GPIO   GPIOC 
#define LED1_PIN    Pin_13      // Blue Pill chỉ có 1 LED, gán trùng PC13 luôn

#define BEEP_GPIO   GPIOA
#define BEEP_PIN    Pin_15      // Dời khỏi PA12 để giải phóng chân USB

#define BARO_GPIO   GPIOB
#define BARO_PIN    Pin_2 

#define GYRO
#define ACC
#define MAG
#define BARO
#define GPS
#define BUZZER
#define LED0
#define MOTOR_PWM_RATE 400

#define SENSORS_SET (SENSOR_ACC | SENSOR_BARO | SENSOR_MAG)
#define I2C_DEVICE (I2CDEV_2)   // Dùng I2C2 (PB10, PB11)

// --- 3. GỌI CÁC DRIVER SAU KHI ĐÃ ĐỊNH NGHĨA XONG HẾT TYPE ---
#include "drv_adc.h"
#include "drv_adxl345.h"
#include "drv_bma280.h"
#include "drv_bmp085.h"
#include "drv_bmp280.h"
#include "drv_ms5611.h"
#include "drv_hmc5883l.h"
#include "drv_ak8975.h"
#include "drv_i2c.h"
#include "drv_spi.h"
#include "drv_mma845x.h"
#include "drv_mpu.h"
#include "drv_pwm.h"
#include "drv_timer.h"
#include "drv_serial.h"
#include "drv_uart.h"
#include "drv_softserial.h"

#else
#error TARGET NOT DEFINED!
#endif

// --- 4. CÁC HELPFUL MACROS ---
#ifdef LED0
#define LED0_TOGGLE              digitalToggle(LED0_GPIO, LED0_PIN);
#define LED0_OFF                 digitalHi(LED0_GPIO, LED0_PIN);
#define LED0_ON                  digitalLo(LED0_GPIO, LED0_PIN);
#endif

#ifdef BEEP_GPIO
#define BEEP_TOGGLE              digitalToggle(BEEP_GPIO, BEEP_PIN);
#define BEEP_OFF                 systemBeep(false);
#define BEEP_ON                  systemBeep(true);
#endif
