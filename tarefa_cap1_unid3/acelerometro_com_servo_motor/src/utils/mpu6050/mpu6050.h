#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

void mpu6050_init(void);
void mpu6050_read_raw(int16_t *ax, int16_t *ay, int16_t *az);
float mpu6050_get_inclination(int16_t ax, int16_t ay, int16_t az);

#endif