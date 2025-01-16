import numpy as np

import matplotlib.pyplot as plt

# Parameters
frequency = 50  # Hz
sampling_rate = 1000  # Hz
duration = 0.5  # seconds
duty_cycles = [10, 30, 50, 70, 90]  # Duty cycles in percentage

# Time array
t = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)

# Generate PWM signal
def generate_pwm(frequency, duty_cycle, t):
    return 2 * (((t * frequency) % 1) < (duty_cycle / 100)).astype(float) - 1

window = np.sin(np.pi * t / duration) ** 3

# Plot FFT for each duty cycle
plt.figure(figsize=(12, 8))
for duty_cycle in duty_cycles:
    pwm_signal = generate_pwm(frequency, duty_cycle, t)
    pwm_signal -= np.average(pwm_signal)
    print(np.average(pwm_signal))
    pwm_signal *= window
    yf = np.fft.fft(pwm_signal)
    xf = np.linspace(0.0, sampling_rate/(4*duration), int(len(yf)/2))
    plt.subplot(len(duty_cycles), 1, duty_cycles.index(duty_cycle) + 1)
    plt.plot(xf, np.abs(yf[:len(xf)]))
    plt.title(f'Duty Cycle: {duty_cycle}%')
    plt.ylabel('Amplitude')
    plt.grid()

plt.show()