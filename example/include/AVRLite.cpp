#include "AVRLite.h"

// Interrupt Service Routine (ISR) for Timer0 overflow
volatile unsigned long timer0_overflow_count;
ISR(TIMER0_OVF_vect) {
    timer0_overflow_count++;
}

// Interrupt Service Routine (ISR) for Timer1 overflow
volatile unsigned long timer1_overflow_count;
ISR(TIMER1_COMPA_vect) {
    timer1_overflow_count++;
}

// Initialize Timers function
void __attribute__((constructor)) __initTimers__() {
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

// Sleep for a specified number of milliseconds
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
int GPIOInit(uint8_t pin, uint8_t mode) {
    if (mode == OUTPUT) {
        if (pin >= 0 && pin <= 7)        DDRD |= (1 << pin);
        else if (pin >= 8 && pin <= 13)  DDRB |= (1 << (pin - 8));
        else if (pin >= 14 && pin <= 19) DDRC |= (1 << (pin - 14));
        
        return 1;
    } 
    else {
        if (pin >= 0 && pin <= 7)        DDRD &= ~(1 << pin);
        else if (pin >= 8 && pin <= 13)  DDRB &= ~(1 << (pin - 8));
        else if (pin >= 14 && pin <= 19) DDRC &= ~(1 << (pin - 14));

        return 1;
    }

    return 0;
}

// Write to a digital pin
void __GPIODigitalWrite__(uint8_t pin, uint8_t mode) {
    if (mode == HIGH) {
        if (pin >= 0 && pin <= 7)        PORTD |= (1 << pin);
        else if (pin >= 8 && pin <= 13)  PORTB |= (1 << (pin - 8));
        else if (pin >= 14 && pin <= 19) PORTC |= (1 << (pin - 14));
    } 
    else if (mode == LOW) {
        if (pin >= 0 && pin <= 7)        PORTD &= ~(1 << pin);
        else if (pin >= 8 && pin <= 13)  PORTB &= ~(1 << (pin - 8));
        else if (pin >= 14 && pin <= 19) PORTC &= ~(1 << (pin - 14));
    }
}

// Handle analogWrite (PWM output)
void __GPIOAnalogWrite__(uint8_t pin, uint8_t value) {
    if (pin == D3 || pin == D11) {
        if (pin == D3) {
            if (value == 0) {
                TCCR2A &= ~(1 << COM2B1); // Non-PWM mode
                OCR2B = 0; // Set duty cycle to 0
            } else {
                TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); // Fast PWM, clear on compare match
                TCCR2B |= (1 << CS21); // Prescaler 8
                OCR2B = value; // Set duty cycle
            }
        } 
        else if (pin == D11) {
            if (value == 0) {
                TCCR2A &= ~(1 << COM2A1); // Non-PWM mode
                OCR2A = 0; // Set duty cycle to 0
            } else {
                TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21); // Fast PWM, clear on compare match
                TCCR2B |= (1 << CS21); // Prescaler 8
                OCR2A = value; // Set duty cycle
            }
        }
    } 
    else if (pin == D5 || pin == D6) {
        if (pin == D5) {
            if (value == 0) {
                TCCR0A &= ~(1 << COM0B1); // Non-PWM mode
                OCR0B = 0; // Set duty cycle to 0
            } else {
                TCCR0A |= (1 << COM0B1) | (1 << WGM00) | (1 << WGM01); // Fast PWM, clear on compare match
                TCCR0B |= (1 << CS01); // Prescaler 8
                OCR0B = value; // Set duty cycle
            }
        } 
        else if (pin == D6) {
            if (value == 0) {
                TCCR0A &= ~(1 << COM0A1); // Non-PWM mode
                OCR0A = 0; // Set duty cycle to 0
            } else {
                TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // Fast PWM, clear on compare match
                TCCR0B |= (1 << CS01); // Prescaler 8
                OCR0A = value; // Set duty cycle
            }
        }
    }
    else if (pin == D9 || pin == D10) {
        if (pin == D9) {
            if (value == 0) {
                TCCR1A &= ~(1 << COM1A1); // Non-PWM mode
                OCR1A = 0; // Set duty cycle to 0
            } else {
                TCCR1A |= (1 << COM1A1) | (1 << WGM10); // Fast PWM, clear on compare match
                TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescaler 8
                OCR1A = value; // Set duty cycle
            }
        } 
        else if (pin == D10) {
            if (value == 0) {
                TCCR1A &= ~(1 << COM1B1); // Non-PWM mode
                OCR1B = 0; // Set duty cycle to 0
            } else {
                TCCR1A |= (1 << COM1B1) | (1 << WGM10); // Fast PWM, clear on compare match
                TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescaler 8
                OCR1B = value; // Set duty cycle
            }
        }
    }
}

uint8_t GPIOWrite(uint8_t pin, uint8_t mode, uint8_t value) {
    if (mode == ANALOGWRITE) {
        __GPIOAnalogWrite__(pin, value);
        return value;
    }
    if (mode == HIGH || mode == LOW) {
        __GPIODigitalWrite__(pin, mode);
        return mode;
    }

    return 0;
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

// Control GPIO states
int GPIOControl(uint8_t pin, uint8_t mode, uint8_t value) {
    if (mode == OUTPUT || mode == INPUT)
        return GPIOInit(pin, mode);
    if (mode == DIGITALREAD || mode == ANALOGREAD)
        return GPIORead(pin, mode);
    if (mode == HIGH || mode == LOW || mode == ANALOGWRITE)
        return GPIOWrite(pin, mode, value);
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
void Serial_println(const char* str) {
    Serial_print(str);
    Serial_print("\n");
}

// Formatted print to Serial with dynamic buffer
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
