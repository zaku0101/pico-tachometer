#ifndef _CALCULATION_H
#define _CALCULATION_H

#include <inttypes.h>
#include <stdio.h>
#include "global.h"
// Function to calculate frequency
inline float calculate_frequency(uint16_t *data, size_t count, uint16_t delta_min) {
    uint16_t min_value = 4095, max_value = 0;
    int crossings = 0;
    float frequency = 0.0;
    // Find min and max values
    for (size_t i = 0; i < count; i++) {
        if (data[i] < min_value) min_value = data[i];
        if (data[i] > max_value) max_value = data[i];
    }
    // Calculate floating threshold
    float threshold = (min_value + max_value) / 2.0;
    float delta = max_value - min_value;
    printf("Delta: %f\n\t", delta);
    if(delta<delta_min) return frequency;

    // Count threshold crossings (rising edge)
    for (size_t i = 1; i < count; i++) {
        if (data[i - 1] < threshold && data[i] >= threshold) {
            crossings++;
        }
    }

    // Calculate frequency
    float time_interval = (float)(count * ((float)SAMPLING_INTERVAL_US + 1/48000.0)) / 1e6; // Convert microseconds to seconds
    frequency = (crossings) / time_interval;

    return frequency;
}

#endif