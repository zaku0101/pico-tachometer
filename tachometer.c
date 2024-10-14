#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define SENSOR_PIN 14

volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile bool first_edge = true;

void gpio_callback(uint gpio, uint32_t events) {
    if (first_edge) {
        start_time = time_us_32();
        first_edge = false;
    } else {
        end_time = time_us_32();
        first_edge = true;
        uint32_t time_diff = end_time - start_time;
        if (time_diff > 0) {
            float rpm = 60.0 / ((float)time_diff / 1000000.0);
            printf("RPM: %.2f\n", rpm);
        }
    }
}

int main(){
    stdio_init_all();

    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);
    gpio_pull_up(SENSOR_PIN);

    gpio_set_irq_enabled_with_callback(SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while(true){
        sleep_ms(500); // Adjust the sleep time as needed
    }
}
