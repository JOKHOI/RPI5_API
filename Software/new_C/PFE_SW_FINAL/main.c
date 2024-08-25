#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include <stdarg.h>


#define TEMP_FILE "/tmp/ttyBLE"

// Function declarations
void writeConfig(const char* command);
char* activateAcquisition();
char* deactivateAcquisition();
char* resetDevice();
void configureTerminal(struct termios* oldt);
void restoreTerminal(struct termios* oldt);
void verbosePrint(const char *format, ...); // for conditional verbose output

int verbose = 0; // Global flag to control verbose output

int main(int argc, char *argv[]) {
    const char *devicePath = "/tmp/ttyBLE";
    char *outputFileName = "output.csv";
    FILE *outputFile = NULL;
    int outputToFile = 0;
    unsigned int sampleNumber = 0;  // Counter for the sample number
    struct termios oldt;
    fd_set set;
    struct timeval timeout;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            outputToFile = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            outputFileName = argv[++i]; // Use the next argument as the file name
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1; // Enable verbose mode
        }
    }

    // Open output file if needed
    if (outputToFile) {
        outputFile = fopen(outputFileName, "w");
        if (!outputFile) {
            perror("Failed to open output file");
            return EXIT_FAILURE;
        }
    }

    int fd = open(devicePath, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    // Configure terminal for non-blocking input
    configureTerminal(&oldt);
    if (verbose) {
        printf("Press 's' to start, 'r' to reset, 'c' to stop, and 'q' to quit.\n");
    }

    while (1) {
        // Non-blocking read from device
        uint16_t part1, part2;
        ssize_t bytesRead;
        bytesRead = read(fd, &part1, sizeof(part1));
        if (bytesRead > 0) {
            bytesRead = read(fd, &part2, sizeof(part2));
            if (bytesRead > 0) {
                uint32_t value = ((uint32_t)part2 << 16) | part1;
                verbosePrint("Received 32-bit value: %u\n", value);
                if (outputToFile) {
                    fprintf(outputFile, "%u,%u\n", sampleNumber++, value);
                    fflush(outputFile);
                }
            }
        }

        // Non-blocking command input
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 0.1 seconds
        if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) == 1) {
            char c = getchar();
            if (c == 'q') break; // Quit
            switch (c) {
                case 's':
                    writeConfig(activateAcquisition());
                    printf("Simulation started\n");
                    break;
                case 'r':
                    writeConfig(resetDevice());
                    printf("Simulation reset\n");
                    break;
                case 'c':
                    writeConfig(deactivateAcquisition());
                    printf("Simulation stopped\n");
                    break;
            }
        }
    }

    // Cleanup
    if (outputToFile && outputFile) {
        fclose(outputFile);
    }
    close(fd);
    restoreTerminal(&oldt);

    return 0;
}

void verbosePrint(const char *format, ...) {
    va_list args;
    if (verbose) {
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void writeConfig(const char* command) {
    FILE *file = fopen(TEMP_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
    } else {
        perror("Failed to open device file");
    }
}

char* activateAcquisition() {
    return "$RPI,STC,START,0x01";
}

char* deactivateAcquisition() {
    return "$RPI,STC,STOP,0x00";
}

char* resetDevice() {
    return "$RPI,STC,RESET,0xff";
}

void configureTerminal(struct termios* oldt) {
    struct termios newt;
    tcgetattr(STDIN_FILENO, oldt); // Save old settings
    newt = *oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreTerminal(struct termios* oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}
