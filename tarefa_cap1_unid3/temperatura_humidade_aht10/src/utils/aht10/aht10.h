#ifndef AHT10_H
#define AHT10_H

#include "hardware/i2c.h"

// Definições
#define AHT10_ADDR 0x38
#define I2C_PORT i2c0
#define SDA_PIN 0
#define SCL_PIN 1

// definição das funções
void aht10_init(void);
void aht10_trigger_measurement(void);
bool aht10_read_values(float *temperature, float *humidity);

#endif
