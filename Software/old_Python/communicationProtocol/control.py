from pynput.keyboard import Key, Listener
import sys
import os

# Add the directory containing messageBuilder.py to sys.path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

from messageBuilder import ConfigureSTM32

#from messageBuilder import ConfigureADS1299

Activate = ConfigureSTM32().activateAcquisition()
Deactivate = ConfigureSTM32().deactivateAcquisition()


def write_command_to_file(command, temp_file):
    with open(temp_file, 'a') as file:
        file.write(f"{command}\n")

def on_press(key, temp_file):
    if key == Key.space:
        print("Space pressed - toggling simulation")
        write_command_to_file(Deactivate, temp_file)  # Example command, toggle this with 'start' as needed
    elif hasattr(key, 'char') and key.char == 'c':
        print("C pressed - starting simulation")
        write_command_to_file(Activate, temp_file)
    elif hasattr(key, 'char') and key.char == 'r':
        print("R pressed - resetting simulation")
        write_command_to_file('reset', temp_file)

def listen_for_keys(temp_file):
    def on_press_wrapper(key):
        return on_press(key, temp_file)
    with Listener(on_press=on_press_wrapper) as listener:
        listener.join()

