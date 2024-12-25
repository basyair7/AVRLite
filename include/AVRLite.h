#ifndef AVRLite_h
#define AVRLite_h

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

// Definitions for HIGH and LOW states
#define LOW          0x0
#define HIGH         0x1

// Definitions for INPUT and OUTPUT modes
#define INPUT        0x2
#define OUTPUT       0x3

// Definitions for GPIO state
#define ANALOGREAD   0x5
#define ANALOGWRITE  0x6
#define DIGITALREAD  0x7

// Definitions for digital pins D0 to D13
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

// Definitions for analog pins A0 to A5
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

#ifdef __cplusplus
extern "C" {
#endif
// Return the number of milliseconds since the program started
unsigned long uptimeMs();

// Return the number of microseconds since the program started
unsigned long uptimeUs();

// sleep for a specified number of milliseconds
void sleep(unsigned long ms);

// Sleep for a specified number of microseconds
void sleepMicroseconds(unsigned int us);

// Configure pin mode (INPUT or OUTPUT)
void GPIOInit(uint8_t pin, uint8_t mode);

// Overloaded versions of GPIOControl for DIGITALREAD and ANALOGREAD without mode parameter
int GPIORead(uint8_t pin, uint8_t state);
// Write to a digital pin
void GPIOWrite(uint8_t pin, uint8_t mode, uint8_t value = 0);
// Control GPIO states
int GPIOControl(uint8_t pin, uint8_t mode, uint8_t value = 0);

// Initialize Serial communication
void Serial_begin(unsigned long baud);
// Write data to Serial
void Serial_write(const char* str);

// Write data to Serial with newline conversion
void Serial_print(const char* str);
// Write data to Serial with newline
void Serial_println(const char* str = "");
// FOrmatted print to Serial with dynamic buffer
void Serial_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
