#include "board.h" // Đảm bảo file board.h chứa các định nghĩa cần thiết

// Khai báo thủ công các hàm để trình biên dịch không báo lỗi
extern void drv_system_init(void);
extern void drv_spi_init(void);
extern void drv_pwm_init(void);
extern void drv_pwm_write_motor(void);
extern int mpu6500_read_gyro(void);

int main(void) {
    // 1. Hệ thống
    SystemInit();
    drv_system_init();
    
    // 2. Cảm biến & Động cơ
    drv_spi_init();     // Khởi tạo SPI cho MPU6500
    drv_pwm_init();     // Khởi tạo PWM cho ESC
    
    // 3. Vòng lặp chính
    while (1) {
        if (mpu6500_read_gyro()) {
            // Tự viết hàm tính PID tại đây
            // Xuất tín hiệu ra ESC
            drv_pwm_write_motor(); 
        }
    }
}

