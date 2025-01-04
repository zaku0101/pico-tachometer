import numpy as np
import matplotlib.pyplot as plt
from calculate_frequency import calculate_frequency, fir_filter


# Parameters
fp = 4e3          # Sampling frequency
fmax = 200         # Maximum frequency of interest
fmin = 5
min_okres = 3      # Minimum number of cycles
N = int(min_okres * fp / fmin)  # Total number of samples
print(f"Liczba probek: {N}")
# Time vector
t = np.linspace(0, (N - 1) / fp, N)

delta_min = 50
sampling_interval_us = 1 / fp  # 48 kHz
freq_error = []
K = 10
for fref in range(fmin, fmax):
    frequency = []
    print(fref)
    for nois in range(1,K):
        y = 1048/np.e*np.exp(np.pow(np.sin(2 * np.pi * fref *t),1)) + (1/8) * np.sin(fref*3 * 2 * np.pi * t) + 2048*0.1 * np.random.randn(N)
        y = np.round(y)
       
        ## IIR low pass
        y_lp = np.zeros(N)
        decay = 0.975
        b = 1 - decay
        y_temp = np.mean(y)
        for i in range (1,N+1):
            y_temp += b*(y[i-1]-y_temp)
            y_lp[i-1] = y_temp
        
        if nois == 1 and fref == fmax-1:
            plt.figure(figsize=(10, 12))
            plt.plot(t, y, 'b-', linewidth=0.2, label='raw')
            plt.plot(t,y_lp,'r-',linewidth=1.2, label='IIR')
            plt.title('Signal')
            plt.xlabel('time')
            plt.ylabel('value')
            plt.legend()
            plt.grid(True)

            plt.tight_layout()
            plt.show()

        y_lp_min = np.min(y_lp)
        y_lp_max = np.max(y_lp)
        threshold_lp = (y_lp_min + y_lp_max) / 2.0
        y_lp_binary = y_lp > threshold_lp  # Binarize

        frequency.append(calculate_frequency(y_lp, delta_min, sampling_interval_us))
    ftest = np.mean(frequency)
    freq_error.append(np.abs(ftest-fref))
    


f = np.linspace(start = fmin, num =195, stop = fmax)
plt.figure(figsize=(10, 12))
plt.plot(f, freq_error, 'b-', linewidth=1.2, label='IIR')
plt.title('Freq estimation error')
plt.xlabel('freq')
plt.ylabel('Absolut error')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()