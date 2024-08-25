from gestionnaireCSV.csvHandler import write_to_csv  
from interfaceGraphique.interfaceGraphique import ContinuousPlotter, read_data_continuous  
from constant import CSV_FILE_PATH, TEMP_FILE, DATA_SOURCE






def main():
    # Assuming you have initialized the plotter with a window of 100 data points
    plotter = ContinuousPlotter(window=100)

    # Specify the path to your temporary file where the data is being written
    data_source = CSV_FILE_PATH
    
    # Start the plotter
    plotter.start(data_source)

if __name__ == "__main__":
    main()

