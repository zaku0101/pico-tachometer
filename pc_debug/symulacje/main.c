#include "global.h"
//#include "calculation.h"
#include "array_export.h"
#include <stdio.h>
#include "inttypes.h"
#include <math.h>

float calculate_frequency(uint16_t *data, int count, uint16_t delta_min);

int main(){

    float f = calculate_frequency(my_array,SAMPLE_COUNT,1);
    printf("czestotliowsc = %f", f);
    return 0;
}



float calculate_frequency(uint16_t *data, int count, uint16_t delta_min) {
    uint16_t min_value = 4095, max_value = 0;
    int crossings = 0;
    float frequency = 0.0;
    // Find min and max values
    for (int i = 0; i < count; i++) {
        if (data[i] < min_value) min_value = data[i];
        if (data[i] > max_value) max_value = data[i];
    }
    // Calculate floating threshold
    float threshold = (min_value + max_value) / 2.0;
    float delta = max_value - min_value;
    printf("Delta: %f\n\t", delta);
    if(delta<delta_min) return frequency;

    // Count threshold crossings (rising edge)
    for (int i = 1; i < count; i++) {
        if (data[i - 1] < threshold && data[i] >= threshold) {
            crossings++;
        }
    }
    float bins_diff = 0;
    int last_bin = 0;
    int k = 0;
    float sum = 0;
    // Time threshold crossings (rising edge)
    for (int i = 1; i < count; i++) {
        if (data[i - 1] < threshold && data[i] >= threshold) {
            if(k==0){
                last_bin = (int)(i);
                k++;
                continue;
            }
            sum = sum + (i - last_bin);
            last_bin = i;
            k++;
        }
    }
    bins_diff = sum / (float)k;
    printf("Bins diff: %f, k = %d\n", bins_diff, k);
    // frequency = ((float)SAMPLING_INTERVAL_US + 1/48000.0)* (bins_diff)*(2*M_PI);
    // frequency = 1/frequency;
    // Calculate frequency
    float time_interval = (float)(count * ((float)SAMPLING_INTERVAL_US + 1/48000.0)); // Convert microseconds to seconds
    frequency = (crossings) / time_interval;

    return frequency;
}