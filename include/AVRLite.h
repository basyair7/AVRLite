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
#define ANALOGREAD   0x5
#define ANALOGWRITE  0x6
#define DIGITALREAD  0x7

// Definitions for digital pins D0 to D13
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

// Definitions for analog pins A0 to A5
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

volatile unsigned long timer1_overflow_count;
volatile unsigned long timer0_overflow_count;

ISR(TIMER0_OVF_vect) {
    timer0_overflow_count++;
}

// Interrupt Service Routine (ISR) for Timer1 overflow
ISR(TIMER1_COMPA_vect) {
    timer1_overflow_count++;
}

// Initialize Timers function
void __attribute__((constructor)) initTimers() {
    // Initialize Timer0 for uptimeUs
    TCCR0A = 0; // Set entire TCCR0A register to 0
    TCCR0B = 0; // Same for TCCR0B
    TCNT0 = 0;  // Initialize counter value to 0
    // Set Timer0 prescaler to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // Enable Timer0 overflow interrupt
    TIMSK0 |= (1 << TOIE0);

    // Initialize Timer1 for uptimeMs
    TCCR1A = 0;  // Set entire TCCR1A register to 0
    TCCR1B = 0;  // Same for TCCR1B
    TCNT1 = 0;   // Initialize counter value to 0
    // Set compare match register for 1 ms increments
    OCR1A = 249;  // = 16MHz / (64*1000) - 1 (must be <65536)
    // Turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS11 and CS10 bits for 64 prescaler
    TCCR1B |= (1 << CS11) | (1 << CS10);  
    // Enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Enable global interrupts
    sei();
}

// Return the number of milliseconds since the program started
unsigned long uptimeMs() {
    unsigned long m;
    // Enter critical section
    cli();
    m = timer1_overflow_count;
    sei();
    return m;
}

// Return the number of microseconds since the program started
unsigned long uptimeUs() {
    unsigned long m;
    uint8_t oldSREG = SREG, t;

    cli();
    m = timer0_overflow_count;
    t = TCNT0;
    if ((TIFR0 & (1 << TOV0) && (t < 255)))
        m++;
    
    SREG = oldSREG;

    return ((m << 8) + t * (64 / (F_CPU / 1000000L)));
}

// sleep for a specified number of milliseconds
void sleep(unsigned long ms) {
    unsigned long start = uptimeUs();

    while (ms > 0) {
        while ((ms > 0) && ((uptimeUs() - start) >= 1000)) {
            _delay_ms(1);
            ms--;
            start += 1000;
        }
    }
}

// Sleep for a specified number of microseconds
void sleepMicroseconds(unsigned int us) {
    while(us > 0) {
        _delay_us(1);
        us--;
    }
}

// Configure pin mode (INPUT or OUTPUT)
void GPIOInit(uint8_t pin, uint8_t mode) {
    if (mode == OUTPUT) {
        if (pin >= 0 && pin <= 7) {
            DDRD |= (1 << pin);
        } else if (pin >= 8 && pin <= 13) {
            DDRB |= (1 << (pin - 8));
        } else if (pin >= 14 && pin <= 19) {
            DDRC |= (1 << (pin - 14));
        }
    } else {
        if (pin >= 0 && pin <= 7) {
            DDRD &= ~(1 << pin);
        } else if (pin >= 8 && pin <= 13) {
            DDRB &= ~(1 << (pin - 8));
        } else if (pin >= 14 && pin <= 19) {
            DDRC &= ~(1 << (pin - 14));
        }
    }
}

// Overloaded versions of GPIOControl for DIGITALREAD and ANALOGREAD without mode parameter
int GPIORead(uint8_t pin, uint8_t state) {
    if (state == DIGITALREAD) {
        if (pin >= 0 && pin <= 7) 
            return (PIND & (1 << pin)) ? HIGH : LOW;
        else if (pin >= 8 && pin <= 13)
            return (PINB & (1 << (pin - 8))) ? HIGH : LOW;
        else if (pin >= 14 && pin <= 19)
            return (PINC & (1 << (pin - 14))) ? HIGH : LOW;

        return LOW;
    }
    
    if (state == ANALOGREAD) {
        if (pin >= A0 && pin <= A5) {
            uint8_t adc_pin = pin - A0;
            ADMUX = (1 << REFS0) | (adc_pin & 0x07);
            ADCSRA |= (1 << ADSC);
            while (ADCSRA & (1 << ADSC));
            return ADC;
        }
    }
    
    return LOW;
}

