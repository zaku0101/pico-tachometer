#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 25

int main(){
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  
  stdio_init_all();

  while(1){
  printf("Blinking \r\n");
  gpio_put(LED_PIN, true);
  sleep_ms(1000);
  gpio_put(LED_PIN, false);
  sleep_ms(1000);
  }
}
