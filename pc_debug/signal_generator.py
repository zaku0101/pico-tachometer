import numpy as np

def generate_sine_wave(frequency, sampling_frequency, num_samples):
    t = np.linspace(0, num_samples / sampling_frequency, num_samples, endpoint=False)
    sine_wave = 0.5*np.sin(2 * np.pi * frequency * t)
    noise = np.random.normal(0, 0.05, num_samples)
    sine_wave += noise
    return sine_wave

def convert_to_uint8(data):
    # Scale sine wave to range 0-255 and convert to uint8
    data_uint8 = ((data + 1) * 127.5).astype(np.uint8)
    return data_uint8

def write_c_header(filename, array_name, data):
    with open(filename, 'w') as f:
        f.write("#ifndef SINE_WAVE_H\n")
        f.write("#define SINE_WAVE_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"uint8_t {array_name}[] = {{\n")
        for value in data:
            f.write(f"    {value},\n")
        f.write("};\n\n")
        f.write("#endif // SINE_WAVE_H\n")

def main():
    frequency = 57.21  # Frequency of the sine wave in Hz
    sampling_frequency = 1000  # Sampling frequency in Hz
    num_samples = 2048  # Number of samples

    sine_wave = generate_sine_wave(frequency, sampling_frequency, num_samples)
    sine_wave_uint8 = convert_to_uint8(sine_wave)
    write_c_header("sine_wave.h", "sine_wave", sine_wave_uint8)

if __name__ == "__main__":
    main()