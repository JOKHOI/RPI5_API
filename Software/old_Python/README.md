 "# ELE795-PFE/Software" 




Real-Time Data Handling and Visualization System for STM32 Microcontroller
This project is designed for real-time data collection, storage, and visualization from an STM32 microcontroller. Initially using simulated data for development and testing, the system is ultimately intended to handle real sensor data or inputs from an STM32 device. Python's threading capabilities are utilized to manage multiple tasks concurrently, ensuring efficient data processing and user interaction.

Features
STM32 Integration: Ready to connect with STM32 microcontroller devices for real-world data collection.
CSV Logging: Continuously reads data from the STM32 device and logs it into a CSV file for future analysis or review.
Keyboard Interaction: Listens for keyboard events, allowing users to interact with the system in real-time.
Real-Time Plotting: Visualizes incoming data in real-time using a rolling window approach, ideal for monitoring and immediate analysis.
Components
The system comprises several key components:

listen_for_keys: A function from interfaceGraphique.control module that listens for keyboard events : 

"Space" to stop acquisition
"c" to start acquisition
"r" to reformat data source file(ttyBLE).


read_from_device_and_write_to_csv: A function from gestionnaireCSV.csvHandler module that reads data from the STM32 device and writes it to a CSV file.
ContinuousPlotter and read_data_continuous: Functions from interfaceGraphique.interfaceGraphique module responsible for the real-time plotting of data.
Getting Started
To get the system up and running with STM32 microcontroller data, follow these steps:

Prerequisites: Ensure you have Python installed on your system. This project was developed with Python 3.8. Dependencies include matplotlib for plotting and threading for concurrent execution.

Installation:

Clone the repository to your local machine.
Install required Python packages using pip install -r requirements.txt (Ensure you have requirements.txt file listing all necessary packages).
Configuration:

Constants like CSV_FILE_PATH and DATA_SOURCE are defined in the constant.py file. Adjust these as necessary to suit your STM32 device and environment.
Execution:

Connect your STM32 microcontroller to your RPI5.
Run the main script with python main.py to start the system.
The system will start reading data from your STM32 device, logging data to a CSV file, and plotting the data in real-time.
Transitioning from Simulation to Real Data
The current system setup includes a simulation component for development and testing purposes. To transition to using real data from your STM32 device, ensure the data reading function (read_data_continuous) is adapted to communicate with your STM32 hardware interface.



