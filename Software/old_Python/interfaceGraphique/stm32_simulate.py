import time
import random
import sys
import os

# Add the directory containing messageBuilder.py to sys.path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

from constante import ACTIVATE, DEACTIVATE
def check_last_line(temp_file):
    with open(temp_file, "r") as file:
        lines = file.readlines()
        if lines:
            return lines[-1].strip()
    return ""

def simulate_st32(temp_file):
    start_time = time.time()
    simulating = True  # Start with simulation enabled

    with open(temp_file, "w") as write_file:
        while True:
            command = check_last_line(temp_file)

            if command == DEACTIVATE:
                simulating = False
            elif command == ACTIVATE:
                simulating = True
            elif command == "reset":
                write_file.seek(0)
                write_file.truncate()
                start_time = time.time()  # Reset the start time
                simulating = True  # Resume simulation after reset

            if simulating:
                elapsed_time = int((time.time() - start_time) * 1000)  # Convert to milliseconds
                random_int = random.randint(0, 3)  # Generate a random integer between 0 and 3
                voltage_binary = format(random_int, 'b').zfill(2)  # Convert to a binary string with at least 2 digits
                write_file.write(f"{elapsed_time},{voltage_binary}\n")
                write_file.flush()  # Ensure data is written to the file
            time.sleep(0.1)  # Adjust the sleep time as needed
