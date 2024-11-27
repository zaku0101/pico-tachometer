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
        printf("OK button pressedx %d\n", gpio);
        state = calibration;
        SSD1306_Clear();
    }else if(gpio == SWITCH_BUTTON && events == GPIO_IRQ_EDGE_FALL){
        printf("Switch button pressedx %d\n", gpio);
        state = measure;
        SSD1306_Clear();
    }else if(gpio == MENU_BUTTON && events == GPIO_IRQ_EDGE_FALL){
        printf("Menu button pressedx %d\n",gpio);
        state = menu;   
        SSD1306_Clear();
    }
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
#define SAMPLE_COUNT (1000)
#define SAMPLING_INTERVAL_US 100 
uint16_t adc_buffer[SAMPLE_COUNT]; // Buffer to store ADC data

// Function to calculate frequency
float calculate_frequency(uint16_t *data, size_t count) {
    uint16_t min_value = 4095, max_value = 0;
    int crossings = 0;

    // Find min and max values
    for (size_t i = 0; i < count; i++) {
        if (data[i] < min_value) min_value = data[i];
        if (data[i] > max_value) max_value = data[i];
    }

    // Calculate floating threshold
    float threshold = (min_value + max_value) / 2.0;

    // Count threshold crossings (rising edge)
    for (size_t i = 1; i < count; i++) {
        if (data[i - 1] < threshold && data[i] >= threshold) {
            crossings++;
        }
    }

    // Calculate frequency
    float time_interval = count * SAMPLING_INTERVAL_US / 1e6; // Convert microseconds to seconds
    float frequency = (crossings / 2.0) / time_interval;

    return frequency;
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

    adc_init();

    // Select ADC input 0 (GP26)
    adc_gpio_init(ADC_PIN);  // Initialize GPIO for ADC function
    adc_select_input(0);
    uint16_t adc_data[SAMPLE_COUNT];
            char voltage_str[14];  // 5 digits + null terminator
            char hz_str[15];
    while(true){
        switch (state){
        case init:
            //SSD1306_Clear();
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("INIT", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case menu:
            //SSD1306_Clear();
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("MENU", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case measure:
            //SSD1306_Clear();
            tight_loop_contents();
            /*Pomiar i wyswietlanie wyniku na ekranie
            pomiar przez sekunde
            oblicz
            wyswielt
            W razie niepewnych pomiarow, komunikat np. out of range*/
            int i = 0;
            int N = 10;

            float voltage = 0;

            uint16_t raw = adc_read();
            voltage = raw * 3.3 / (1 << 12);
            
            for (size_t i = 0; i < SAMPLE_COUNT; i++) {
            adc_data[i] = adc_read();
            sleep_us(SAMPLING_INTERVAL_US);
        }

             // Calculate frequency
            float frequency = calculate_frequency(adc_data, SAMPLE_COUNT);

            // Convert raw value to voltage (assuming 3.3V reference)

            // Format the voltage as a 5-character string (e.g., "3.30V")
            sprintf(voltage_str, "U = %4.2f V", voltage);
            sprintf(hz_str, "f = %4.2f Hz", frequency);
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts (voltage_str, &Font_7x10, 1);

            SSD1306_GotoXY (30, 40);
            SSD1306_Puts (hz_str, &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case calibration:
            //SSD1306_Clear();
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
        sleep_ms(10);
    }
}
