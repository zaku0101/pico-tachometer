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
#include "include/sine_wave.h"

volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile bool first_edge = true;


uint16_t adc_buffer[SAMPLE_COUNT]; // Buffer to store ADC data

void dma_adc_capture(uint8_t *adc_dma_data);
void config_all(void);

int main(){
    /// LOCAL SHIT
    uint16_t adc_data[SAMPLE_COUNT];
    
    uint8_t adc_dma_data[SAMPLE_COUNT];

    char voltage_str[14];  // 5 digits + null terminator
    char hz_str[15];
    char message[24];
    uint16_t raw = 0;
    /// INIT
    config_all();

    printf("Zyje!!!\n");
    float fft_frequency = 0;
    /// MAIN LOOP
    while(true){
        switch (state){
        case init:
            //SSD1306_Clear();
            while (1)
            {
                fft_frequency = fft_interpolation_process(sine_wave);
                printf("F: %f\n", fft_frequency);
                sleep_ms(1000);
            }
            
            // SSD1306_GotoXY (30, 20);
            // SSD1306_Puts ("INIT", &Font_7x10, 1);
            // SSD1306_UpdateScreen();
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

            // raw = adc_read();
            // voltage = raw * 3.3 / (1 << 12);
            
            // for (i; i < SAMPLE_COUNT; i++) {
            //     adc_data[i] = adc_read();
            //     sleep_us(SAMPLING_INTERVAL_US);
            // }
             // Calculate frequency
            //float frequency = calculate_frequency(adc_data, SAMPLE_COUNT,50);
            dma_adc_capture(adc_dma_data);
            printf("Jedna z probek: %d\n", sine_wave[3]);
            fft_frequency = fft_interpolation_process(sine_wave);
            printf("Frequency: %f\n", fft_frequency);

            // Convert raw value to voltage (assuming 3.3V reference)

            // Format the voltage as a 5-character string (e.g., "3.30V")
            sprintf(voltage_str, "U = %4.2f V", voltage);
            sprintf(hz_str, "f = %4.2f Hz", fft_frequency);
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

void dma_adc_capture(uint8_t *adc_dma_data) {
    adc_set_clkdiv(0);
    uint dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    channel_config_set_dreq(&cfg, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
        adc_dma_data,    // dst
        &adc_hw->fifo,  // src
        SAMPLE_COUNT,  // transfer count
        true            // start immediately
    );

    adc_run(true);

    dma_channel_wait_for_finish_blocking(dma_chan);
    adc_run(false);
    adc_fifo_drain();
}

void config_all(void){
    stdio_init_all();
    setup_uart();
    printf("UART INITIALIZED\n");
    i2c_init(i2c_default, 400 * 1000);
    gpio_init_all();
    SSD1306_Init();
    adc_init();   
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
    
    
    adc_fifo_setup(
        true,
        true,
        1,
        false,
        true
    );

}