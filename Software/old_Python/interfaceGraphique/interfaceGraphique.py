import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import sys
import os

# Add the directory containing your constants or configurations
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
from constante import ACTIVATE, DEACTIVATE  # Import constants

def read_data_continuous(temp_file):
    """Generator to continuously read time-voltage data from a file."""
    with open(temp_file, "r") as file:
        while True:
            line = file.readline()
            if not line or line.strip().lower() in [ACTIVATE, DEACTIVATE, 'reset']:
                time.sleep(0.1)
                continue

            try:
                parts = line.strip().split(',')
                elapsed_time = int(parts[0])
                voltage = int(parts[1])  # Directly read the voltage as an integer
                yield (elapsed_time, voltage)
            except ValueError as e:
                print(f"Error processing line: {line}. Error: {e}")
                continue

class ContinuousPlotter:
    def __init__(self, window=100):
        """Setup the initial plotting area and data structures."""
        self.fig, self.ax = plt.subplots()
        self.line, = self.ax.plot([], [], lw=2)
        self.window = window
        self.xdata, self.ydata = deque(maxlen=window), deque(maxlen=window)
        self.max_voltage = 0  # Initialize max voltage to zero

        self.ax.set_xlabel('Elapsed Time (ms)')
        self.ax.set_ylabel('Voltage (V)')

    def init(self):
        """Initialize the limits for the plot."""
        self.ax.set_ylim(0, 100)  # Start with a default small range that will be updated
        self.ax.set_xlim(0, self.window)
        self.line.set_data(self.xdata, self.ydata)
        return self.line,

    def update(self, data):
        """Update the plot with new data."""
        elapsed_time, voltage = data
        self.xdata.append(elapsed_time)
        self.ydata.append(voltage)

        # Update the maximum voltage seen so far
        self.max_voltage = max(self.max_voltage, voltage)
        self.ax.set_ylim(0, self.max_voltage + self.max_voltage * 0.1)  # Add 10% headroom

        self.line.set_data(self.xdata, self.ydata)

        xmin = max(0, elapsed_time - self.window * 50)
        xmax = elapsed_time + (self.window * 0.5)
        self.ax.set_xlim(xmin, xmax)

        return self.line,

    def data_gen(self, temp_file):
        """Generator to fetch data continuously."""
        for data in read_data_continuous(temp_file):
            yield data

    def start(self, temp_file):
        """Start the animation using the update function and data generator."""
        self.ani = animation.FuncAnimation(
            self.fig, self.update, lambda: self.data_gen(temp_file),
            blit=True, interval=5, repeat=False, init_func=self.init
        )
        plt.show()

# Example usage:
# plotter = ContinuousPlotter(window=100)
# plotter.start('path_to_your_temp_file.txt')
