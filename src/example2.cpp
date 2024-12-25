#include "AVRLite.h"

const uint16_t LED_PINS[3] = {D3, D5, D6};

int main() {
    for (const auto pin : LED_PINS) {
        GPIOControl(pin, OUTPUT);
    }

    for (;;) {
        for (const auto pin : LED_PINS) {
            for (int i = 0; i < 255; i++) {
                GPIOWrite(pin, i);
                sleep(50);
            }
            for (int i = 255; i > 0; i--) {
                GPIOWrite(pin, i);
                sleep(50);
            }
        }
    }

    return 0;
}
