/**  
 * @file example2.cpp
 * @brief Basic PWM ANALOGWRITE
 * @author basyair7
 * @date 2024
*/

#include "AVRLite.h"

const uint16_t LED_PINS[3] = {D3, D5, D6};

int main() {
    // Setup
    for (const auto pin : LED_PINS) {
        GPIOInit(pin, OUTPUT);
    }

    // Loop
    const uint32_t interval = 500;
    while(1) {
        for (const auto pin : LED_PINS) {
            for (int i = 0; i <= 255; i++) {
                GPIOWrite(pin, ANALOGWRITE, i);
                sleep(interval);
            }
            for (int i = 255; i >= 0; i--) {
                GPIOWrite(pin, ANALOGWRITE, i);
                sleep(interval);
            }
        }
    }

    return 0;
}
