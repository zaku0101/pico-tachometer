#ifndef __GLOBAL_H
#define __GLOBAL_H

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

#endif