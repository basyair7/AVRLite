/**
 * @file example2.cpp
 * @brief Basic PWM ANALOGWRITE
 * This example demonstrates the use of PWM (Pulse Width Modulation) for controlling the brightness of LEDs.
 * The LEDs are connected to pins D3, D5, and D6.
 * The brightness of each LED is gradually increased and then decreased in a loop.
 * The delay between each step is controlled by the sleepMicroseconds() function.
 * 
 * @details
 * - Pins D3, D5, and D6 are initialized as OUTPUT.
 * - The LEDs connected to these pins are controlled using PWM.
 * - The brightness of the LEDs is varied from 0 (off) to 255 (full brightness) and back to 0.
 * - The interval between each step is 500 microseconds.
 * 
 * @date 2024
 * 
 * @autor basyair7
 */

#include "AVRLite.h"

// Define the pins connected to the LEDs
const uint16_t LED_PINS[3] = {D3, D5, D6};

int main() {
    // Setup: Initialize each LED pin as OUTPUT
    for (const auto pin : LED_PINS) {
        GPIOControl(pin, OUTPUT);
    }

    // Constant interval for delay in microseconds
    const uint32_t interval = 500;

    // Infinite loop to continuously vary the LED brightness
    while (1) {
        // Loop through each LED pin
        for (const auto pin : LED_PINS) {
            // Gradually increase the brightness
            for (int i = 0; i <= 255; i++) {
                GPIOControl(pin, ANALOGWRITE, i); // Set PWM duty cycle
                sleepMicroseconds(interval); // Wait for the defined interval
            }

            // Gradually decrease the brightness
            for (int i = 255; i >= 0; i--) {
                GPIOControl(pin, ANALOGWRITE, i); // Set PWM duty cycle
                sleepMicroseconds(interval); // Wait for the defined interval
            }
        }
    }

    // Return statement, though never reached due to infinite loop
    return 0;
}
