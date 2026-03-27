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

// --- 1. ENUMS & TYPEDEFS (GIỮ NGUYÊN ĐỂ KHÔNG LỖI BIÊN DỊCH) ---

typedef enum HardwareRevision {
    NAZE32 = 1,
    NAZE32_REV5,
    NAZE32_SP,
    NAZE32_REV6,
} HardwareRevision;

typedef enum {
    X = 0, Y, Z
} sensor_axis_e;

typedef enum {
    ALIGN_DEFAULT = 0,
    CW0_DEG, CW90_DEG, CW180_DEG, CW270_DEG,
    CW0_DEG_FLIP, CW90_DEG_FLIP, CW180_DEG_FLIP, CW270_DEG_FLIP
} sensor_align_e;

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
    ACC_DEFAULT = 0, ACC_ADXL345, ACC_MPU6050, ACC_MMA8452, ACC_BMA280, ACC_MPU6500, ACC_NONE
} AccelSensors;

typedef enum CompassSensors {
    MAG_DEFAULT = 0, MAG_HMC5883L, MAG_AK8975, MAG_NONE
} CompassSensors;

typedef enum {
    FEATURE_PPM = 1 << 0,
    FEATURE_VBAT = 1 << 1,
    FEATURE_INFLIGHT_ACC_CAL = 1 << 2,
    FEATURE_SERIALRX = 1 << 3,
    FEATURE_MOTOR_STOP = 1 << 4,
    FEATURE_SERVO_TILT = 1 << 5,
    FEATURE_SOFTSERIAL = 1 << 6,
    FEATURE_LED_RING = 1 << 7,
    FEATURE_GPS = 1 << 8,
    FEATURE_FAILSAFE = 1 << 9,
    FEATURE_SONAR = 1 << 10,
    FEATURE_TELEMETRY = 1 << 11,
    FEATURE_POWERMETER = 1 << 12,
    FEATURE_VARIO = 1 << 13,
    FEATURE_3D = 1 << 14,
    FEATURE_FW_FAILSAFE_RTH = 1 << 15,
    FEATURE_SYNCPWM = 1 << 16,
    FEATURE_FASTPWM = 1 << 17,
} AvailableFeatures;

typedef enum {
    SERIALRX_SPEKTRUM1024 = 0, SERIALRX_SPEKTRUM2048, SERIALRX_SBUS, SERIALRX_SUMD, SERIALRX_MSP, SERIALRX_IBUS, SERIALRX_PROVIDER_MAX = SERIALRX_IBUS
} SerialRXType;

typedef enum {
    GPS_NMEA = 0, GPS_UBLOX, GPS_MTK_NMEA, GPS_MTK_BINARY, GPS_MAG_BINARY, GPS_HARDWARE_MAX = GPS_MAG_BINARY
} GPSHardware;

typedef enum {
    GPS_BAUD_115200 = 0, GPS_BAUD_57600, GPS_BAUD_38400, GPS_BAUD_19200, GPS_BAUD_9600, GPS_BAUD_MAX = GPS_BAUD_9600
} GPSBaudRates;

typedef void (*sensorInitFuncPtr)(sensor_align_e align);
typedef void (*sensorReadFuncPtr)(int16_t *data);
typedef void (*baroOpFuncPtr)(void);
typedef void (*baroCalculateFuncPtr)(int32_t *pressure, int32_t *temperature);
typedef void (*serialReceiveCallbackPtr)(uint16_t data);
typedef uint16_t (*rcReadRawDataPtr)(uint8_t chan);
typedef void (*pidControllerFuncPtr)(void);

typedef struct sensor_t {
    sensorInitFuncPtr init;
    sensorReadFuncPtr read;
    sensorReadFuncPtr temperature;
    float scale;
} sensor_t;

typedef struct baro_t {
    uint16_t ut_delay;
    uint16_t up_delay;
    baroOpFuncPtr start_ut;
    baroOpFuncPtr get_ut;
    baroOpFuncPtr start_up;
    baroOpFuncPtr get_up;
    baroCalculateFuncPtr calculate;
} baro_t;

// --- 2. PINOUT CHO BLUE PILL (THAY ĐỔI TẠI ĐÂY) ---
#if defined(NAZE)

#define LED0_GPIO   GPIOC
#define LED0_PIN    Pin_13      // LED Blue Pill (PC13)
#define LED1_GPIO   GPIOC 
#define LED1_PIN    Pin_13      // Dùng chung PC13 để fix lỗi Driver

#define BEEP_GPIO   GPIOA
#define BEEP_PIN    Pin_15      // Dời khỏi PA12 để giải phóng chân USB

#define BARO_GPIO   GPIOB
#define BARO_PIN    Pin_2       // Tránh PC13

#define GYRO
#define ACC
#define MAG
#define BARO
#define GPS
#define LEDRING
#define SONAR
#define BUZZER
#define LED0
#define LED1
#define MOTOR_PWM_RATE 400

#define SENSORS_SET (SENSOR_ACC | SENSOR_BARO | SENSOR_MAG)
#define I2C_DEVICE (I2CDEV_2)   // Dùng I2C2 (PB10, PB11) cho MPU6500

// --- 3. INCLUDES (PHẢI Ở DƯỚI TYPEDEF) ---
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
#include "drv_ledring.h"
#include "drv_mma845x.h"
#include "drv_mpu.h"
#include "drv_l3g4200d.h"
#include "drv_pwm.h"
#include "drv_timer.h"
#include "drv_serial.h"
#include "drv_uart.h"
#include "drv_softserial.h"
#include "drv_hcsr04.h"

#else
#error TARGET NOT DEFINED!
#endif

// --- 4. MACROS ---
#ifdef LED0
#define LED0_TOGGLE              digitalToggle(LED0_GPIO, LED0_PIN);
#define LED0_OFF                 digitalHi(LED0_GPIO, LED0_PIN);
#define LED0_ON                  digitalLo(LED0_GPIO, LED0_PIN);
#endif
#ifdef LED1
#define LED1_TOGGLE              digitalToggle(LED1_GPIO, LED1_PIN);
#define LED1_OFF                 digitalHi(LED1_GPIO, LED1_PIN);
#define LED1_ON                  digitalLo(LED1_GPIO, LED1_PIN);
#endif
#ifdef BEEP_GPIO
#define BEEP_TOGGLE              digitalToggle(BEEP_GPIO, BEEP_PIN);
#define BEEP_OFF                 systemBeep(false);
#define BEEP_ON                  systemBeep(true);
#endif