// Write to a digital pin
void GPIOWrite(uint8_t pin, uint8_t mode, uint8_t value = 0) {
    if (mode == HIGH) {
        if (pin >= 0 && pin <= 7) {
            PORTD |= (1 << pin);
        } else if (pin >= 8 && pin <= 13) {
            PORTB |= (1 << (pin - 8));
        } else if (pin >= 14 && pin <= 19) {
            PORTC |= (1 << (pin - 14));
        }
    } else if (mode == LOW) {
        if (pin >= 0 && pin <= 7) {
            PORTD &= ~(1 << pin);
        } else if (pin >= 8 && pin <= 13) {
            PORTB &= ~(1 << (pin - 8));
        } else if (pin >= 14 && pin <= 19) {
            PORTC &= ~(1 << (pin - 14));
        }
    } else if (mode == ANALOGWRITE) {
        // Handle analogWrite (PWM output)
        if (pin == D3 || pin == D11) {
            if (pin == D3) {
                TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); // Fast PWM, clear on compare match
                TCCR2B |= (1 << CS21); // Prescaler 8
                OCR2B = value; // Set duty cycle
            } else if (pin == D11) {
                TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21); // Fast PWM, clear on compare match
                TCCR2B |= (1 << CS21); // Prescaler 8
                OCR2A = value; // Set duty cycle
            }
        } else if (pin == D5 || pin == D6) {
            if (pin == D5) {
                TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // Fast PWM, clear on compare match
                TCCR0B |= (1 << CS01); // Prescaler 8
                OCR0B = value; // Set duty cycle
            } else if (pin == D6) {
                TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // Fast PWM, clear on compare match
                TCCR0B |= (1 << CS01); // Prescaler 8
                OCR0A = value; // Set duty cycle
            }
        } else if (pin == D9 || pin == D10) {
            if (pin == D9) {
                TCCR1A |= (1 << COM1A1) | (1 << WGM10); // Fast PWM, clear on compare match
                TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescaler 8
                OCR1A = value; // Set duty cycle
            } else if (pin == D10) {
                TCCR1A |= (1 << COM1B1) | (1 << WGM10); // Fast PWM, clear on compare match
                TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescaler 8
                OCR1B = value; // Set duty cycle
            }
        }
    }
}

// Control GPIO states
int GPIOControl(uint8_t pin, uint8_t mode, uint8_t value = 0) {
    if (mode == OUTPUT || mode == INPUT)
        GPIOInit(pin, mode);
    if (mode == HIGH || mode == LOW)
        GPIOWrite(pin, mode, value);
    if (mode == DIGITALREAD || mode == ANALOGREAD)
        return GPIORead(pin, mode);
}

// Initialize Serial communication
void Serial_begin(unsigned long baud) {
    unsigned long ubrr = F_CPU / 16 / baud - 1;
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Write data to Serial
void Serial_write(const char* str) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = *str;
}

// Write data to Serial with newline conversion
void Serial_print(const char* str) {
    while (*str) {
        if (*str == '\n') {
            // Send '\r' first for newline compatibility
            Serial_write("\r");
        }
        Serial_write(str);
        str++;
    }
}

// Write data to Serial with newline
void Serial_println(const char* str = "") {
    Serial_print(str);
    Serial_print("\n");
}

// FOrmatted print to Serial with dynamic buffer
void Serial_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Calculate the size needed
    int size = vsnprintf(NULL, 0, format, args) + 1; // +1 for null terminator
    va_end(args);

    // Allocate buffer dynamically
    char *buffer = (char *)malloc(size);
    if (buffer == NULL) {
        // Handle memory alloaction failure
        Serial_println("Error: Memory allocation failed");
        return;
    }

    va_start(args, format);
    vsnprintf(buffer, size, format, args);
    va_end(args);

    Serial_print(buffer);
    free(buffer); // Free the allocated memory
}

#endif
