import numpy as np

def calculate_frequency(data, delta_min, sampling_interval_us=1/48000.0):
    """
    Calculate frequency from the provided data array.

    Parameters:
    - data: list or numpy array of uint16 values (input signal data).
    - delta_min: minimum delta threshold to consider a valid frequency.
    - sampling_interval_us: time interval between samples in microseconds. Default is for a 48 kHz sampling rate.

    Returns:
    - frequency: Calculated frequency in Hz. Returns 0.0 if conditions are not met.
    """
    # Convert data to numpy array for easier manipulation
    data = np.array(data, dtype=np.uint16)
    
    # Find min and max values
    min_value = np.min(data)
    max_value = np.max(data)
    
    # Calculate delta and threshold
    delta = max_value - min_value
    # print(f"Delta: {delta}")
    
    if delta < delta_min:
        return 0.0  # No valid frequency if delta is below the threshold

    threshold = (min_value + max_value) / 2.0

    # Count threshold crossings (rising edge)
    crossings = 0
    last_bin = 0
    k = 0
    sum_bins_diff = 0.0

    for i in range(1, len(data)):
        if data[i - 1] < threshold and data[i] >= threshold:
            crossings += 1
            if k == 0:
                last_bin = i
                k += 1
                continue
            sum_bins_diff += (i - last_bin)
            last_bin = i
            k += 1

    if (k < 2 or sum_bins_diff == 0):
        return 0.0  # Prevent division by zero

    bins_diff = sum_bins_diff / (k-1)
    #print(f"k= {k}, binsdiff = {bins_diff}")
    # Calculate frequency
    sampling_interval_sec = sampling_interval_us 
    frequency = 1 / ( sampling_interval_sec * bins_diff)

    return frequency


# Example code, defines a class that implements a low-pass single-pole filter.

# Configuration.
decay = 0.9

class LowPassSinglePole:
    def __init__(self, decay):
        self.b = 1 - decay
        self.y = 0
    def filter(self, x):
        self.y += self.b * (x - self.y)
        return self.y

low_pass_single_pole = LowPassSinglePole(decay)

# Apply the filter function for each new input sample x, resulting in a new
# output sample y of the filtered signal:
# y = low_pass_single_pole.filter(x)

FILTER_TAP_NUM = 31

coefficients= [
   -96955808568.91116,
  273387046642.8124,
  510724163306.0944,
  490190243486.0663,
  31417435894.008507,
  -563122797532.523,
  -600504184176.4781,
  -489518701417.9829,
  68743438643.43535,
  191351321424.93033,
  -46615243951.24134,
  -457851694837.70386,
  -550093000871.2968,
  -123270391291.70703,
  792405090913.8821,
  1139426164674.4077,
  792405090913.8821,
  -123270391291.70703,
  -550093000871.2968,
  -457851694837.70386,
  -46615243951.24134,
  191351321424.93033,
  68743438643.43535,
  -489518701417.9829,
  -600504184176.4781,
  -563122797532.523,
  31417435894.008507,
  490190243486.0663,
  510724163306.0944,
  273387046642.8124,
  -96955808568.91116]

def fir_filter(input_signal):
    """
    Implementacja filtra FIR
    :param input_signal: Lista lub tablica z próbkami sygnału wejściowego
    :param coefficients: Lista współczynników filtra FIR
    :return: Lista z próbkami sygnału wyjściowego
    """
    # Liczba współczynników filtra
    n_coeffs = len(coefficients)
    # Liczba próbek sygnału wejściowego
    n_samples = len(input_signal)
    # Lista na wynik
    output_signal = [0.0] * n_samples
    
    # Iteracja przez sygnał wejściowy
    for n in range(n_samples):
        # Obliczanie splotu
        for k in range(n_coeffs):
            if n - k >= 0:  # Sprawdzenie zakresu indeksu
                output_signal[n] += input_signal[n - k] * coefficients[k]
    
    return output_signal