/**  
 * @file example5.cpp
 * @brief Basic MultiThreading with uptimeMs
 * This example demonstrates basic multitasking by toggling the states of three LEDs at different intervals
 * and controlling the brightness of an LED using PWM.
 * 
 * @details
 * - LED_1 (D13) toggles every 5 seconds.
 * - LED_2 (D12) toggles every 1 second.
 * - LED_3 (D11) fades in and out using PWM with a fade interval of 30 ms.
 * 
 * @date 2024
 * @autor basyair7
 */

#include "AVRLite.h"  // Include the necessary header for AVR-based microcontroller operations

#define LED_1 D13  // Define LED_1 on pin D13
#define LED_2 D12  // Define LED_2 on pin D12
#define LED_3 D11  // Define LED_3 on pin D11

// Variables to track the last uptime and the current brightness state
unsigned long previous_uptimeMs = 0;  // Stores the last update time for PWM functionality (using millis)
int brightness = 0;  // The current brightness of the LED, range from 0 to 255
int fadeAmount = 5;  // Amount by which brightness changes per step

/**
 * @brief Function to control PWM on LED_3, simulating a fading effect.
 * The brightness is incremented or decremented, creating a fade-in and fade-out effect.
 */
void pwmExample() {
    // Check if the elapsed time since the last update is greater than or equal to 30 ms
    if (uptimeMs() - previous_uptimeMs >= 30L) {
        previous_uptimeMs = uptimeMs();  // Update the previous uptime timestamp

        // Increase or decrease the brightness based on the fadeAmount
        brightness += fadeAmount;

        // Reverse the fading direction once brightness reaches the limits (0 or 255)
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;  // Reverse the direction of brightness change
        }

        // Apply the calculated brightness to LED_3 using PWM
        GPIOWrite(LED_3, ANALOGWRITE, brightness);  // This will control the PWM for LED_3
    }
}

int main() {
    // Initialize serial communication with a baud rate of 9600
    Serial_begin(9600);

    // Send a test message to the serial monitor
    Serial_print("Hello ");
    Serial_println("world");

    // Set the pins for LED_1, LED_2, and LED_3 to OUTPUT mode
    GPIOControl(LED_1, OUTPUT);  // or GPIOInit(LED_1, OUTPUT);
    GPIOControl(LED_2, OUTPUT);  // or GPIOInit(LED_2, OUTPUT);
    GPIOControl(LED_3, OUTPUT);  // or GPIOInit(LED_3, OUTPUT);

    // Variables to keep track of last time state changes occurred and the current states of the LEDs
    static unsigned long LastuptimeMs1 = 0, LastuptimeMs2 = 0;  // Stores the last time LED_1 and LED_2 were toggled
    static unsigned long LastuptimeMs_Read = 0;  // Stores the last time LED states were read
    static uint8_t state1 = LOW, state2 = LOW;  // Initial states of LED_1 and LED_2 (LOW = OFF)

    while(1) {
        // Periodically read the states of the LEDs every 500 milliseconds
        if (uptimeMs() - LastuptimeMs_Read >= 500L) {
            LastuptimeMs_Read = uptimeMs();  // Update the timestamp for state read

            // Read the current digital states of the LEDs
            int readState1 = GPIOControl(LED_1, DIGITALREAD);  // or GPIORead(LED_1, DIGITALREAD);
            int readState2 = GPIOControl(LED_2, DIGITALREAD);  // or GPIORead(LED_2, DIGITALREAD);
            int readState3 = GPIOControl(LED_3, DIGITALREAD);  // or GPIORead(LED_3, DIGITALREAD);

            // Output the state of the LEDs to the serial monitor
            Serial_printf("LED 1: %d, LED 2: %d, LED 3: %d\n", readState1, readState2, readState3);
        }

        // Toggle the state of LED_1 every 5000 milliseconds (5 seconds)
        if (uptimeMs() - LastuptimeMs1 >= 5000L) {
            LastuptimeMs1 = uptimeMs();  // Update the timestamp
            state1 = !state1;  // Toggle the state of LED_1 (ON to OFF or OFF to ON)
        }

        // Toggle the state of LED_2 every 1000 milliseconds (1 second)
        if (uptimeMs() - LastuptimeMs2 >= 1000L) {
            LastuptimeMs2 = uptimeMs();  // Update the timestamp
            state2 = !state2;  // Toggle the state of LED_2 (ON to OFF or OFF to ON)
        }

        // Apply the new states to LED_1 and LED_2
        GPIOControl(LED_1, state1);  // or GPIOWrite(LED_1, state1);
        GPIOControl(LED_2, state2);  // or GPIOWrite(LED_2, state2);

        // Run the pwmExample function to control the fading of LED_3
        pwmExample();

        // Small delay to prevent excessive CPU usage (50 milliseconds)
        sleep(5);
    }

    return 0;
}
