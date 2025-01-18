#include "include/global.h"

int var_sample_count = SAMPLE_COUNT;
volatile enum states state = init;
char enter = 0;
char button_next_value = 0;
char button_enter_value = 0;
enum Menu menu_selection = mmeasure;
char number_of_menu_options = 3; 
float thres = 1.5;
volatile uint8_t unit = 0;
int sampling_time = 2;