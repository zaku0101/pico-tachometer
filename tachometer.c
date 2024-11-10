#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

#include "font.h"
#include "ssh1106.h"

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

    i2c_init(i2c_default, 400 * 1000);

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);
    gpio_pull_up(SENSOR_PIN);

    gpio_set_irq_enabled_with_callback(SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    SSD1306_Init();

    SSD1306_GotoXY (30, 20);
    SSD1306_Puts ("Jebac AIR :)", &Font_7x10, 1);
    SSD1306_UpdateScreen();


    while(true){
        sleep_ms(500); // Adjust the sleep time as needed
    }
}
