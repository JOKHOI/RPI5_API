import datetime

TEMP_FILE = "/tmp/ttyBLE"
#TEMP_FILE = "./test.txt"
""" ADS1299 registers map """
REG_CONFIG1 = 0x01
REG_CONFIG2 = 0x02
REG_CONFIG3 = 0x03
REG_MISC = 0x15
REG_BIAS_SENSP = 0x0D
REG_BIAS_SENSN = 0x0E
MAX_NB_CHANNELS = 8
""" constante"""
DEFAULT_SAMPLING_RATE = 500
DEFAULT_GAIN = 24
DEAFAULT_CHANNEL = 1

class ConfigureSTM32:
    def __init__(self):
        pass

    def activateAcquisition(self):
        timestamp = datetime.datetime.now().strftime("%H%M%S")
        msg = f"$RPI,STC,{timestamp},0x01,0,0,0,\n" 
        return msg

    def deactivateAcquisition(self):
        timestamp = datetime.datetime.now().strftime("%H%M%S")
        msg = f"$RPI,STC,{timestamp},0x00,0,0,0,\n" 
        return msg
    


class ConfigureADS1299:
    def __init__(self):
        # Device configuration
        self.n_channel = 1  # {1-8}
        self.gain = 24  # {1,2,4,6,8,12,24}
        self.sampling_rate = 500  # {250,500,1000,2000,4000}
        self.bias_enabled = False  # {True, False}
    
        # True when a data stream is active
        self.stream_active = False

        # Temporary register values for configuration
        self.temp_sampling_rate_reg_value = 0x00
        self.temp_gain_reg_value = 0x00
        self.temp_channel_reg_value = 0x00
        self.temp_bias_enable_reg_value = 0x00

    def configure(self, n_channel=None, sampling_rate=None, bias_enabled=None,gain=None):
        ConfigureSTM32().deactivateAcquisition()  # Make sure ConfigureSTM32 is defined/imported correctly
        if n_channel is not None:
            self.n_channel = n_channel
            self.selectChannel()  # Update channel register value

        if sampling_rate is not None:
            self.sampling_rate = sampling_rate
            self.setSamplingRate()  # Update sampling rate register value
        
        if gain is not None:
            self.gain = gain
            self.setGain()

        if bias_enabled is not None:
            self.bias_enabled = bias_enabled
            self.setEnableBias()

    def setEnableBias(self):
        if self.bias_enabled == True:
            self.temp_bias_enable_reg_value = 0x07



    def setSamplingRate(self):
        """Set CONFIG1 register, which defines the sampling rate."""
        # Mapping of sampling rates to their corresponding register values
        sampling_rate_to_reg_value = {
            2000: 0x03,
            1000: 0x04,
            500: 0x05
        }

        # Check if the sampling rate is in the allowed values and set the register value
        if self.sampling_rate in sampling_rate_to_reg_value:
            self.temp_sampling_rate_reg_value = sampling_rate_to_reg_value[self.sampling_rate]
        else:
            # Print an error message if the sampling rate is not one of the allowed values
            print(f"Error: Invalid sampling rate ({self.sampling_rate}). Allowed values are 500, 1000, 2000.")

    def selectChannel(self):
        self.temp_channel_value = 0x05 #default to channel 1
        
        if self.n_channel == 1:
            self.temp_channel_reg_value = 0x05

        elif self.n_channel == 2:
            self.temp_channel_reg_value = 0x06

        elif self.n_channel == 3:
            self.temp_channel_reg_value = 0x07

        elif self.n_channel == 4:
            self.temp_channel_reg_value = 0x08

        elif self.n_channel == 5:
            self.temp_channel_reg_value = 0x09

        elif self.n_channel == 6:
            self.temp_channel_reg_value = 0x0A

        elif self.n_channel == 7:
            self.temp_channel_reg_value = 0x0B
        
        elif self.n_channel == 8:
            self.temp_channel_reg_value = 0x0C
        else :
            print("Error: Invalid number of channels. Allowed values are 1-8.")


    def setGain(self):
        # Mapping of gain values to their corresponding register values
        gain_to_reg_value = {
            1: 0x00,
            2: 0x01,
            4: 0x02,
            6: 0x03,
            8: 0x04,
            12: 0x05,
            24: 0x06
        }

        # Check if the gain is in the allowed values and set the register value
        if self.gain in gain_to_reg_value:
            self.temp_gain_reg_value = gain_to_reg_value[self.gain]
        else:
            # Print an error message if the gain is not one of the allowed values
            print(f"Error: Invalid gain value ({self.gain}). Allowed values are 1, 2, 4, 6, 8, 12, 24.")

    def sendADSMessageModification(self):
        timestamp = datetime.datetime.now().strftime("%H%M%S")
        msg = f"$RPI,A9C,{timestamp},{self.temp_channel_reg_value},{self.temp_gain_reg_value}\n"
        with open(TEMP_FILE, 'a') as file:
            file.write(f"{msg}")
        return msg

    def sendSamplingRateMessageModification(self):
        timestamp = datetime.datetime.now().strftime("%H%M%S")
        msg =f"$RPI,A9C,{timestamp},{REG_CONFIG1},{self.temp_sampling_rate_reg_value}\n"
        with open(TEMP_FILE, 'a') as file:
            file.write(f"{msg}")
        return msg

    def sendBiasEnableMessageModification(self):
        timestamp = datetime.datetime.now().strftime("%H%M%S")
        # Use a list for messages to maintain order and allow indexing
        msg = [
            f"$RPI,A9C,{timestamp},{REG_BIAS_SENSN},{self.temp_bias_enable_reg_value}\n",
            f"$RPI,A9C,{timestamp},{REG_BIAS_SENSP},{self.temp_bias_enable_reg_value}\n",
            f"$RPI,A9C,{timestamp},{REG_CONFIG3},0xEC\n"  # Removed the f-string for 0xEC
        ]
        
        # Open TEMP_FILE in append mode and write all messages
        with open(TEMP_FILE, 'a') as file:
            for message in msg:  # Iterate over all messages in the list
                file.write(message)
        
        return msg  # Return the list of messages

#========================TEST==================================
def printLastLineOfFile(file_path):
    """Print the last line of a file specified by file_path."""
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
            if lines:  # Check if the list of lines is not empty
                print("Last line in the file:", lines[-1].strip())  # Strip to remove any trailing newline characters
            else:
                print("The file is empty.")
    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")

def clearFile(file_path):
    """Clear the contents of the specified file."""
    try:
        # Open the file in write mode, which automatically clears its contents
        with open(file_path, 'w'):
            pass  # No need to do anything, opening in 'w' mode clears the file
        print(f"The file '{file_path}' has been cleared.")
    except Exception as e:
        print(f"Error occurred while clearing the file: {e}")

