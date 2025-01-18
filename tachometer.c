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

uint dma_chan;
uint16_t adc_dma_data[SAMPLE_COUNT];
dma_channel_config cfg ;

void config_all(void);

int main(){
    char info_str[14];  // 5 digits + null terminator
    char display_str[15];
    uint16_t raw = 0;

    /// INIT
    config_all();
    float fft_frequency = 0;
    int16_t fft_revolution = 0;
    unit =0;

    /// MAIN LOOP
    while(true){
        switch (state){
        case init:

            SSD1306_Clear();
            sprintf(display_str, "%05d", 123);

            SSD1306_GotoXY (5, 20);
            SSD1306_Puts (display_str, &Font_16x26, 1);

            SSD1306_GotoXY (88, 26);
            SSD1306_Puts ("RPM", &Font_11x18, 1);
            SSD1306_UpdateScreen();

            sleep_ms(1000);
            SSD1306_Clear();
            sprintf(display_str, "%06.2f", 12.34);

            SSD1306_GotoXY (5, 20);
            SSD1306_Puts (display_str, &Font_16x26, 1);

            SSD1306_GotoXY (105, 26);
            SSD1306_Puts ("Hz", &Font_11x18, 1);
            SSD1306_UpdateScreen();
            sleep_ms(1000);
            state = menu;
            SSD1306_Clear();
            break;
            
        case menu:
            SSD1306_GotoXY (45, 0);
            SSD1306_Puts ("MENU", &Font_11x18, 1);
            SSD1306_GotoXY (10, 40);
            SSD1306_Puts ("Meas", &Font_7x10, 1);
            SSD1306_GotoXY (90, 40);
            SSD1306_Puts ("Debg", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;

        case measure:
            tight_loop_contents();

            if(unit == 0){
                fft_revolution = fft_frequency * 60;
                sprintf(display_str, "%05d", fft_revolution);

                SSD1306_GotoXY (5, 20);
                SSD1306_Puts (display_str, &Font_16x26, 1);

                SSD1306_GotoXY (88, 26);
                SSD1306_Puts ("RPM", &Font_11x18, 1);
            }

            else{
                sprintf(display_str, "%06.2f", fft_frequency);

                SSD1306_GotoXY (5, 20);
                SSD1306_Puts (display_str, &Font_16x26, 1);

                SSD1306_GotoXY (105, 26);
                SSD1306_Puts ("Hz", &Font_11x18, 1);
            }

            SSD1306_GotoXY (5, 50);
            sprintf(info_str, "T= %dsek | Fs= 1kHz", sampling_time);
            SSD1306_Puts (info_str, &Font_7x10, 1);
            SSD1306_UpdateScreen();
            printf("sample count %d", var_sample_count);
            dma_adc_capture(adc_dma_data, dma_chan, cfg);
            fft_frequency = calculate_frequency(adc_dma_data);
            if(fft_frequency < 1){
                fft_frequency = 0.0;
            }
            break;

        case calibration:
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("CALI", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;

        case debug:
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("DEBUG MODE", &Font_16x26, 1);
            SSD1306_UpdateScreen();

            dma_adc_capture(adc_dma_data, dma_chan, cfg);
            fft_frequency = calculate_frequency(adc_dma_data);
            if(fft_frequency < 1){
                fft_frequency = 0.0;
            }

            for (int i = 0; i < var_sample_count; i++) {
                printf("%d\n", adc_dma_data[i]);
            }
            break;
        
        default:
            break;
        };
    }    
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
        false
    );

    adc_set_clkdiv(48000);

    dma_chan = dma_claim_unused_channel(true);
    if (dma_chan == -1) {
        while(1){
            fprintf(stderr, "Failed to claim unused DMA channel\n");
              sleep_ms(1000);
        }
    }
    cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_ADC);
}