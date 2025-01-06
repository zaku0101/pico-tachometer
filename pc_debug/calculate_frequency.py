import numpy as np
import matplotlib.pyplot as plt

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

# coefficients= [
#    -96955808568.91116,
#   273387046642.8124,
#   510724163306.0944,
#   490190243486.0663,
#   31417435894.008507,
#   -563122797532.523,
#   -600504184176.4781,
#   -489518701417.9829,
#   68743438643.43535,
#   191351321424.93033,
#   -46615243951.24134,
#   -457851694837.70386,
#   -550093000871.2968,
#   -123270391291.70703,
#   792405090913.8821,
#   1139426164674.4077,
#   792405090913.8821,
#   -123270391291.70703,
#   -550093000871.2968,
#   -457851694837.70386,
#   -46615243951.24134,
#   191351321424.93033,
#   68743438643.43535,
#   -489518701417.9829,
#   -600504184176.4781,
#   -563122797532.523,
#   31417435894.008507,
#   490190243486.0663,
#   510724163306.0944,
#   273387046642.8124,
#   -96955808568.91116]

FILTER_TAP_NUM = 204
coefficients = [
  -2.446073790147e-07,-0.000149028749508,-0.0002848829353408,-0.0005246931507749,
  -0.0008589444539459,-0.001288839168955,-0.001802326970252,-0.002372065341086,
  -0.002955321590145,-0.003496374093523,-0.003931520944614,-0.004196489373426,
  -0.004235413798881,-0.004010230559684,-0.003509031536355,-0.002751890360187,
  -0.001792824878893,-0.0007168971233036, 0.000367744190839, 0.001342237123732,
   0.002092455228622, 0.002525538797147, 0.002585053840852, 0.002262100108755,
   0.001600179584617,0.0006925967171042,-0.0003279932141046,-0.001307085970012,
  -0.002090591056822,-0.002548560490008, -0.00259684433163, -0.00221308533491,
  -0.001443697506758,-0.0004000946143537,0.0007556827100593, 0.001834785728268,
   0.002651433034233,  0.00305382068875, 0.002951679724526, 0.002335291199352,
    0.00128158413985,-5.314863373646e-05,-0.001455492539544,-0.002687429833875,
  -0.003525869831387,-0.003801723574104,-0.003432521891229,-0.002442131586046,
  -0.0009635616034021,0.0007769734135435, 0.002493101135661, 0.003883980866317,
   0.004685522294526, 0.004718709127545, 0.003926449239074, 0.002391732196156,
  0.0003327536902548,-0.001925764072806,-0.004001760263012,-0.005518367865843,
  -0.006169919520496,-0.005780247602129,-0.004342635520597,-0.002033200817242,
  0.0008064626852395, 0.003716546769412, 0.006190279965713, 0.007758573418821,
   0.008074409336993, 0.006982597028924,  0.00456176966625,  0.00112931106165,
  -0.002794282804282,-0.006558775117629,-0.009485693011278, -0.01098288773651,
   -0.01065404079615,-0.008384158872107,-0.004384861353617,0.0008111633489428,
   0.006410131600549,   0.0114657771296,  0.01502347386108,  0.01627912673923,
    0.01472911854001,  0.01028659772564,  0.00334355741969,-0.005235344798654,
   -0.01419124117388, -0.02201553726411, -0.02714337468968,  -0.0281711001911,
   -0.02406765485521, -0.01434915092306,0.0008101495438378,   0.0205493730698,
    0.04338288124406,  0.06734775233948,  0.09022003446999,   0.1097726894855,
     0.1240418275456,   0.1315659331717,   0.1315659331717,   0.1240418275456,
     0.1097726894855,  0.09022003446999,  0.06734775233948,  0.04338288124406,
     0.0205493730698,0.0008101495438378, -0.01434915092306, -0.02406765485521,
    -0.0281711001911, -0.02714337468968, -0.02201553726411, -0.01419124117388,
  -0.005235344798654,  0.00334355741969,  0.01028659772564,  0.01472911854001,
    0.01627912673923,  0.01502347386108,   0.0114657771296, 0.006410131600549,
  0.0008111633489428,-0.004384861353617,-0.008384158872107, -0.01065404079615,
   -0.01098288773651,-0.009485693011278,-0.006558775117629,-0.002794282804282,
    0.00112931106165,  0.00456176966625, 0.006982597028924, 0.008074409336993,
   0.007758573418821, 0.006190279965713, 0.003716546769412,0.0008064626852395,
  -0.002033200817242,-0.004342635520597,-0.005780247602129,-0.006169919520496,
  -0.005518367865843,-0.004001760263012,-0.001925764072806,0.0003327536902548,
   0.002391732196156, 0.003926449239074, 0.004718709127545, 0.004685522294526,
   0.003883980866317, 0.002493101135661,0.0007769734135435,-0.0009635616034021,
  -0.002442131586046,-0.003432521891229,-0.003801723574104,-0.003525869831387,
  -0.002687429833875,-0.001455492539544,-5.314863373646e-05,  0.00128158413985,
   0.002335291199352, 0.002951679724526,  0.00305382068875, 0.002651433034233,
   0.001834785728268,0.0007556827100593,-0.0004000946143537,-0.001443697506758,
   -0.00221308533491, -0.00259684433163,-0.002548560490008,-0.002090591056822,
  -0.001307085970012,-0.0003279932141046,0.0006925967171042, 0.001600179584617,
   0.002262100108755, 0.002585053840852, 0.002525538797147, 0.002092455228622,
   0.001342237123732, 0.000367744190839,-0.0007168971233036,-0.001792824878893,
  -0.002751890360187,-0.003509031536355,-0.004010230559684,-0.004235413798881,
  -0.004196489373426,-0.003931520944614,-0.003496374093523,-0.002955321590145,
  -0.002372065341086,-0.001802326970252,-0.001288839168955,-0.0008589444539459,
  -0.0005246931507749,-0.0002848829353408,-0.000149028749508,-2.446073790147e-07
]
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

