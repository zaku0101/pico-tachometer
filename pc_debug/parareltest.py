from joblib import Parallel, delayed
import numpy as np
import matplotlib.pyplot as plt
from calculate_frequency import calculate_frequency

def process_noise(fref, nois, t, N, delta_min, sampling_interval_us):
    y = 512 / np.e * np.exp(np.power(np.sin(2 * np.pi * fref * t), 1)) + \
        (1/8) * np.sin(fref * 3 * 2 * np.pi * t) + \
        2048 * 0.1 * np.random.randn(N)
    y = np.round(y)

    # IIR low pass
    y_lp = np.zeros(N)
    decay = 0.975
    b = 1 - decay
    y_temp = np.mean(y)
    for i in range(1, N + 1):
        y_temp += b * (y[i - 1] - y_temp)
        y_lp[i - 1] = y_temp

    y_lp_min = np.min(y_lp)
    y_lp_max = np.max(y_lp)
    threshold_lp = (y_lp_min + y_lp_max) / 2.0
    y_lp_binary = y_lp > threshold_lp  # Binarize

    return calculate_frequency(y_lp, delta_min, sampling_interval_us)

def main(fmin, fmax, t, N, delta_min, sampling_interval_us, K):
    freq_error = []
    for fref in range(fmin, fmax):
        print(fref)
        # Parallelize the inner loop
        frequency = Parallel(n_jobs=7)(delayed(process_noise)(fref, nois, t, N, delta_min, sampling_interval_us) for nois in range(1, K))
        ftest = np.mean(frequency)
        freq_error.append(np.abs(ftest - fref))

    
    f = np.linspace(start = fmin, num =195, stop = fmax)
    plt.figure(figsize=(10, 12))
    plt.plot(f, freq_error, 'b-', linewidth=1.2, label='IIR')
    plt.title('Freq estimation error')
    plt.xlabel('freq')
    plt.ylabel('Estimation absolut error')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

# Parameters
fp = 4e3          # Sampling frequency
fmax = 200         # Maximum frequency of interest
fmin = 5
min_okres = 3      # Minimum number of cycles
N = int(min_okres * fp / fmin)  # Total number of samples
delta_min = 50
sampling_interval_us = 1 / fp  # 48 kHz
# Time vector
t = np.linspace(0, (N - 1) / fp, N)
K = 10
main(fmin,fmax,t,N,delta_min,sampling_interval_us,K)
