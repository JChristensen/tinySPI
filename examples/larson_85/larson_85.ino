// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
// https://github.com/JChristensen/tinySPI
//
// Example sketch to implement an 8-LED Larson scanner using an
// ATtiny25/45/85 and a 74HC595 shift register.
//
// Tested with Arduino 1.8.5 and the ATTiny Core,
// https://github.com/SpenceKonde/ATTinyCore
//
// Connections:
//   ATtiny85 pin 0 (DIP pin 5, PB0) to 74HC595 pin 12 (storage register clock)
//   ATtiny85 pin 1 (DIP pin 6, PB1) to 74HC595 pin 14 (data in)
//   ATtiny85 pin 2 (DIP pin 7, PB2) to 74HC595 pin 11 (shift register clock)
//   74HC595 pin 10 (master reset) to Vcc (e.g. +5V)
//   74HC595 pin 13 (output enable) to ground
//   Eight LEDs connect from the shift register outputs (Q0-Q7) to ground,
//     through appropriate current-limiting resistors.
//
// Jack Christensen 14May2018

#include <tinySPI.h>            // https://github.com/JChristensen/tinySPI

// timing constants.
// for a bit different appearance, overlap turns the next LED on before
// turning the previous one off. set msOverlap to zero to disable the effect.
const uint32_t
    msInterval(250),            // milliseconds between changing the LED pattern
    msOverlap(50);              // milliseconds overlap between patterns

// pin definitions
const int
    LATCH_PIN(0),               // storage register clock (slave select)
    DATA_PIN(1),                // data in
    CLOCK_PIN(2);               // shift register clock

void setup()
{
    // pullups on for unused pins to minimize power consumption
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);

    SPI.begin();                    // start hardware SPI (the library sets up the clock and data pins)
    pinMode(LATCH_PIN, OUTPUT);     // set up the latch pin
    digitalWrite(LATCH_PIN, HIGH);

    // turn off all LEDs
    digitalWrite(LATCH_PIN, LOW);
    SPI.transfer(0);
    digitalWrite(LATCH_PIN, HIGH);
}

enum states_t {wait, overlap, next};    // states for the state machine

void loop()
{
    static states_t state;
    static uint8_t pattern(0x01);       // LED pattern
    static uint8_t prevPattern(0x01);   // previous LED pattern
    static bool scanDirection(true);
    static uint32_t msLast;             // last time we changed LEDs
    uint32_t ms = millis();

    switch (state)
    {
        case wait:
            // wait until time to change the LED pattern
            if (ms - msLast >= msInterval - msOverlap)
            {
                // overlap: turn on both current and previous LED patterns.
                if (msOverlap > 0)
                {
                    state = overlap;
                    digitalWrite(LATCH_PIN, LOW);
                    SPI.transfer(pattern | prevPattern);
                    digitalWrite(LATCH_PIN, HIGH);
                }
                else    // no overlap
                {
                    state = next;
                }
            }
            break;

        case overlap:
            // wait until the end of the overlap period
            if (ms - msLast >= msInterval) state = next;
            break;

        case next:
            // send the current pattern to the shift register
            state = wait;
            msLast = ms;
            digitalWrite(LATCH_PIN, LOW);
            SPI.transfer(pattern);
            digitalWrite(LATCH_PIN, HIGH);

            // calculate the next pattern
            prevPattern = pattern;
            if (scanDirection)
            {
                pattern *= 2;
                if (pattern == 0x80) scanDirection = !scanDirection;
            }
            else
            {
                pattern /= 2;
                if (pattern == 0x01) scanDirection = !scanDirection;
            }
            break;
    }
}

