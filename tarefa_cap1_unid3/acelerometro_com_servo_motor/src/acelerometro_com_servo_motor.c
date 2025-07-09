// inclusão de bibliotecas padrões do C e do SDK do Raspberry Pi Pico
#include <stdio.h>
#include "pico/stdlib.h"

// inclusão de arquivos utilitarios
#include "mpu6050.h"
#include "servo.h"
#include "display.h"

// variáveis para armazenar os valores dos eixos x, y e z
int16_t ax, ay, az;
float inclinacao;

// variáveis para exibir feedbacks no display
char inclinacao_msg[30];
char alert_msg[40];

// função para exibir o dado de inclinação e os alertas no display
void show_data_on_display(float inclinacao) {

    // limpa conteúdo do display
    display_clear();

    // escreve informações no display
    snprintf(inclinacao_msg, sizeof(inclinacao_msg), "inclinacao: %.1f °", inclinacao);
    display_write(inclinacao_msg, 0, 0, 1);

    // gerando alerta baseado no valor da inclinação
    if (inclinacao > 30.0f) {
        snprintf(alert_msg, sizeof(alert_msg), "- ALTA INCLINACAO!");
    } else if (inclinacao >= 15.0f) {
        snprintf(alert_msg, sizeof(alert_msg), "- ATENCAO");
    } else {
        snprintf(alert_msg, sizeof(alert_msg), "- ESTAVEL");
    }
    display_write("STATUS:", 0, 15, 1);
    display_write(alert_msg, 0, 30, 1);

    // exibe conteúdo no display
    display_show();
}

// função responsável por inicialiazar os dispositivos
void setup() {

    // inicializa a comunicação serial
    stdio_init_all();

    // inicializa o acelerometro mpu-6050
    mpu6050_init();

    // inicializa o servo motor SG90
    servo_init();

    // incicializa o display
    display_init();
}

int main() {

    // inicializa os dispositos
    setup();
    sleep_ms(100);      // aguarda 100ms

    while(true) {

        // realiza leitura dos eixos
        mpu6050_read_raw(&ax, &ay, &az);

        // calcula a inclinação
        inclinacao = mpu6050_get_inclination(ax, ay, az);

        // converte a inclinaçao do acelerometro em um ângulo (0-180) para o servo motor
        uint angle = (uint)(inclinacao + 90);

        // Garante que o ângulo fica dentro dos limites do servo
        if (angle > 180) angle = 180;
        if (angle < 0) angle = 0;

        // move o servo motor com o angulo calculado
        servo_set_angle(angle);

        // exibindo valores no display (com alertas)
        show_data_on_display(inclinacao);

        // delay de 0.5s
        sleep_ms(500);
    }

    return 0;
}