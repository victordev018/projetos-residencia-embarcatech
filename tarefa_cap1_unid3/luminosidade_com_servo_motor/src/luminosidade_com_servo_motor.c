// inclusão de bibliotecas padrões do C e do SDK da Raspberry Pi Pico
#include <stdio.h>
#include "pico/stdlib.h"

// inclusão de arquivos utilitarios
#include "servo.h"
#include "bh1750.h"

// instância i2c que será usada para ler os dados do sensor de luminosidade
i2c_inst_t *i2c = NULL;

// função responsável por iniciar os dispositivos
void setup() {

    // inicializa a comunicação serial
    stdio_init_all();

    // inicializa o servo motor
    servo_init();

    // inicializa o sensor de luminosidade
    // Usa a instância 'i2c0' retornada pela função de inicialização
    i2c = bh1750_init(i2c0, 0, 1); 
}

int main()
{
    
    // inicializando os dispositivos
    setup();
    sleep_ms(1500);     // aguarda 1.5s

    while (true) {

        // Variáveis para armazenar o valor lido
        float lux_value;

        // Leitura do sensor BH1750 e armazenamento do valor lido
        bh1750_read_lux(i2c, &lux_value);

        // converte o valor do sensor de luminosidade para um angulo (0 -180) para o servo motor
        int angle = lux_value / 27;
        servo_set_angle(angle);

        // exibe os valores no serial monitor
        printf("Luminosidade: %.1f\n", lux_value);
        printf("Angulo motor: %d\n", angle);

        // Pausa de 100ms
        sleep_ms(300); 
    }
}
