#ifndef EMAGLOBAL_H
#define EMAGLOBAL_H

////////////////////////////////////////////////////////////////////////////////////////
// Return values
////////////////////////////////////////////////////////////////////////////////////////

#define EMA_SUCCESS                                0

#define EMA_ERROR_WAIT_TIMEOUT                    -1  // Timeout Error

#define EMA_ERROR_READ_EVENT_CANCELLED            -2

#define EMA_ERROR_INVALID_HANDLE                  -3  // Invalid EMA_HANDLE in argument

#define EMA_ERROR_CANNOT_ALLOC_MEMORY             -4

#define EMA_ERROR_OPEN_FAILED                     -5

#define EMA_ERROR_ALREADY_OPENED                  -6

#define EMA_ERROR_ALREADY_CLOSED                  -7

#define EMA_ERROR_WRITE_FAILED                    -8

#define EMA_ERROR_NO_INPUT_DATA                   -9

#define EMA_ERROR_READ_FAILED                     -10

#define EMA_ERROR_READ_SIZE_MISMATCH              -11

#define EMA_ERROR_IN_ARGUMENTS                    -12

#define EMA_ERROR_WRONG_WRITE_EVENT_TYPE          -13

#define EMA_ERROR_WRONG_READ_EVENT_TYPE           -14

#define EMA_ERROR_WRONG_EVENT_FORMAT              -15

#define EMA_ERROR_WIEGAND_NOT_CONFIGURED          -16

#define EMA_ERROR_WIEGAND_NUMOFBITS_MISMATCH      -17

#define EMA_ERROR_WIEGAND_INVALID_NUMOFBITS       -18

#define EMA_ERROR_WIEGAND_INVALID_PULSEWIDTH      -19

#define EMA_ERROR_WIEGAND_INVALID_PULSEINTERVAL   -20

#define EMA_ERROR_WIEGAND_AUTO_DETECTION_FAILED   -21

#define EMA_ERROR_WIEGAND_READ_SIZE_MISMATCH      -22

#define EMA_ERROR_GPIO_INVALID_VALUE              -23

#define EMA_ERROR_WIEGAND_OUT_INVALID_CHANNEL     -24

//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Event Type for EMA_EVENT structure
////////////////////////////////////////////////////////////////////////////////////////

#define EMA_EVENT_TYPE_CLOSE                         0

#define EMA_EVENT_TYPE_OPEN                          1

#define EMA_EVENT_TYPE_WIEGAND_READ_DATA             2

#define EMA_EVENT_TYPE_WIEGAND_WRITE_DATA            3

#define EMA_EVENT_TYPE_WIEGAND_SET_CONFIG            4

#define EMA_EVENT_TYPE_WIEGAND_GET_CONFIG            5

#define EMA_EVENT_TYPE_WIEGAND_AUTO_DETECT           6


#define EMA_EVENT_TYPE_GPI12_READ                    16    // GPI1 & GPI2 read

#define EMA_EVENT_TYPE_GPO1_WRITE                    17    // GPO1 write

#define EMA_EVENT_TYPE_GPO2_WRITE                    18    // GPO2 write

#define EMA_EVENT_TYPE_GPO12_WRITE                   19    // GPO1 & GPO2 write

#define EMA_EVENT_TYPE_RELAY_WRITE                   32

#define EMA_EVENT_TYPE_USB_POWER_OFF                 64    // USB Power Off to reset

#define EMA_EVENT_TYPE_USB_POWER_ON                  65    // USB Power On after reset

// GPIO Value
#define EMA_EVENT_VALUE_GPIO_HIGH                     1
#define EMA_EVENT_VALUE_GPIO_LOW                      0

// Relay Value
#define EMA_EVENT_VALUE_RELAY_ABNORMAL                1    // make COM to connect to NO(Normal Open)
#define EMA_EVENT_VALUE_RELAY_NORMAL                  0    // make COM to connect to NC(Normal Connect)
                                                           // default position

////////////////////////////////////////////////////////////////////////////////////////
// Wiegand return type
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

#define WIEGAND_MAX_DATA_SIZE                         33    // bytes, 33x8 = 264 bits

typedef void *EMA_HANDLE;

typedef struct _EMA_EVENT {
    int cbSize;
    int eventType;
    // Wiegand
    unsigned char  wiegandOutChannel;          // EMA: 1, ECU: 1 and 2 selectable.
    unsigned short numOfBits;                  // 26bits, 34bits, .. up to 272bits
    unsigned short pulseWidth;                 // usec
    unsigned short pulseInterval;              // usec  
    unsigned char  wiegandData[WIEGAND_MAX_DATA_SIZE];
    // GPIO
    unsigned char gpo1Value;                   // General Purpose Output 1
    unsigned char gpo2Value;                   // General Purpose Output 2
    unsigned char gpi1Value;                   // General Purpose Input 1
    unsigned char gpi2Value;                   // General Purpose Input 2
    // Relay
    unsigned char relayValue;
    unsigned char relayInterval;               // after "relayInterval" x 0.1sec,
                                               // the relay will return to previous state
                                               // if relayInterval == 0, then the relay will
                                               // not return to previous state.
} EMA_EVENT;

#endif // EMAGLOBAL_H
