#include "aht10.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

void aht10_init(void) {
    // iniciando o canal i2c
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    
    uint8_t cmd[] = {0xBE, 0x08, 0x00};
    i2c_write_blocking(I2C_PORT, AHT10_ADDR, cmd, 3, false);
    sleep_ms(50);
}

void aht10_trigger_measurement(void) {
    uint8_t cmd[] = {0xAC, 0x33, 0x00};
    i2c_write_blocking(I2C_PORT, AHT10_ADDR, cmd, 3, false);
    sleep_ms(80);
}

bool aht10_read_values(float *temperature, float *humidity) {
    // Aguarda até o sensor estar pronto (busy = 0)
    int tentativas = 100;
    while (tentativas-- > 0) {
        uint8_t status;
        int res = i2c_read_blocking(I2C_PORT, AHT10_ADDR, &status, 1, false);
        if (res == 1 && !(status & 0x80)) {
            break;
        }
        sleep_ms(2);
    }
    if (tentativas <= 0) return false; // se esgotou as tentativas

    // Leitura dos dados
    uint8_t data[6];
    int res = i2c_read_blocking(I2C_PORT, AHT10_ADDR, data, 6, false);
    if (res != 6) return false;

    uint32_t hum_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t temp_raw = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    *humidity = ((float)hum_raw / 1048576.0f) * 100.0f;
    *temperature = ((float)temp_raw / 1048576.0f) * 200.0f - 50.0f;

    return true;
}
