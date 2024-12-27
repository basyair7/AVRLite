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
#define ANALOGREAD   0xA0
#define ANALOGWRITE  0xA1
#define DIGITALREAD  0xD

// Definitions for analog pins A0 to A5
#define A0 0xE
#define A1 0xF
#define A2 0x10
#define A3 0x11
#define A4 0x12
#define A5 0x13

// Definitions for digital pins D0 to D19
#define D0  0x0
#define D1  0x1
#define D2  0x2
#define D3  0x3
#define D4  0x4
#define D5  0x5
#define D6  0x6
#define D7  0x7
#define D8  0x8
#define D9  0x9
#define D10 0xA
#define D11 0xB
#define D12 0xC
#define D13 0xD
#define D14 A0
#define D15 A1
#define D16 A2
#define D17 A3
#define D18 A4
#define D19 A5

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
int GPIOInit(uint8_t pin, uint8_t mode);

// Overloaded versions of GPIOControl for DIGITALREAD and ANALOGREAD without mode parameter
int GPIORead(uint8_t pin, uint8_t state);
// Write to a digital pin
uint8_t GPIOWrite(uint8_t pin, uint8_t mode, uint8_t value = 0);
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
