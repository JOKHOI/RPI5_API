#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char *devicePath = "/tmp/ttyBLE";
    char *outputFileName = "output.csv";
    FILE *outputFile = NULL;
    int outputToFile = 0;
    unsigned int sampleNumber = 0;  // Counter for the sample number

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            outputToFile = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            outputFileName = argv[++i]; // Use the next argument as the file name
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

    uint16_t part1, part2;
    ssize_t bytesRead;

    while (1) {
        bytesRead = read(fd, &part1, sizeof(part1));
        if (bytesRead <= 0) break; // Exit loop on error or no data
        
        bytesRead = read(fd, &part2, sizeof(part2));
        if (bytesRead <= 0) break; // Exit loop on error or no data
        
        uint32_t value = ((uint32_t)part2 << 16) | part1;
        printf("Received 32-bit value: %u\n", value);

        if (outputToFile) {
            fprintf(outputFile, "%u,%u\n", sampleNumber++, value);
            fflush(outputFile); // Flush after each write
        }
    }

    if (outputToFile) {
        fclose(outputFile);
    }
    close(fd);
    return 0;
}
