import time
import serial
import time
import sys
import os
import unittest
# Adjust the system path to include the parent directory of communicationProtocol
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, parent_dir)

# Now you can import from communicationProtocol.messageBuilder
from communicationProtocol.messageBuilder import ConfigureSTM32

ACTIVATE = ConfigureSTM32().activateAcquisition()
DEACTIVATE = ConfigureSTM32().deactivateAcquisition()

# Adjust the system path to include the parent directory of communicationProtocol
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, parent_dir)

# Now you can import from communicationProtocol.messageBuilder
from communicationProtocol.messageBuilder import ConfigureSTM32

ACTIVATE = ConfigureSTM32().activateAcquisition()
DEACTIVATE = ConfigureSTM32().deactivateAcquisition()



def read_data_continuous(serial_port):
    # Open the serial port with the specified baud rate and timeout
    with serial.Serial(serial_port, baudrate=115200, timeout=0.5) as ser:
        while True:
            line_bytes = ser.readline()  # Read a line from the serial port
            if not line_bytes:
                break  # End of data or timeout, exit the loop
            try:
                yield (line_bytes)

            except ValueError as e:
                # If a ValueError occurs, print the error and skip the line
                print(f"Error processing line: {line_bytes}. Error: {e}")
                continue

def write_to_csv(serial_port, csv_file_path):
    while True:
        with open(csv_file_path, 'a') as csv_file:
            for components in read_data_continuous(serial_port):
                csv_line = ''.join(map(str, components)) + '\n'
                print(csv_line)
                csv_file.write(csv_line)
                csv_file.flush()  # Ensure data is immediately written to disk
