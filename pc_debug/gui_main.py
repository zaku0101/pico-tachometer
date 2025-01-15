import serial
import threading
import time
import numpy as np
import tkinter as tk
from tkinter import ttk, messagebox
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
from queue import Queue


class SerialPlotterApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Real-Time ADC Plotter with FFT")
        self.running = False
        self.data_queue = Queue()
        self.data = []
        self.sampling_frequency = 1000  # Default sampling frequency
        self.num_samples = 1000  # Default number of samples for FFT
        self.ser = None
        self.console_enabled = True
        self.fft_enabled = False

        # GUI Layout
        self.create_widgets()

        # Plotting
        self.fig, (self.ax_time, self.ax_fft) = plt.subplots(2, 1, figsize=(6, 8))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_frame)
        self.canvas.get_tk_widget().pack()
        self.line, = self.ax_time.plot([], [], label="ADC Data")
        self.ax_time.set_title("Time Domain")
        self.ax_time.set_xlabel("Sample")
        self.ax_time.set_ylabel("ADC Value")
        self.ax_time.grid()
        self.ax_time.legend(loc="upper right")
        self.ax_fft.set_title("Frequency Domain (FFT)")
        self.ax_fft.set_xlabel("Frequency (Hz)")
        self.ax_fft.set_ylabel("Magnitude")
        self.ax_fft.grid()

    def create_widgets(self):
        # Connection Frame
        connection_frame = tk.Frame(self.root)
        connection_frame.pack(pady=10)

        tk.Label(connection_frame, text="Serial Port:").grid(row=0, column=0, padx=5)
        self.port_entry = tk.Entry(connection_frame)
        self.port_entry.grid(row=0, column=1, padx=5)
        self.port_entry.insert(0, "COM11")  # Default port

        tk.Label(connection_frame, text="Baud Rate:").grid(row=0, column=2, padx=5)
        self.baud_entry = tk.Entry(connection_frame)
        self.baud_entry.grid(row=0, column=3, padx=5)
        self.baud_entry.insert(0, "115200")  # Default baud rate

        self.connect_button = tk.Button(connection_frame, text="Connect", command=self.toggle_connection)
        self.connect_button.grid(row=0, column=4, padx=5)

        self.console_toggle_button = tk.Button(connection_frame, text="Disable Console Log", command=self.toggle_console_log)
        self.console_toggle_button.grid(row=0, column=5, padx=5)

        self.fft_toggle_button = tk.Button(connection_frame, text="Enable FFT", command=self.toggle_fft)
        self.fft_toggle_button.grid(row=0, column=6, padx=5)

        # Settings Frame
        settings_frame = tk.Frame(self.root)
        settings_frame.pack(pady=10)

        tk.Label(settings_frame, text="Sampling Frequency (Hz):").grid(row=0, column=0, padx=5)
        self.freq_entry = tk.Entry(settings_frame)
        self.freq_entry.grid(row=0, column=1, padx=5)
        self.freq_entry.insert(0, str(self.sampling_frequency))

        tk.Label(settings_frame, text="Number of Samples:").grid(row=0, column=2, padx=5)
        self.samples_entry = tk.Entry(settings_frame)
        self.samples_entry.grid(row=0, column=3, padx=5)
        self.samples_entry.insert(0, str(self.num_samples))

        # Plot Frame
        self.plot_frame = tk.Frame(self.root)
        self.plot_frame.pack(pady=10)

        # Console Log Frame
        console_frame = tk.Frame(self.root)
        console_frame.pack(pady=10)

        tk.Label(console_frame, text="Serial Console Log:").pack()
        self.console_log = tk.Text(console_frame, height=10, width=80, state=tk.DISABLED)
        self.console_log.pack()

    def toggle_connection(self):
        if self.running:
            self.disconnect()
        else:
            self.connect()

    def connect(self):
        port = self.port_entry.get()
        baud_rate = int(self.baud_entry.get())
        self.sampling_frequency = int(self.freq_entry.get())
        self.num_samples = int(self.samples_entry.get())

        try:
            self.ser = serial.Serial(port, baud_rate, timeout=0)
            self.running = True
            self.connect_button.config(text="Disconnect")
            threading.Thread(target=self.read_serial_data, daemon=True).start()
            threading.Thread(target=self.plot_data, daemon=True).start()
        except serial.SerialException as e:
            messagebox.showerror("Error", f"Failed to connect to {port}: {e}")

    def disconnect(self):
        self.running = False
        if self.ser:
            self.ser.close()
        self.connect_button.config(text="Connect")

    def read_serial_data(self):
        while self.running:
            if self.ser.in_waiting > 0:
                try:
                    line_data = self.ser.readline().decode("utf-8").strip()
                    if line_data:
                        if self.console_enabled:
                            self.append_to_console(line_data)
                        value = float(line_data)
                        self.data_queue.put(value)
                except ValueError:
                    continue
            

    def append_to_console(self, message):
        self.console_log.config(state=tk.NORMAL)
        self.console_log.insert(tk.END, message + "\n")
        self.console_log.see(tk.END)
        self.console_log.config(state=tk.DISABLED)

    def plot_data(self):
        counter = 0
        while self.running:
            new_data = []
            while not self.data_queue.empty():
                new_data.append(self.data_queue.get())
                counter += 1

            if new_data:
                self.data.extend(new_data)
                if len(self.data) > self.num_samples:
                    self.data = self.data[-self.num_samples:]

                self.update_time_plot()
                print(counter)
                if self.fft_enabled and (counter > 2048):
                    self.update_frequency_plot()
                    counter = 0

            time.sleep(0.05)

    def update_time_plot(self):
        self.line.set_data(range(len(self.data)), self.data)
        self.ax_time.relim()
        self.ax_time.autoscale_view()
        self.canvas.draw_idle()

    def update_frequency_plot(self):
        mean_value = np.mean(self.data)
        self.data = [x - mean_value for x in self.data]

        fft_data = np.fft.fft(self.data)
        freqs = np.fft.fftfreq(len(self.data), d=1/self.sampling_frequency)
        magnitude = np.abs(fft_data)[:len(fft_data)//2]
        freqs = freqs[:len(freqs)//2]

        self.ax_fft.clear()
        self.ax_fft.plot(freqs, magnitude, label="FFT Magnitude")
        self.ax_fft.set_title("Frequency Domain (FFT)")
        self.ax_fft.set_xlabel("Frequency (Hz)")
        self.ax_fft.set_ylabel("Magnitude")
        self.ax_fft.grid()
        self.ax_fft.legend()
        self.canvas.draw_idle()

    def toggle_console_log(self):
        self.console_enabled = not self.console_enabled
        self.console_toggle_button.config(
            text="Enable Console Log" if not self.console_enabled else "Disable Console Log"
        )

    def toggle_fft(self):
        self.fft_enabled = not self.fft_enabled
        self.fft_toggle_button.config(
            text="Disable FFT" if self.fft_enabled else "Enable FFT"
        )


if __name__ == "__main__":
    root = tk.Tk()
    app = SerialPlotterApp(root)
    root.mainloop()
