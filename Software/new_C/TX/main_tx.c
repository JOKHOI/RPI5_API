#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>

#define TEMP_FILE "/tmp/ttyBLE"

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
    tcgetattr(STDIN_FILENO, oldt); // save old settings
    newt = *oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreTerminal(struct termios* oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

int main() {
    struct termios oldt;
    fd_set set;
    struct timeval timeout;

    // Configure terminal
    configureTerminal(&oldt);

    printf("Press 's' to start, 'r' to reset, and c to stop simulation. Press 'q' to quit.\n");

    while (1) {
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
                default:
                    break;
            }
        }
    }

    // Restore terminal settings
    restoreTerminal(&oldt);

    return 0;
}
