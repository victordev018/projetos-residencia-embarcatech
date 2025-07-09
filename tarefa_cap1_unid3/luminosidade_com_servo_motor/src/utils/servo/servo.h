#ifndef SERVOR_H
#define SERVOR_H

#include <stdio.h>              // Para a função printf
#include "hardware/pwm.h"       // Para todas as funções pwm_*
#include "hardware/clocks.h"    // Para clock_get_hz e clk_sys
#include "hardware/gpio.h"      // Para gpio_set_function e GPIO_FUNC_PWM

// --- Constantes para o controle do Servo ---
// Frequência do sinal PWM para servos (geralmente 50Hz, ou um período de 20ms)
#define PWM_PERIOD_MS 20

// Define o pino GPIO que está conectado ao fio de sinal do servo
// como está conectado ao I2C 0 e o seu canal SDA está conectado ao pino 0
#define SERVO_PIN 28

// Largura do pulso em microssegundos (us) para o SG90
#define SERVO_MIN_PULSE_US 500 // Pulso para o ângulo 0 no nosso código
#define SERVO_MAX_PULSE_US 2500 // Pulso para o ângulo 180 no nosso código

// Function prototypes
void servo_init();
void servo_set_angle(uint angle);

#endif // SERVOR_H