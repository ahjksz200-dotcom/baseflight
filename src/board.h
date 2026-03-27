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

#define U_ID_0 (*(uint32_t*)0x1FFFF7E8)
#define U_ID_1 (*(uint32_t*)0x1FFFF7EC)
#define U_ID_2 (*(uint32_t*)0x1FFFF7F0)

typedef enum HardwareRevision {
    NAZE32 = 1,
    NAZE32_REV5,
    NAZE32_SP,
    NAZE32_REV6,
} HardwareRevision;

typedef enum {
    SENSOR_GYRO = 1 << 0,
    SENSOR_ACC = 1 << 1,
    SENSOR_BARO = 1 << 2,
    SENSOR_MAG = 1 << 3,
    SENSOR_SONAR = 1 << 4,
    SENSOR_GPS = 1 << 5,
    SENSOR_GPSMAG = 1 << 6,
} AvailableSensors;

typedef enum AccelSensors {
    ACC_DEFAULT = 0,
    ACC_ADXL345 = 1,
    ACC_MPU6050 = 2,
    ACC_MMA8452 = 3,
    ACC_BMA280 = 4,
    ACC_MPU6500 = 5,
    ACC_NONE = 6
} AccelSensors;

typedef enum CompassSensors {
    MAG_DEFAULT = 0,
    MAG_HMC5883L = 1,
    MAG_AK8975 = 2,
    MAG_NONE = 3
} CompassSensors;

typedef enum {
    FEATURE_PPM = 1 << 0,
    FEATURE_VBAT = 1 << 1,
    FEATURE_SERIALRX = 1 << 3,
    FEATURE_MOTOR_STOP = 1 << 4,
    FEATURE_GPS = 1 << 8,
    FEATURE_FAILSAFE = 1 << 9,
    FEATURE_TELEMETRY = 1 << 11,
} AvailableFeatures;

// --- PINOUT DEFINITION FOR BLUE PILL ---
#if defined(NAZE)

#define LED0_GPIO   GPIOC
#define LED0_PIN    Pin_13      // LED Blue Pill nằm ở PC13
// Vô hiệu hóa LED1 vì Blue Pill chỉ có 1 LED chính
#define LED1_GPIO   GPIOC 
#define LED1_PIN    Pin_13 

// Giải phóng PA12 (USB) - Dời Beeper sang PA15
#define BEEP_GPIO   GPIOA
#define BEEP_PIN    Pin_15 

// Giải phóng PC13 khỏi chức năng BARO cũ của Naze
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

// Sử dụng I2C2 (PB10, PB11) cho MPU6500/6050
#define I2C_DEVICE (I2CDEV_2) 

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

// Macros điều khiển LED (Blue Pill LED là Active Low - Kéo xuống GND là sáng)
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
