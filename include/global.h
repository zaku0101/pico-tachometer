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

extern int var_sample_count;

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

extern volatile enum states state;
extern char enter;
extern char button_next_value;
extern char button_enter_value;
extern enum Menu menu_selection;
extern char number_of_menu_options;//measure,calibration,debug

extern float thres;
extern volatile uint8_t unit;
extern int sampling_time;


#endif