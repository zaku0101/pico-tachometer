#ifndef __GLOBAL_H
#define __GLOBAL_H

/// HARDWARE CONFIG
#define ADC_PIN 26
#define OK_BUTTON 12
#define MENU_BUTTON 13
#define SWITCH_BUTTON 20

/// MEASSURE CONFIG
#define SAMPLE_COUNT (2048)
#define SAMPLING_INTERVAL_US (100)
#define SAMPLING_FREQ 1000 

enum states{
    init,
    menu,
    measure,
    calibration,
    debug
};
enum Menu{
    mmeasure,
    mcalibration,
    mdebug
};

volatile enum states state = init;
char enter = 0;
char button_next_value = 0;
char button_enter_value = 0;
enum Menu menu_selection = mmeasure;
char number_of_menu_options = 3;//measure,calibration,debug

float thres = 1.5;
volatile uint8_t unit =0;


#endif