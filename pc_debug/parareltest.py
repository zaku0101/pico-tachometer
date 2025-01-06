from joblib import Parallel, delayed
import numpy as np
import matplotlib.pyplot as plt
from calculate_frequency import calculate_frequency, freq_fft_interpolate

# Parameters
fp = 2e3          # Sampling frequency
fmax = 1000         # Maximum frequency of interest
fmin = 1
min_okres = 2      # Minimum number of cycles
N = int(min_okres * fp / fmin)  # Total number of samples
step = 0.01

def process_noise(fref, nois, t, N, delta_min, sampling_interval_us):
    y = 512 / np.e * np.exp(np.power(np.sin(2 * np.pi * fref * t + 2*np.pi*np.random.rand(1)), 1)) + \
        (1/8) * np.sin(fref * 3 * 2 * np.pi * t) + \
        20 * np.random.randn(N)
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

    return freq_fft_interpolate(y,fp,N)
    # return calculate_frequency(y_lp, delta_min, sampling_interval_us)

def main(fmin, fmax, t, N, delta_min, sampling_interval_us, K):
    freq_error = []
    freq_sigma = []
    chuj = range(fmin, fmax)
    print(chuj)
    for fref in np.arange(fmin, fmax, step):
        print(fref)
        # Parallelize the inner loop
        frequency = Parallel(n_jobs=7)(delayed(process_noise)(fref, nois, t, N, delta_min, sampling_interval_us) for nois in range(1, K))
        ftest = np.mean(frequency)
        freq_sigma.append(np.std(frequency)*60)
        freq_error.append(np.abs(ftest - fref)*60)
    print(len(freq_error))
    f = np.arange(fmin*60, fmax*60, step*60)
    # f = np.linspace(start = fmin, num =len(freq_error)*step, stop = fmax*60)
    print(f"dlugosc f {len(f*60)} dlugosc freq_error {len(freq_error*60)}")
    plt.figure(figsize=(10, 12))
    plt.plot(f, freq_error, 'b-', linewidth=1.2, label='mean')
    plt.plot(f, freq_sigma, 'r-', linewidth=1.2, label='var')
    plt.title('RPM estimation error with interpolated fft')
    plt.xlabel('RPM')
    plt.ylabel('Estimation absolut error')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

print(f"Liczba probke: {N}")
delta_min = 50
sampling_interval_us = 1 / fp  # 48 kHz
# Time vector
t = np.linspace(0, (N - 1) / fp, N)
K = 1000
main(fmin,fmax,t,N,delta_min,sampling_interval_us,K)
