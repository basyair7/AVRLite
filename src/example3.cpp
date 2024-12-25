/**  
 * @file example3.cpp
 * @brief Basic Flip-Flop
 * This example demonstrates a simple flip-flop (toggle) operation.
 * An LED connected to pin D13 is turned on and off with a delay of 1 second.
 * 
 * @details
 * - The LED is toggled by setting the pin HIGH to turn it on and LOW to turn it off.
 * - The `sleep` function is used to introduce a delay of 1000 milliseconds (1 second) between toggles.
 * 
 * @date 2024
 * @autor basyair7
 */

#include "AVRLite.h"  // Include the necessary header for AVR-based microcontroller operations

// Main program entry point
int main() {
    // Initialize pin D13 as OUTPUT
    GPIOControl(D13, OUTPUT);

    // Infinite loop
    for (;;) {
        // Set pin D13 to HIGH (turn LED on)
        GPIOControl(D13, HIGH);
        
        // Wait for 1000 milliseconds (1 second)
        sleep(1000);
        
        // Set pin D13 to LOW (turn LED off)
        GPIOControl(D13, LOW);
        
        // Wait for 1000 milliseconds (1 second)
        sleep(1000);
    }

    // Return statement, though never reached due to infinite loop
    return 0;
}
