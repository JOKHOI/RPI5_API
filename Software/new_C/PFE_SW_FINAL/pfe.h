#ifndef PFE_H
#define PFE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>

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

// Prototypes
void activateAcquisition(void);
void deactivateAcquisition(void);
void setSamplingRate(ConfigureADS1299 *self);
void setGain(ConfigureADS1299 *self);
void selectChannel(ConfigureADS1299 *self);
void setEnableBias(ConfigureADS1299 *self);
void sendADSMessageModification(ConfigureADS1299 *self);
void sendBiasEnableMessageModification(ConfigureADS1299 *self);
void sendSamplingRateMessageModification(ConfigureADS1299 *self);
void printLastLineOfFile(const char *file_path);
void clearFile(const char *file_path);

#endif // PFE_H
	
