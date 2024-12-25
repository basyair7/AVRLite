# AVRLite Project

## Overview
The AVRLite Project is a lightweight library designed for AVR microcontrollers, particularly the ATmega328P. This project offers core functionalities akin to the [Arduino framework](https://www.arduino.cc), such as GPIO management, timing, and serial communication, while maintaining simplicity and minimal overhead.

## Installation

### Installing `avr-g++` on WSL

1. **Open your WSL terminal**: If you're using Ubuntu, you can open it by searching for "Ubuntu" in the Windows search bar.
2. **Update your package list**: Run the following command to update your package list:
```sh
sudo apt update
```
3. **Install `avr-g++`**: Install the AVR toolchain by running:
```sh
sudo apt install gcc-avr avr-libc binutils-avr avrdude
```
4. **Verify the installation**: Check if `avr-g++` is installed correctly by running:
```sh
avr-g++ --version
```

### Installing `cmake` on WSL

1. **Open your WSL Terminal**
2. **Update your package list**: Run the following command to update your package list:
```sh
sudo apt update
```
3. **Install `cmake`**: Install CMake by running.
```sh
sudo apt install cmake
```
4. **Verify the installation**: Check if `cmake` is installed correctly by running:
```sh
cmake --version
```

## Running CMake on WSL

1. **Create a build directory**: Navigate to your project directory and create a build directory:
```sh
mkdir build
cd build
```
2. **Run CMake**: Run CMake to configure your project:
```sh
cmake ..
```
3. **BUild your project**: Compile your project using the make command:
```sh
make
```

## Files
- `AVRLite.h`: Contains essential functions for GPIO control, timing, and serial communication.
- `main.cpp`: Demonstrates the application of `AVRLite.h`.

## References
- The design and features of the AVRLite library were inspired by the [Arduino framework](https://www.arduino.cc), which provides a versatile development environment for microcontrollers.
- Timing functionalities such as `uptimeUs()` and `uptimeMs()` are based on the Timer overflow mechanisms similar to the Arduino functions [micros()](https://docs.arduino.cc/language-reference/en/functions/time/micros/) and [millis()](https://docs.arduino.cc/language-reference/en/functions/time/millis/).
- Delay functionalities such as `sleep()` and `sleepMicroseconds()` are based on the Arduino functions [delay()](https://docs.arduino.cc/language-reference/en/functions/time/delay/) and [delayMicroseconds()](https://docs.arduino.cc/language-reference/en/functions/time/delayMicroseconds/).
- Detailed technical explanations for timer overflow mechanisms and other low-level operations can be found in the [AVR Libc Documentation](https://www.nongnu.org/avr-libc/user-manual/).
- Serial communication concepts refer to the [Serial Communication](https://www.arduino.cc/reference/en/language/functions/communication/serial/) documentation on the Arduino website.

## AVRLite.h

### Functions
1. **`GPIOControl(uint8_t pin, uint8_t mode = LOW)`**:
   - Manages GPIO states and operations including configuring pin modes, writing to digital pins, and reading from pins.
   - **Parameters**:
     - `pin`: Pin number to operate on.
     - `mode`: Operation to perform (HIGH, LOW, INPUT, OUTPUT, DIGITALREAD, ANALOGREAD).
   - **Returns**:
     - For DIGITALREAD and ANALOGREAD: Returns the read value.

2. **`GPIOInit(uint8_t pin, uint8_t mode)`**:
   - Configures the pin mode (INPUT or OUTPUT).
   - **Parameters**:
      - `pin`: Pin number to configure.
      - `mode`: Mode to set (INPUT or OUTPUT).
   - **Reference**: 
      - [pinMode function](https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/)

3. **`GPIOWrite(uint8_t pin, uint8_t mode, uint8_t value = 0)`**:
   - Writes a value to a digital pin.
   - **Parameters**:
     - `pin`: Pin number to write to.
     - `mode`: Value to set (HIGH, LOW, ANALOGWRITE).
     - `value`: The duty cycle: between 0 (always off) and 255 (always on)
   - **Reference**: 
      - [digitalWrite function](https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/)
      - [analogWrite function](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogWrite/)

4. **`GPIORead(uint8_t pin, uint8_t state)`**:
   - Reads a value from a pin.
   - **Parameters**:
     - `pin`: Pin number to read from.
     - `state`: Type of read operation (DIGITALREAD or ANALOGREAD).
   - **Returns**:
     - The read value.
   - **Reference**: 
      - [digitalRead function](https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread)
      - [analogRead function](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/)

5. **`uptimeUs()`**
   - Returns elapsed time in microseconds since the start of the program.
   - Ensures precise timing via the Timer0 overflow interrupt.

6. **`uptimeMs()`**: 
   - Returns elapsed time in milliseconds since the start of the program.
   - Ensures precise timing via the Timer1 overflow interrupt.

7. **`sleep(unsigned long ms)`**: 
   - Pauses the program for the specified number of milliseconds.

8. **`sleepMicroseconds(unsigned long us)`**
   - Pauses the program for the specified number of microseconds.

9. **`Serial_begin(unsigned long baud)`**: 
   - Initiates Serial communication at the given baud rate.

10. **`Serial_print(const char* str)`**: 
   - Sends a string over the Serial port.
   - Converts newlines `\n` to carriage return + newline `\r\n` for compatibility.

11. **`Serial_println(const char* str = "")`**: 
   - Sends a string over the Serial port with a newline character.
   - Default parameter allows calling `Serial_println()` without arguments to print just a newline.

12. **`Serial_printf(const char *format, ...)`**:
    - Sends a formatted string over the Serial port.
    - Uses dynamic buffer allocation to handle variable length formatted strings.

## main.cpp

### Description
The `main.cpp` file demonstrates the use of the `AVRLite.h` library. It showcases GPIO control, timing, and serial communication.

### Implementation
1. **Initialization**:
   - Sets up the millis timer and initializes Serial communication.
   - Configures pins D13, D12 and D11 as OUTPUT using `GPIOInit`.

2. **Main Loop**:
   - **Reading LED States**:
     - Every 500 milliseconds, reads the states of LEDs on D13 and D12, printing them to the Serial Monitor.
   - **Toggling LEDs**:
     - LED on D11 fade flip-flop (set ANALOGWRITE pwm 0 - 255)
     - LED on D13 toggles every 5000 milliseconds.
     - LED on D12 toggles every 1000 milliseconds.
   - **State Update**:
     - Updates LED states based on their toggled values.
   - **Timing**:
     - Utilizes `uptimeMs()` for timing operations.

### Code Example
```cpp
#include "AVRLite.h"  // Include the necessary header for AVR-based microcontroller operations

#define LED_1 D13  // Define LED_1 on pin D13
#define LED_2 D12  // Define LED_2 on pin D12
#define LED_3 D11  // Define LED_3 on pin D11

// Variables to track the last uptime and the current brightness state
unsigned long previous_uptimeMs = 0;  // Stores the last update time for PWM functionality (using millis)
int brightness = 0;  // The current brightness of the LED, range from 0 to 255
int fadeAmount = 5;  // Amount by which brightness changes per step

/**
 * Function to control PWM on LED_3, simulating a fading effect.
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
```