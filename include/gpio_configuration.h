#ifndef __GPIO_CONF_H
#define __GPIO_CONF_H

#include "pico/stdlib.h"
#include "global.h"
#include "../include/ssh1106.h"

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
        state = debug;   
        SSD1306_Clear();
    }
}

void gpio_init_all(void){
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
}


#endif