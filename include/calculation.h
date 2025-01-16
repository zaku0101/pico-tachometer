#ifndef _CALCULATION_H
#define _CALCULATION_H

#include <inttypes.h>
#include <stdio.h>
#include "pico/fft.h"
#define SAMPLING_INTERVAL_US (100)
#pragma once
float calculate_frequency(uint16_t *capture_buf);
void precalculate_window();

#endif