// inclusão das bibliotecas padrões do C e do SKD do Raspberry Pi Pico
#include <stdio.h>
#include "pico/stdlib.h"

// inclusão de arquivos utilitários
#include "aht10.h"
#include "display.h"

// variáveis para armazenar os valores de temperatura e pressão
float temp, hum;

// Buffers para mensagens no display
char temperature_msg[30];
char humidity_msg[30];
char temp_alert_msg[30];
char hum_alert_msg[30];

// função para exibir os dados de temperatura e umidade no display com as mensagens de alerta
void show_data_on_display(float temp, float hum) {
    // limpa o display
    display_clear();

    // preparando mensagem e exibindo (valores)
    snprintf(temperature_msg, sizeof(temperature_msg), "temp: %.2f C", temp);
    snprintf(humidity_msg, sizeof(humidity_msg), "umid: %.1f %%", hum);
    display_write(temperature_msg, 0, 0, 1);
    display_write(humidity_msg, 0, 13, 1);

    // preparando mensagem (alerta)
    snprintf(
    temp_alert_msg,
    sizeof(temp_alert_msg),
    temp < 20.0f ? "temp: Muito baixa!" : "temp: Estavel"
    );

    snprintf(
        hum_alert_msg,
        sizeof(hum_alert_msg),
        hum > 70.0f ? "umid: Alta!" : "umid: Estavel"
    );

    display_write(temp_alert_msg, 0, 35, 1);
    display_write(hum_alert_msg, 0, 50, 1);

    // exibe o conteudo no display
    display_show();
}

// função responsável por iniciar os dispositivos
void setup() {

    // incializa a comunicação serial
    stdio_init_all();

    // inicia o sensor de AHT10
    aht10_init();

    // inicia o display
    display_init();
}

int main() {
    
    // iniciando os dispositivos
    setup();
    sleep_ms(100);      // aguarda 100ms

    while (true) {

        // prepara para iniciar uma medição
        aht10_trigger_measurement();

        // se a leitura for realizada com sucesso
        if (aht10_read_values(&temp, &hum)) {
            // exibe os valores no display (com os possiveis alertas)
            show_data_on_display(temp, hum);
        } else {
            printf("Failed to read AHT10\n");
        }

        // delay de 2.2s
        sleep_ms(2250);
    }
}
