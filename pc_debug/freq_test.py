import numpy as np
import matplotlib.pyplot as plt

# Parameters
fp = 48e3
fmax = 200
min_okres = 3
N = int(min_okres * fp / fmax)

# Time vector
t = np.linspace(0, (N - 1) / fp, N)

# Signal generation
y = 5 - np.exp(-t) + np.sin(200 * 2 * np.pi * t) + (1 / 8) * np.sin(600 * 2 * np.pi * t) + 0.1 * np.random.randn(N)
y = y * 2048

# Plot original signal
plt.figure(1)
plt.plot(t, y, 'r', label='Original Signal')

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

# Plot filtered signal
plt.plot(t, y_filtered, 'b-', label='Filtered Signal')
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.title('Moving Median Low-Pass Filter')

# Binarize the filtered signal
y_min, y_max = np.min(y_filtered), np.max(y_filtered)
threshold = (y_min + y_max) / 2
y_binarized = y_filtered >= threshold  # Logical array: 0 (below), 1 (above)
y_binarized_scaled = y_binarized * (y_max - y_min) + y_min

# Plot the binarized signal
plt.plot(t, y_binarized_scaled, 'k-', label='Binarized Signal')
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.title('Moving Median Filter with Binarization')

plt.figure(2)

# Leaky integrator filter implementation
alpha = 0.1  # Leaky integrator coefficient (0 < alpha < 1)
y_leaky = np.zeros(N)
y_leaky[0] = y[0]

for i in range(1, N):
    y_leaky[i] = alpha * y[i] + (1 - alpha) * y_leaky[i - 1]


# Plot comparison of filters
plt.plot(t, y_filtered, 'b-', linewidth=1.5, label='Moving Median Filter')
plt.plot(t, y_leaky, 'g-', linewidth=1.5, label='Leaky Integrator Filter')
plt.legend()
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.title('Comparison of Moving Median and Leaky Integrator Filters')
plt.grid()
plt.show()

# Convert array to C-style syntax
c_array = ', '.join(map(str, map(int, y)))
c_array = f'{{ {c_array} }}'

# Write to file
with open('./symulacje/array_export2.h', 'w') as file:
    file.write(f'int my_array[] = {c_array};\n')
