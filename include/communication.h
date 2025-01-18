#ifndef __COMMUNICATON_H
#define __COMMUNICATON_H

#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0  // GPIO pin for UART TX
#define UART_RX_PIN 1  // GPIO pin for UART RX (if needed)

void setup_uart() {
    // Initialize the UART with the chosen baud rate
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Configure UART parameters (8N1: 8 data bits, no parity, 1 stop bit)
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);

    // Enable UART FIFO
    uart_set_fifo_enabled(UART_ID, true);
}

// Function to send a string over UART
void send_string(const char *str) {
    uart_puts(UART_ID, str);
}


void dma_adc_capture(uint16_t *adc_dma_data, uint dma_chan, dma_channel_config cfg){

    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_chan, &cfg,
        adc_dma_data,    // dst
        &adc_hw->fifo,  // src
        var_sample_count,  // transfer count
        true            // start immediately
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_chan);

}

#endif