#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>  // For bool, true, false

#define TEMP_FILE "/tmp/ttyBLE"
#define REG_CONFIG1 0x01
#define REG_CONFIG2 0x02
#define REG_CONFIG3 0x03
#define REG_MISC 0x15
#define REG_BIAS_SENSP 0x0D
#define REG_BIAS_SENSN 0x0E
#define MAX_NB_CHANNELS 8
#define DEFAULT_SAMPLING_RATE 500
#define DEFAULT_GAIN 24
#define DEFAULT_CHANNEL 1

typedef struct {
    // Empty struct, as original Python class had no member variables
} ConfigureSTM32;

typedef struct {
    int n_channel;
    int gain;
    int sampling_rate;
    bool bias_enabled;
    bool stream_active;
    int temp_sampling_rate_reg_value;
    int temp_gain_reg_value;
    int temp_channel_reg_value;
    int temp_bias_enable_reg_value;
} ConfigureADS1299;

void activateAcquisition(ConfigureSTM32 *self) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("$RPI,STC,%02d%02d%02d,0x01,0,0,0,\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void deactivateAcquisition(ConfigureSTM32 *self) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("$RPI,STC,%02d%02d%02d,0x00,0,0,0,\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void setSamplingRate(ConfigureADS1299 *self) {
    switch (self->sampling_rate) {
        case 2000:
            self->temp_sampling_rate_reg_value = 0x03;
            break;
        case 1000:
            self->temp_sampling_rate_reg_value = 0x04;
            break;
        case 500:
            self->temp_sampling_rate_reg_value = 0x05;
            break;
        default:
            printf("Error: Invalid sampling rate (%d). Allowed values are 500, 1000, 2000.\n", self->sampling_rate);
    }
}

void setGain(ConfigureADS1299 *self) {
    switch (self->gain) {
        case 1:
            self->temp_gain_reg_value = 0x00;
            break;
        case 2:
            self->temp_gain_reg_value = 0x01;
            break;
        case 4:
            self->temp_gain_reg_value = 0x02;
            break;
        case 6:
            self->temp_gain_reg_value = 0x03;
            break;
        case 8:
            self->temp_gain_reg_value = 0x04;
            break;
        case 12:
            self->temp_gain_reg_value = 0x05;
            break;
        case 24:
            self->temp_gain_reg_value = 0x06;
            break;
        default:
            printf("Error: Invalid gain value (%d). Allowed values are 1, 2, 4, 6, 8, 12, 24.\n", self->gain);
    }
}

void selectChannel(ConfigureADS1299 *self) {
    if (self->n_channel >= 1 && self->n_channel <= 8) {
        self->temp_channel_reg_value = 0x04 + self->n_channel;  // Assuming 0x05 corresponds to channel 1
    } else {
        printf("Error: Invalid number of channels (%d). Allowed values are 1-8.\n", self->n_channel);
    }
}

void setEnableBias(ConfigureADS1299 *self) {
    self->temp_bias_enable_reg_value = self->bias_enabled ? 0x07 : 0x00;  // Example value, adjust based on actual requirement
}

void sendADSMessageModification(ConfigureADS1299 *self) {
    FILE *file;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    file = fopen(TEMP_FILE, "a");
    if (file != NULL) {
        fprintf(file, "$RPI,A9C,%02d%02d%02d,%d,%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec,
                self->temp_channel_reg_value, self->temp_gain_reg_value);
        fclose(file);
    } else {
        printf("Error opening file\n");
    }
}

void sendBiasEnableMessageModification(ConfigureADS1299 *self) {
    FILE *file = fopen(TEMP_FILE, "a");
    if (file != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        fprintf(file, "$RPI,A9C,%02d%02d%02d,%d,%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, REG_BIAS_SENSN, self->temp_bias_enable_reg_value);
        fprintf(file, "$RPI,A9C,%02d%02d%02d,%d,%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, REG_BIAS_SENSP, self->temp_bias_enable_reg_value);
        fprintf(file, "$RPI,A9C,%02d%02d%02d,%d,0xEC\n", tm.tm_hour, tm.tm_min, tm.tm_sec, REG_CONFIG3);  // Adjusted for C syntax

        fclose(file);
    } else {
        printf("Error opening file\n");
    }
}

void sendSamplingRateMessageModification(ConfigureADS1299 *self) {
    FILE *file = fopen(TEMP_FILE, "a");
    if (file != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        fprintf(file, "$RPI,A9C,%02d%02d%02d,%d,%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, REG_CONFIG1, self->temp_sampling_rate_reg_value);

        fclose(file);
    } else {
        printf("Error opening file\n");
    }
}

void printLastLineOfFile(const char *file_path) {
    char buffer[256];
    char lastLine[256] = "";
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: The file '%s' was not found.\n", file_path);
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(lastLine, buffer);
    }

    if (feof(file)) {  // Check if the end of file was reached
        printf("Last line in the file: %s", lastLine);
    } else {
        printf("The file is empty or an error occurred.\n");
    }

    fclose(file);
}

void clearFile(const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Error occurred while opening the file.\n");
        return;
    }

    // Opening the file in write mode with "w" flag automatically clears its contents
    fclose(file);
    printf("The file '%s' has been cleared.\n", file_path);
}
