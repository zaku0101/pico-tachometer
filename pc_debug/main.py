import serial
import matplotlib.pyplot as plt
import time
import numpy as np
import threading

# Configuration
SERIAL_PORT = "COM11"  # Update with your serial port
BAUD_RATE = 115200
DATA_POINTS = 1000  # Number of data points to display

# Initialize serial connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0)  # Set timeout=0 for non-blocking
    print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

# Initialize plot
plt.ion()  # Turn on interactive mode
fig, ax = plt.subplots()
data = []
line, = ax.plot(data, label="ADC Data")
ax.set_title("Real-Time ADC Data Plot")
ax.set_xlabel("Sample")
ax.set_ylabel("ADC Value")
ax.legend()
ax.grid()

# Function to read data from serial port in a non-blocking manner
def read_serial_data():
    global data
    while True:
        if ser.in_waiting > 0:  # Check if there's data to read
            try:
                line_data = ser.readline().decode("utf-8").strip()
                if line_data:
                    value = float(line_data)  # Convert to float
                    data.append(value)
                    if len(data) > DATA_POINTS:
                        data = data[-DATA_POINTS:]  # Keep the last `DATA_POINTS` entries
            except ValueError:
                continue  # Ignore invalid data

# Start a thread to continuously read data from the serial port
thread = threading.Thread(target=read_serial_data, daemon=True)
thread.start()

# Main loop for plotting
try:
    while True:
        # Update the plot
        if len(data) > 0:
            line.set_ydata(data)
            line.set_xdata(range(len(data)))

            # Adjust plot limits
            ax.relim()
            ax.autoscale_view()

            # Redraw the plot
            fig.canvas.flush_events()

        #time.sleep(0.1)  # Adjust this sleep time to control update rate

except KeyboardInterrupt:
    print("Exiting program.")
finally:
    ser.close()
    plt.ioff()  # Turn off interactive mode
    plt.show()  # Display the final plot
