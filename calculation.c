#include "include/pico/kiss_fftr.h"
#include "include/calculation.h"
#include "include/global.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define P (4)

static float window[SAMPLE_COUNT];

static void precalculate_window() {
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        window[i] = powf(sinf((M_PI * i) / (SAMPLE_COUNT - 1)), P);
    }
}

__attribute__((constructor))
static void initialize() {
    precalculate_window();
}

static void fill_fft_input(uint8_t *buffer, kiss_fft_scalar *fft_in, int size) {
  float avg = calculate_average(buffer, size);
  for (int i = 0; i < size; i++) {
    fft_in[i] = ((float)buffer[i] - avg)*window[i];
  }
}

float fft_interpolation_process(uint8_t *capture_buf) {
  kiss_fft_scalar fft_in[SAMPLE_COUNT];
  kiss_fft_cpx fft_out[SAMPLE_COUNT];
  kiss_fftr_cfg cfg = kiss_fftr_alloc(SAMPLE_COUNT, 0, NULL, NULL);

  if (!cfg) {
    fprintf(stderr, "Failed to allocate FFT configuration\n");
    return;
  }

  fill_fft_input(capture_buf, fft_in, SAMPLE_COUNT);
  kiss_fftr(cfg, fft_in, fft_out);

  float max_amplitude = 0;
  int max_index = 0;
  for (int i = 0; i < SAMPLE_COUNT / 2; i++) {
    float amplitude = sqrtf(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i);
    if (amplitude > max_amplitude) {
      max_amplitude = amplitude;
      max_index = i;
    }
  }
    kiss_fft_cpx X1, X2, X3;
    X1 = fft_out[max_index + 1];
    X2 = fft_out[max_index];
    X3 = fft_out[max_index - 1];

    float l1 = (float)max_index;
    float m = (float)P;
    kiss_fft_cpx epsilon;
    kiss_fft_cpx epsilon_div;
    epsilon.r = (m + 2) * ((m + 2 + 4 * l1) * X1.r  +  2 * m * X2.r  +  (m + 2 - 4 * l1) * X3.r);
    epsilon.i = (m + 2) * ((m + 2 + 4 * l1) * X1.i  +  2 * m * X2.i  +  (m + 2 - 4 * l1) * X3.i);

    epsilon_div.r =  4 * (X1.r - 2*X2.r + X3.r);
    epsilon_div.i = -4 * (X1.i - 2*X2.i + X3.i);
    float magnitude_epsilon_div = sqrtf(epsilon_div.r * epsilon_div.r + epsilon_div.i * epsilon_div.i);

    epsilon.r /= magnitude_epsilon_div;
    epsilon.i /= magnitude_epsilon_div;
    C_MUL(epsilon, epsilon, epsilon_div);

    float freq = (l1*l1 + epsilon.r) * SAMPLING_FREQ / (2*SAMPLE_COUNT);
    printf("Frequency: %f Hz\n", freq);
    return sqrtf(freq);


  kiss_fft_free(cfg);
}
