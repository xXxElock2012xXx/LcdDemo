#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Change this to the GPIO where your LED is connected
#define LED_GPIO GPIO_NUM_2

void blink_task(void *pvParameter)
{
    while (1)
    {
        gpio_set_level(LED_GPIO, 1);   // LED ON
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_GPIO, 0);   // LED OFF
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    // Configure the LED GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    // Create the blink task
    xTaskCreate(
        blink_task,        // Task function
        "blink_task",      // Task name
        2048,              // Stack size (bytes)
        NULL,              // Parameters
        5,                 // Priority
        NULL               // Task handle
    );
}