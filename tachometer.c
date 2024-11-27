#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "global.h"

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#include "font.h"
#include "ssh1106.h"

#define ADC_PIN 26
#define OK_BUTTON 12
#define MENU_BUTTON 13
#define SWITCH_BUTTON 20

volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile bool first_edge = true;

void button_callback(uint gpio, uint32_t events) {
    if (gpio == OK_BUTTON && events == GPIO_IRQ_EDGE_FALL) {
        printf("OK button pressed %d\n", gpio);
        state = calibration;
    }else if(gpio == SWITCH_BUTTON && events == GPIO_IRQ_EDGE_FALL){
        printf("Switch button pressed %d\n", gpio);
        state = measure;
    }else if(gpio == MENU_BUTTON && events == GPIO_IRQ_EDGE_FALL){
        printf("Menu button pressed %d\n",gpio);
        state = menu;   
    }
    sleep_ms(100);
}

//could be good start to write rpm calc function
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

    //i2c initialization
    i2c_init(i2c_default, 400 * 1000);

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    //button initialization
    gpio_init(OK_BUTTON);
    gpio_set_dir(OK_BUTTON, GPIO_IN);
    gpio_pull_up(OK_BUTTON);

    gpio_init(SWITCH_BUTTON);
    gpio_set_dir(SWITCH_BUTTON, GPIO_IN);
    gpio_pull_up(SWITCH_BUTTON);
    
    gpio_init(MENU_BUTTON);
    gpio_set_dir(MENU_BUTTON, GPIO_IN);
    gpio_pull_up(MENU_BUTTON);

    //interrupt initialization
    gpio_set_irq_enabled_with_callback(OK_BUTTON,GPIO_IRQ_EDGE_FALL , true, &button_callback);
    gpio_set_irq_enabled_with_callback(SWITCH_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(MENU_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    SSD1306_Init();

    while(true){
        switch (state){
        case init:
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("INIT", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            SSD1306_Clear();
            break;
        case menu:
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("MENU", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case measure:
            tight_loop_contents();
            /*Pomiar i wyswietlanie wyniku na ekranie
            pomiar przez sekunde
            oblicz
            wyswielt
            W razie niepewnych pomiarow, komunikat np. out of range*/
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("MEASURE", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case calibration:
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("CALI", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case debug:
            /*
            Logi po USB
            */
            break;
        
        default:
            break;
        };
    }
}