MWSPT_NSEC = 11
NL = [1, 3, 1, 3, 1, 3, 1, 3, 1, 2, 1]
NUM = [
    [0.1582496408197, 0, 0],
    [1, -1.79203654695, 1],
    [0.5550797854312, 0, 0],
    [1, -1.762644221962, 1],
    [0.2630254719122, 0, 0],
    [1, -1.656079075518, 1],
    [0.04138939175369, 0, 0],
    [1, -1.140276135946, 1],
    [0.1906572073082, 0, 0],
    [1, 1, 0],
    [1, 0, 0]
]
DL = [1, 3, 1, 3, 1, 3, 1, 3, 1, 2, 1]
DEN = [
    [1, 0, 0],
    [1, -1.838431340129, 0.9896200174297],
    [1, 0, 0],
    [1, -1.83131889999, 0.9630702943343],
    [1, 0, 0],
    [1, -1.830442232761, 0.9209021962229],
    [1, 0, 0],
    [1, -1.83253976838, 0.8681232161892],
    [1, 0, 0],
    [1, -0.9169970155177, 0],
    [1, 0, 0]
]

def iir_filter(input_signal):
    """
    Implementacja filtra IIR
    :param input_signal: Lista lub tablica z próbkami sygnału wejściowego
    :return: Lista z próbkami sygnału wyjściowego
    """
    # Liczba sekcji filtra
    n_sections = len(NUM)
    print(f"sekce : {n_sections}")
    # Liczba próbek sygnału wejściowego
    n_samples = len(input_signal)
    # Lista na wynik
    output_signal = input_signal.copy()
    
    # Iteracja przez sekcje filtra
    for section in range(n_sections):
        # Współczynniki licznika i mianownika
        b = NUM[section]
        a = DEN[section]
        # Lista na wynik dla bieżącej sekcji
        section_output = [0.0] * n_samples
        
        # Iteracja przez sygnał wejściowy
        for n in range(n_samples):
            # Obliczanie splotu dla licznika
            for k in range(len(b)):
                if n - k >= 0:  # Sprawdzenie zakresu indeksu
                    section_output[n] += b[k] * output_signal[n - k]
            # Obliczanie splotu dla mianownika
            for k in range(1, len(a)):
                if n - k >= 0:  # Sprawdzenie zakresu indeksu
                    section_output[n] -= a[k] * section_output[n - k]
            # Normalizacja wyniku
            section_output[n] /= a[0]
        
        # Aktualizacja sygnału wyjściowego
        output_signal = section_output
    
    return output_signal

def freq_fft_interpolate(input_signal, fp, N):
    """
    Implementacja interpolacji za pomocą FFT
    :param input_signal: Lista lub tablica z próbkami sygnału wejściowego
    :return: Lista z próbkami sygnału wyjściowego
    """
    m = 3
    # Liczba próbek sygnału wejściowego
    n_samples = len(input_signal)
    # Subtract the mean (DC component) from the input signal
    input_signal = input_signal - np.mean(input_signal)

    # Apply a sin^m window to the input signal
    window = np.sin(np.pi * np.arange(n_samples) / (n_samples - 1)) ** m
    input_signal = input_signal * window

    # Liczba próbek sygnału wyjściowego
    n_samples_out = n_samples * 2
    # Lista na wynik
    output_signal = [0.0] * n_samples_out
    output_signal[:n_samples] = input_signal
 
    # Obliczanie transformaty Fouriera
    output_signal = np.fft.fft(output_signal)
    fftabs = np.abs(output_signal)

    # # Display only the first half of the spectrum
    # plt.figure(figsize=(10, 6))
    # plt.plot(fftabs[:n_samples_out // 2], 'b-', linewidth=1.2, label='fft')
    # plt.title('FFT Spectrum (First Half)')
    # plt.xlabel('Frequency Bin')
    # plt.ylabel('Magnitude')
    # plt.legend()
    # plt.grid(True)
    # Find the sample with the maximum spectrum value
    l = np.argmax(fftabs[:n_samples_out // 2])
    # print(f"l: {l}")
    l1 = l/2
    # l = np.int64(l/2)
    # Interpolate the spectrum
    epsilon = (m+2)*((m+2+4*l1)*output_signal[l+2] + 2*m*output_signal[l] + (m+2-4*l1)*output_signal[l-2])/(4*(output_signal[l+2] + output_signal[l-2] - 2*output_signal[l]))
    lamb = np.sqrt(l**2 + np.real(epsilon))

    return lamb*fp/(2*N)