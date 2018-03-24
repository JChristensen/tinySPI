// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
// https://github.com/JChristensen/tinySPI
//
// Example sketch to demonstrate both hardware and software SPI on an
// ATtiny24/44/84 to make an 8-LED chaser with a 74HC595 shift register.
// Tested with Arduino 1.8.5 and the ATTiny Core,
// https://github.com/SpenceKonde/ATTinyCore
//
// Set pin mapping to counterclockwise and the system clock to 1MHz internal.
//
// For hardware SPI, connect:
//   ATtiny84 pin 5 (DIP pin 8, PA5)  to 74HC595 pin 14 (data in)
//   ATtiny84 pin 6 (DIP pin 9, PA4)  to 74HC595 pin 11 (shift register clock)
//   ATtiny84 pin 7 (DIP pin 10, PA3) to 74HC595 pin 12 (storage register clock)
//
// For software SPI (shiftOut), connect:
//   ATtiny84 pin 8  (DIP pin 11, PA2) to 74HC595 pin 14 (data in)
//   ATtiny84 pin 9  (DIP pin 12, PA1) to 74HC595 pin 11 (shift register clock)
//   ATtiny84 pin 10 (DIP pin 13, PA0) to 74HC595 pin 12 (storage register clock)
//
// Connect eight LEDs from the shift register's outputs (Q0-Q7) to ground, through
// appropriate current-limiting resistors.
//
// Connect the 74HC595 Master Reset pin (MR) to Vcc (e.g. +5V) and the Output
// Enable pin (OE) to ground.
//
// Note that the two run simultaneously, so the shift register can be connected
// to either set of pins. Alternatively, two shift registers could be
// connected simultaneously.
//
// Jack Christensen 24Oct2013

#include <tinySPI.h>        // https://github.com/JChristensen/tinySPI

// pin definitions
const int
    SPI_LATCH(7),           // storage register clock (slave select) -- hardware
    SO_DATA(8),             // data -- software
    SO_CLOCK(9),            // shift register clock -- software
    SO_LATCH(10);           // storage register clock (slave select) -- software

void setup()
{
    // pullups on for unused pins to minimize power consumption
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    
    SPI.begin();                    // start hardware SPI
    pinMode(SPI_LATCH, OUTPUT);     // set up the hardware SS pin (the SPI library sets up the clock and data pins)
    digitalWrite(SPI_LATCH, HIGH);

    pinMode(SO_CLOCK, OUTPUT);      // set up the pins for software SPI
    pinMode(SO_DATA, OUTPUT);
    pinMode(SO_LATCH, OUTPUT);
}

void loop()
{
    for (int b=0; b<8; b++)
    {
        // hardware SPI
        digitalWrite(SPI_LATCH, LOW);
        SPI.transfer(1 << b);
        digitalWrite(SPI_LATCH, HIGH);

        // software SPI
        digitalWrite(SO_LATCH, LOW);
        shiftOut(SO_DATA, SO_CLOCK, MSBFIRST, 1 << b);
        digitalWrite(SO_LATCH, HIGH);

        delay(100);
    }
}
