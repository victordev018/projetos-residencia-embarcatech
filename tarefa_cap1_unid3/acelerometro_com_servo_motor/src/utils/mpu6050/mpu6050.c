#include "hardware/i2c.h"
#include <math.h>
#include "pico/stdlib.h"
#include "mpu6050.h"

#define MPU6050_ADDR 0x68
#define SDA_PIN 0
#define SCL_PIN 1

void mpu6050_init(void) {
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    uint8_t reset[2] = {0x6B, 0x00}; // Power Management register
    i2c_write_blocking(i2c0, MPU6050_ADDR, reset, 2, false);
}

void mpu6050_read_raw(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];
    i2c_write_blocking(i2c0, MPU6050_ADDR, (uint8_t[]){0x3B}, 1, true); // ACCEL_XOUT_H
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

    *ax = (buffer[0] << 8) | buffer[1];
    *ay = (buffer[2] << 8) | buffer[3];
    *az = (buffer[4] << 8) | buffer[5];
}

float mpu6050_get_inclination(int16_t ax, int16_t ay, int16_t az) {
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    return atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * (180.0 / M_PI);
}