/**
 *  @file example4.cpp
 *  @brief An example program to control three LEDs connected to pins D9, D10, and D11 on an AVR microcontroller.
 *  
 *  This program initializes the specified LED pins as output and cycles through various brightness levels to create a fading effect.
 *  The brightness levels are adjusted based on different time intervals, cycling through multiple modes. The LEDs will fade from 
 *  off to fully lit and back, simulating a smooth breathing effect.
 */

#include "AVRLite.h"

// Array to store the pin numbers for the LEDs
const uint16_t LED[3] = {D9, D10, D11};

int main() {
    // Initialize each LED pin as an output
    for (size_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); ++i) {
        GPIOControl(LED[i], OUTPUT);
    }

    // Array to store different time intervals for fading effects
    const unsigned long interval[4] = {1000, 500, 100, 50};  // Different intervals for each mode
    int16_t fadeAmount = 5;  // Amount to increment or decrement the brightness
    int16_t brightness = 0;  // Current brightness level
    uint8_t counter = 0;  // Counter to switch between modes
    uint8_t mode = 0;  // Initialize mode counter

    while (1) {
        // Loop through each LED
        for (size_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); ++i) {
            // Fade the LED from 0 to 255 and back
            for (brightness = 0; brightness <= 255; brightness += fadeAmount) {
                GPIOWrite(LED[i], ANALOGWRITE, brightness);
                sleep(interval[mode] / 255);  // Adjust delay based on the current mode
            }
            for (brightness = 255; brightness >= 0; brightness -= fadeAmount) {
                GPIOWrite(LED[i], ANALOGWRITE, brightness);
                sleep(interval[mode] / 255);  // Adjust delay based on the current mode
            }
        }

        // Increment counter to track iterations
        counter++;
        
        // Change mode every 10 iterations (adjust as needed)
        if (counter >= 10) {
            counter = 0;  // Reset counter
            mode = (mode + 1) % 4;  // Cycle through modes 0 to 3
        }

        // Short delay to ensure smooth operation
        sleepMicroseconds(50);
    }

    return 0;
}
