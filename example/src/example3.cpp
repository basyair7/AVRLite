/**
 *  @file example3.cpp
 *  @brief An example program to control three LEDs connected to pins A0 (D14), A1 (D15), and A2 (D16) on an AVR microcontroller.
 *  
 *  This program initializes the specified LED pins as output and cycles through each LED with different on-off intervals. 
 *  The mode of operation changes after a set number of iterations, and the LEDs will light up in a sequence, both forward and backward.
 */

#include "AVRLite.h"

// Array to store the pin numbers for the LEDs
const uint16_t LED[3] = {D14, D15, D16}; // A0, A1, A2 pinout

int main() {
    // Initialize each LED pin as an output
    for (size_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); ++i) {
        GPIOControl(LED[i], OUTPUT);
    }

    const unsigned long interval[3] = {1000, 500, 100};  // Different intervals for each mode
    bool direction = true;  // `true` for forward, `false` for backward
    uint8_t mode = 0;  // Initialize mode counter
    uint8_t counter = 0;  // Counter to switch between modes

    while (1) {
        // Loop through each LED
        for (size_t i = 0; i < (sizeof(LED)/sizeof(LED[0])); ++i) {
            GPIOControl(LED[i], HIGH);  // Turn on the current LED
            sleep(interval[mode]);  // Adjust delay based on the current mode
            GPIOControl(LED[i], LOW);  // Turn off the current LED
        }

        // Increment counter to track iterations
        counter++;
        
        // Change mode every 5 iterations (adjust as needed)
        if (counter >= 5) {
            counter = 0;  // Reset counter

            // Update mode based on direction
            if (direction) {
                mode++;  // Move to the next mode
                if (mode == 2) {
                    direction = false;  // Reverse direction when at the last mode
                }
            } 
            else {
                mode--;  // Move to the previous mode
                if (mode == 0) {
                    direction = true;  // Change direction to forward when at the first mode
                }
            }
        }

        // Short delay to ensure smooth operation
        sleepMicroseconds(50);
    }

    return 0;
}
