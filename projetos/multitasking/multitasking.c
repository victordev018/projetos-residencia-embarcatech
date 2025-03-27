#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// constantes dos pinos do led
const uint pin_led_blue = 11;
const uint pin_led_red = 12;

// incializando leds
void setup() {
    gpio_init(pin_led_blue);
    gpio_set_dir(pin_led_blue, GPIO_OUT);
    gpio_init(pin_led_red);
    gpio_set_dir(pin_led_red, GPIO_OUT);
}

// tarefa que será executada
void vBlinkTask(void *pvParamters) {

    uint *led = (uint *)pvParamters;
    for(;;) {
        gpio_put(*led, 1);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        gpio_put(*led, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

int main()
{
    stdio_init_all();
    setup();

    xTaskCreate(vBlinkTask, "Blink Task", 128, (void *)&pin_led_blue, 1, NULL);
    xTaskCreate(vBlinkTask, "Blink Task 2", 128, (void *)&pin_led_red, 1, NULL);
    vTaskStartScheduler();
    

    // nunca chegará aqui
    while (true) {
    }
}
