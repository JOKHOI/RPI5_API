import sys
import os

# Adjust the system path to include the parent directory of communicationProtocol
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.insert(0, parent_dir)

# Now you can import from communicationProtocol.messageBuilder
from communicationProtocol.messageBuilder import ConfigureSTM32

ACTIVATE = ConfigureSTM32().activateAcquisition()
DEACTIVATE = ConfigureSTM32().deactivateAcquisition()

#DATA SOURCE FOR SIMULATED DATA
TEMP_FILE = "./interfaceGraphique/simulated_data.txt"

