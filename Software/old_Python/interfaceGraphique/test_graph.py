import threading
from stm32_simulate import simulate_st32
from interfaceGraphique import read_data_continuous, ContinuousPlotter 
from communicationProtocol.control import listen_for_keys
from constante import TEMP_FILE


# Define the file path for simulated data

#to delete
def generate_data():
    simulate_st32(TEMP_FILE)

def plot_data():
    plotter = ContinuousPlotter(window=100)
    plotter.start(read_data_continuous, TEMP_FILE)

# Create threads for data generation, plotting, and listening for keyboard inputs
data_thread = threading.Thread(target=generate_data)
plot_thread = threading.Thread(target=plot_data)
listener_thread = threading.Thread(target=listen_for_keys, args=(TEMP_FILE,), daemon=True)


# Start the threads
data_thread.start()
plot_thread.start()
listener_thread.start()

# Optionally, join the threads if there's a condition to end the simulation
# data_thread.join()
# plot_thread.join()
