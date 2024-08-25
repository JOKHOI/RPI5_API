// configure_system.h

#ifndef CONFIGURE_SYSTEM_H
#define CONFIGURE_SYSTEM_H

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

// Function prototypes

void setSamplingRate(ConfigureADS1299 *self);
void setGain(ConfigureADS1299 *self);
void selectChannel(ConfigureADS1299 *self);
void setEnableBias(ConfigureADS1299 *self);
void sendADSMessageModification(ConfigureADS1299 *self);
void sendBiasEnableMessageModification(ConfigureADS1299 *self);
void sendSamplingRateMessageModification(ConfigureADS1299 *self);
void printLastLineOfFile(const char *file_path);
void clearFile(const char *file_path);

#endif // CONFIGURE_SYSTEM_H
