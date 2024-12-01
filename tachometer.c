#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "include/global.h"
#include "include/calculation.h"
#include "include/communication.h"
#include "include/gpio_configuration.h"

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "hardware/dma.h"
#include "hardware/regs/dma.h"

#include "include/font.h"
#include "include/ssh1106.h"


volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile bool first_edge = true;


uint16_t adc_buffer[SAMPLE_COUNT]; // Buffer to store ADC data

int main(){
    /// LOCAL SHIT
    uint16_t adc_data[SAMPLE_COUNT];
    char voltage_str[14];  // 5 digits + null terminator
    char hz_str[15];
    char message[24];
    uint16_t raw = 0;
    /// INIT
    stdio_init_all();
    i2c_init(i2c_default, 400 * 1000);
    gpio_init_all();
    SSD1306_Init();
    adc_init();   
    setup_uart();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
    /// MAIN LOOP
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
            tight_loop_contents();

            int i = 0;
            int N = 10;

            float voltage = 0;

            raw = adc_read();
            voltage = raw * 3.3 / (1 << 12);
            
            for (i; i < SAMPLE_COUNT; i++) {
                adc_data[i] = adc_read();
                sleep_us(SAMPLING_INTERVAL_US);
            }
             // Calculate frequency
            float frequency = calculate_frequency(adc_data, SAMPLE_COUNT,50);

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
            raw = adc_read();
            sprintf(message,"%d\n\r\0",raw);
            //send_string(message);
            //printf("Done\n");
            printf("%d\n\r",raw);
            //dma_uart_send(message,10);
            sleep_us(SAMPLING_INTERVAL_US);
            //sleep_ms(10);
            break;
        
        default:
            break;
        };
        //sleep_ms(1);
    }
}
