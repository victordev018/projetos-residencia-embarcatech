#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"    // biblioteca para gerenciar o módulo wifi (CYW4339) integrado no raspberry pi pico w
#include "hardware/vreg.h"      // biblioteca para controlar o regulador de voltage (Voltage Regulator) na Rasp Pi Pico
#include "hardware/clocks.h"    // biblioteca para controlar os clocks internos da Rasp Pi Pico, como frequêmcia da cpu e periféricos

// pingagem do led vermelho
const uint led_pin_red = 13;


// Callback para resultados das varreduras. Essa função será chamada automaticamente sempre que o resultado
// de varredura Wi-Fi for encontrada
static int scan_result(void *env, const cyw43_ev_scan_result_t *result) {

    // verifica se há resultado válido, ou seja, se result != NULL
    if(result) {
        // imprime as informações do resultado (SSID, RSSI, canal, Endereço MAC etc...)
        printf("SSID: %-32s RSSI: %4d CHAN: %3d MAC: %02x:%02x:%02x:%02x:%02x:%02x SEC: %u\n",
        result->ssid, result->rssi, result->channel, result->bssid[0], result->bssid[1], result->bssid[2],
        result->bssid[3], result->bssid[4], result->bssid[5],result->auth_mode
        );
    }
    return 0; // Retorna 0 para continuar a varredura
}

// Função para piscar o led em um dado tempo em ms
void led_blink(const uint time_ms) {
    gpio_put(led_pin_red, true); // Acende o LED
    sleep_ms(time_ms);
    gpio_put(led_pin_red, false); // Apaga o LED
    sleep_ms(time_ms);
}

// função para fazer o setup inicial (configurar dispositivos e entradas/saidas padrão)
void setup() {

    // Inicializa a entrada/saída padrão
    stdio_init_all();

    // inicia e configura o led vermelho como um dispositivo de saida
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, 1);
}

// função principal
int main() {

    setup();                // inicia comunicação serial e inicializa o led
    sleep_ms(2000);         // aguarda 2s para (útil para estabilizar dispositovos)
    printf("Iniciando...\n");

    // iniciando hardware WIFI
    if (cyw43_arch_init()) {
        printf("Falha ao iniciar o Wifi\n");
        return 1;
    }

    printf("Wifi iniciado com sucesso!\n");
    cyw43_arch_enable_sta_mode(); // Habilita o modo estação (client) para a Pico

    // Configura tempo inicial para 0 (varredura imediata)
    absolute_time_t scan_time = make_timeout_time_ms(0);
    bool scan_in_progress = false;      // variável de controle para indicar se há uma varredura em andamento

    while (true) {

        // Controla o LED vermelho (pisca em intervalos de 200 ms)
        led_blink(200);

        // Verificando se o tempo para iniciar a nova varredura já começou
        if (absolute_time_diff_us(get_absolute_time(), scan_time) < 0) {

            // se nenhuma varredura estiver em andamento
            if (!scan_in_progress) {

                // Configura opções padrão para varredura
                cyw43_wifi_scan_options_t scan_options = {0};

                // inicia uma nova varredura
                int err = cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, scan_result);

                // verifica se a varredura foi iniciada com sucesso
                if (err == 0) {
                    printf("Iniciando varredura WIFI...\n");
                    scan_in_progress = true;
                }
                else {  // caso haja erro ao inicar a varredura
                    printf("Falha ao inicar varredura!\n");
                    // agenda nova tentativa em 10s
                    scan_time = make_timeout_time_ms(10000);
                }

            } else if(cyw43_wifi_scan_active(&cyw43_state)) {  // Verifica se a varredura foi concluída
                printf("Varredura concluida\n");
                // agenda nova varredura em 10s
                scan_time = make_timeout_time_ms(10000);
                scan_in_progress = false;       // atualiza a variável para indicar que não há nenhuma varredura em andamento
            }
        }
        #if PICO_CYW43_ARCH_POLL // Verifica se o modo de polling está habilitado
                cyw43_arch_poll(); // Executa a verificação manual de eventos
                cyw43_arch_wait_for_work_until(scan_time); // Aguarda até o próximo evento ou o tempo configurado
        #else
                sleep_ms(1000); // Aguarda 1 segundo antes de continuar (caso o modo de polling não esteja ativo)
        #endif
    }

    cyw43_arch_deinit(); // Desliga e limpa os recursos do hardware Wi-Fi
    return 0; // Retorna 0 para indicar que o programa foi finalizado sem erros
}