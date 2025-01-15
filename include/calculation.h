#ifndef _CALCULATION_H
#define _CALCULATION_H

#include <inttypes.h>
#include <stdio.h>
//#include "global.h"
#include "pico/fft.h"
#define SAMPLING_INTERVAL_US (100)
#pragma once
float calculate_frequency(uint8_t *capture_buf);


#endif