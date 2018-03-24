// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
// https://github.com/JChristensen/tinySPI
//
// Example sketch to demonstrate use of hardware or software SPI on an
// ATtiny25/45/85 to make an 8-LED chaser with a 74HC595 shift register.
// Tested with Arduino 1.8.5 and the ATTiny Core,
// https://github.com/SpenceKonde/ATTinyCore
//
// Set the system clock to 1MHz internal.
//
// Connections:
//   ATtiny85 pin 0 (DIP pin 5, PB0) to 74HC595 pin 12 (storage register clock)
//   ATtiny85 pin 1 (DIP pin 6, PB1) to 74HC595 pin 14 (data in)
//   ATtiny85 pin 2 (DIP pin 7, PB2) to 74HC595 pin 11 (shift register clock)
//
// Connect eight LEDs from the shift register's outputs (Q0-Q7) to ground, through
// appropriate current-limiting resistors.
//
// Connect the 74HC595 Master Reset pin (MR) to Vcc (e.g. +5V) and the Output
// Enable pin (OE) to ground.
//
// Jack Christensen 24Oct2013

#include <tinySPI.h>            // https://github.com/JChristensen/tinySPI

#define HARDWARE_SPI 1          // set to 1 to use hardware SPI, set to 0 to use shiftOut (software SPI)

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
    
    #if HARDWARE_SPI == 1
    SPI.begin();                   // start hardware SPI (the library sets up the clock and data pins)
    #else
    pinMode(CLOCK_PIN, OUTPUT);    // set up the pins for software SPI
    pinMode(DATA_PIN, OUTPUT);
    #endif
    pinMode(LATCH_PIN, OUTPUT);    // latch pin needs to be set up for hardware or software
    digitalWrite(LATCH_PIN, HIGH);
}

void loop()
{
    for (int b=0; b<8; b++)
    {
        uint8_t myByte = 1 << b;
        digitalWrite(LATCH_PIN, LOW);
        
        // write the same byte 1000 times so the difference can be seen
        // between hardware and software SPI.
        for(int n=0; n<1000; n++)
        {
            #if HARDWARE_SPI == 1
            SPI.transfer(myByte);
            #else
            shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, myByte);
            #endif
        }
        digitalWrite(LATCH_PIN, HIGH);
    }
}
