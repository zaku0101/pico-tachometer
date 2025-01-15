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
uint dma_chan;
uint8_t adc_dma_data[SAMPLE_COUNT];
dma_channel_config cfg ;
void dma_adc_capture(uint8_t *adc_dma_data);
void debug_dma_adc_capture(uint8_t *adc_dma_data);
void config_all(void);

int main(){
    /// LOCAL SHIT
    uint16_t adc_data[SAMPLE_COUNT];
    

    char voltage_str[14];  // 5 digits + null terminator
    char hz_str[15];
    char message[24];
    uint16_t raw = 0;
    /// INIT
    config_all();

    printf("Zyje!!!\n");
    float fft_frequency = 0;
    int16_t fft_revolution = 0;
    /// MAIN LOOP
    while(true){
        switch (state){
        case init:
            SSD1306_Clear();
            sprintf(hz_str, "%05d", 123);

            SSD1306_GotoXY (5, 20);
            SSD1306_Puts (hz_str, &Font_16x26, 1);

            SSD1306_GotoXY (88, 26);
            SSD1306_Puts ("RPM", &Font_11x18, 1);
            SSD1306_UpdateScreen();

            sleep_ms(1000);
            SSD1306_Clear();
            sprintf(hz_str, "%06.2f", 12.34);

            SSD1306_GotoXY (5, 20);
            SSD1306_Puts (hz_str, &Font_16x26, 1);

            SSD1306_GotoXY (105, 26);
            SSD1306_Puts ("Hz", &Font_11x18, 1);
            SSD1306_UpdateScreen();
            sleep_ms(1000);
            state = menu;
            SSD1306_Clear();
            break;
        case menu:
            //SSD1306_Clear();
            SSD1306_GotoXY (30, 0);
            SSD1306_Puts ("MENU", &Font_11x18, 1);
            SSD1306_GotoXY (0, 40);
            SSD1306_Puts ("Measure", &Font_7x10, 1);
            SSD1306_GotoXY (40, 40);
            SSD1306_Puts ("Debug", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case measure:
            tight_loop_contents();

            // dma_adc_capture(adc_dma_data);
            // fft_frequency = fft_interpolation_process(adc_dma_data);
            // if(fft_frequency < 1){
            //     fft_frequency = 0.0;
            // }

            printf("Frequency: %f\n", fft_frequency);

            if(unit == 0){
                fft_revolution = fft_frequency * 60;
                sprintf(hz_str, "%05d", fft_revolution);

                SSD1306_GotoXY (5, 20);
                SSD1306_Puts (hz_str, &Font_16x26, 1);

                SSD1306_GotoXY (88, 26);
                SSD1306_Puts ("RPM", &Font_11x18, 1);
            }

            else{
                sprintf(hz_str, "%06.2f", fft_frequency);

                SSD1306_GotoXY (5, 20);
                SSD1306_Puts (hz_str, &Font_16x26, 1);

                SSD1306_GotoXY (105, 26);
                SSD1306_Puts ("Hz", &Font_11x18, 1);
            }

            SSD1306_GotoXY (5, 50);
            sprintf(voltage_str, "T= 2sek | Fs= 1kHz");
            SSD1306_Puts (voltage_str, &Font_7x10, 1);
            SSD1306_UpdateScreen();

            dma_adc_capture(adc_dma_data);
            fft_frequency = calculate_frequency(adc_dma_data);
            if(fft_frequency < 1){
                fft_frequency = 0.0;
            }
            
            break;
        case calibration:
            //SSD1306_Clear();
            SSD1306_GotoXY (30, 20);
            SSD1306_Puts ("CALI", &Font_7x10, 1);
            SSD1306_UpdateScreen();
            break;
        case debug:

            while(state == debug){
                raw = adc_read();
                printf("%d\n\r",raw);
                
            }

            break;
        
        default:
            break;
        };
        //sleep_ms(1);
    }    
}

void dma_adc_capture(uint8_t *adc_dma_data) {

    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_chan, &cfg,
        adc_dma_data,    // dst
        &adc_hw->fifo,  // src
        SAMPLE_COUNT,  // transfer count
        true            // start immediately
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_chan);

}

void debug_dma_adc_capture(uint8_t *adc_dma_data) {

    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_chan, &cfg,
        &uart_get_hw(UART_ID)->dr,  // dst
        &adc_hw->fifo,  // src
        SAMPLE_COUNT,  // transfer count
        true            // start immediately
    );


    while (true) {
        adc_run(true);
        dma_channel_wait_for_finish_blocking(dma_chan);

        // for (int i = 0; i < SAMPLE_COUNT; i++) {
        //     printf("%d\n", adc_dma_data[i]);
        // }
        adc_run(false);

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
        true
    );

    adc_set_clkdiv(48000);
    dma_chan = dma_claim_unused_channel(true);

    dma_chan = dma_claim_unused_channel(true);
    if (dma_chan == -1) {
        while(1){
            fprintf(stderr, "Failed to claim unused DMA channel\n");
              sleep_ms(1000);
        }
    }
    cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    channel_config_set_dreq(&cfg, DREQ_ADC);

}