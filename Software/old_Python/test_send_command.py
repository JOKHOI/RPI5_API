from communicationProtocol.messageBuilder import ConfigureADS1299 
from constant import TEMP_FILE
from communicationProtocol.messageBuilder import printLastLineOfFile,clearFile
    # Example usage
file_path = 'path_to_your_file.txt'  # Replace with your actual file path

if __name__ == "__main__":
        clearFile(file_path)

def _test():
        
        clearFile(TEMP_FILE)
        
        # init ads api
        ads = ConfigureADS1299()    
        ads.configure(sampling_rate=1000)
        ads.configure(n_channel=1,gain=24)

        ads.sendSamplingRateMessageModification()
        #ads.sendADSMessageModification()

        # Now verify the last written message
        printLastLineOfFile(TEMP_FILE)

if __name__ == "__main__":
        _test()