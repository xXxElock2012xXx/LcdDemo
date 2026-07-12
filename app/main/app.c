#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Change this to the GPIO where your LED is connected
#define LED_GPIO GPIO_NUM_2

int delay = 500;
int delay_intervals[] = {500,400,300,200,100,600,700,800,900,1000};


void blink_task(void *pvParameter)
{
  while (1) {
    gpio_set_level(LED_GPIO, 1); // LED ON
    vTaskDelay(pdMS_TO_TICKS(delay));

    gpio_set_level(LED_GPIO, 0); // LED OFF
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}

void delay_thread(void *pvParameter) {
  static int i = 0;
  while (1) {

    delay = delay_intervals[i];
    i++;
    i%=10;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void) {
  // Configure the LED GPIO
  gpio_config_t io_conf = {.pin_bit_mask = (1ULL << LED_GPIO),
                           .mode = GPIO_MODE_OUTPUT,
                           .pull_up_en = GPIO_PULLUP_DISABLE,
                           .pull_down_en = GPIO_PULLDOWN_DISABLE,
                           .intr_type = GPIO_INTR_DISABLE};

  gpio_config(&io_conf);

  // Create the blink task
  xTaskCreatePinnedToCore(blink_task,   // Task function
                          "blink_task", // Task name
                          1024,         // Stack size (bytes)
                          NULL,         // Parameters
                          5,            // Priority
                          NULL,         // Task handle
                          0);
  xTaskCreatePinnedToCore(delay_thread,   // Task function
                          "delay_task", // Task name
                          1024,         // Stack size (bytes)
                          NULL,         // Parameters
                          5,            // Priority
                          NULL,         // Task handle
                          0);
}