import numpy as np
import matplotlib.pyplot as plt
from calculate_frequency import calculate_frequency, fir_filter, iir_filter, freq_fft_interpolate

# Parameters
fp = 1e3          # Sampling frequency
fmax = 200         # Maximum frequency of interest
fmin = 1
min_okres = 3      # Minimum number of cycles
N = int(min_okres * fp / fmin)  # Total number of samples
print(f"Liczba probek: {N}")

# Time vector
t = np.linspace(0, (N - 1) / fp, N)

# Input signal
freq = 1.2
y = 5 - np.exp(-t) + np.sin(freq * 2 * np.pi * t + np.pi/3) + (1 / 8) * np.sin(freq*3 * 2 * np.pi * t) + 0.5 * np.random.randn(N)
y = y * 2048  # Scale the signal
# y = 2048/np.e*np.exp(np.pow(np.sin(2 * np.pi * freq *t),1)) + (1/8) * np.sin(freq*3 * 2 * np.pi * t) + 2048*0.1 * np.random.randn(N)

# Generate sine wave
sine_wave = np.sin(2 * np.pi * freq * t)

# Generate binary signal
binary_signal = (sine_wave >= 0).astype(int)  # 1 for positive, 0 for negative

# # Plot original signal
# plt.figure()
# plt.plot(t, y, 'r.', label='Original Signal')
# plt.title('Original Signal')
# plt.xlabel('Time [s]')
# plt.ylabel('Amplitude')
# plt.grid(True)
# plt.show()

# Quantize signal
y = np.round(y)

# Moving median filter implementation
window_size = 11  # Window size (must be odd for simplicity)
half_window = window_size // 2
y_filtered = np.zeros(N)

for i in range(N):
    start_idx = max(0, i - half_window)
    end_idx = min(N, i + half_window + 1)
    window = y[start_idx:end_idx]
    y_filtered[i] = np.median(window)

# Leaky integrator filter implementation
alpha = 0.02  # Leaky integrator coefficient (0 < alpha < 1)
y_leaky = np.zeros(N)
y_leaky[0] = y[0]  # Initial condition

for i in range(1, N):
    y_leaky[i] = alpha * y[i] + (1 - alpha) * y_leaky[i - 1]

## IIR low pass
y_lp = np.zeros(N)
decay = 0.998
b = 1 - decay
y_temp = np.mean(y)
for i in range (1,N+1):
    if i == N-1:
        print("Hello")
    y_temp += b*(y[i-1]-y_temp)
    y_lp[i-1] = y_temp

## FIR FIlter
y_fir = iir_filter(y)

# Binarization of Moving Median Filter
y_median_min = np.min(y_filtered)
y_median_max = np.max(y_filtered)
threshold_median = (y_median_min + y_median_max) / 2
y_median_binary = y_filtered >= threshold_median  # Binarize

# Binarization of Leaky Integrator
y_leaky_min = np.min(y_leaky)
y_leaky_max = np.max(y_leaky)
threshold_leaky = (y_leaky_min + y_leaky_max) / 2
y_leaky_binary = y_leaky >= threshold_leaky  # Binarize


# Binarization of Leaky Integrator
y_lp_min = np.min(y_lp)
y_lp_max = np.max(y_lp)
threshold_lp = (y_lp_min + y_lp_max) / 2.0
y_lp_binary = y_lp > threshold_lp  # Binarize


# Binarization of Leaky Integrator
y_fir_min = np.min(y_fir)
y_fir_max = np.max(y_fir)
threshold_fir = (y_fir_min + y_fir_max) / 2.0
y_fir_binary = y_fir > threshold_fir  # Binarize


#fft interpolation
f_fft = freq_fft_interpolate(y,fp,N)
print(f"Frequency fft: {f_fft} Hz")