#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xMutex;   // Mutex garante a exclusividade no acesso a um recurso compartilhado
int sharedCounter = 0;      // Área de memória que será compartilhada entre as duas tarefas

// task 1
void task1(void *vParamters) {
    
    for(;;) {

        // protege a região crítica (região de memória compartilhada)
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            sharedCounter++;
            printf("Task 1: Counter: %d\n", sharedCounter);
            xSemaphoreGive(xMutex);     // libera o semáforo
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// task 2
void task2(void *vParamters) {

    for(;;) {

        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            sharedCounter++;
            printf("Task 2: Counter: %d\n", sharedCounter);
            xSemaphoreGive(xMutex);     // libera o semáforo
        }
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}

int main()
{
    stdio_init_all();
    xMutex = xSemaphoreCreateMutex();

    if (xMutex != NULL) {
        xTaskCreate(task1, "Task 1", 256, NULL, 1, NULL);
        xTaskCreate(task2, "Task 2", 256, NULL, 1, NULL);
        vTaskStartScheduler();
    }

    while (true);
}
