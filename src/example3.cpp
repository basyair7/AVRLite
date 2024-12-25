/**  
 * @file example3.cpp
 * @brief Basic Flip-Flop
 * @author basyair7
 * @date 2024
*/

#include "AVRLite.h"

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
