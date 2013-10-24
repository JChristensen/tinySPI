//Demonstration of both hardware and software SPI on an ATtiny44/84
//to make an 8-LED chaser with a 74HC595 shift register.
//Written using Arduino 1.0.5 with the Arduino-Tiny ATtiny
//core, http://code.google.com/p/arduino-tiny/
//
//For hardware SPI, connect:
//  ATtiny84 pin 5 (DIP pin 8, PA5)  to 74HC595 pin 14 (data in)
//  ATtiny84 pin 6 (DIP pin 9, PA4)  to 74HC595 pin 11 (shift register clock)
//  ATtiny84 pin 7 (DIP pin 10, PA3) to 74HC595 pin 12 (storage register clock)
//
//For software SPI (shiftOut), connect:
//  ATtiny84 pin 8  (DIP pin 11, PA2) to 74HC595 pin 14 (data in)
//  ATtiny84 pin 9  (DIP pin 12, PA1) to 74HC595 pin 11 (shift register clock)
//  ATtiny84 pin 10 (DIP pin 13, PA0) to 74HC595 pin 12 (storage register clock)
//
//Connect eight LEDs from the shift register's outputs (Q0-Q7) to ground, through
//appropriate current-limiting resistors.
//
//Note that the two run simultaneously, so the shift register can be connected
//to either set of pins. Alternatively, two shift registers could be
//connected simultaneously.
//
//Jack Christensen 24Oct2013
//
//CC BY-SA:
//This work is licensed under the Creative Commons Attribution-
//ShareAlike 3.0 Unported License. To view a copy of this license,
//visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
//letter to Creative Commons, 171 Second Street, Suite 300,
//San Francisco, California, 94105, USA.

#include <tinySPI.h>               //http://github.com/JChristensen/tinySPI

const int SPI_LATCH = 7;           //storage register clock (slave select) -- hardware
const int SO_DATA = 8;             //data -- software
const int SO_CLOCK = 9;            //shift register clock -- software
const int SO_LATCH = 10;           //storage register clock (slave select) -- software

void setup(void)
{
    //pullups on for unused pins to minimize power consumption
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    
    SPI.begin();                   //start hardware SPI
    pinMode(SPI_LATCH, OUTPUT);    //set up the hardware SS pin (the SPI library sets up the clock and data pins)
    digitalWrite(SPI_LATCH, HIGH);

    pinMode(SO_CLOCK, OUTPUT);     //set up the pins for software SPI
    pinMode(SO_DATA, OUTPUT);
    pinMode(SO_LATCH, OUTPUT);
}

void loop(void)
{
    for (int b=0; b<8; b++) {

        //hardware SPI
        digitalWrite(SPI_LATCH, LOW);
        SPI.transfer(1 << b);
        digitalWrite(SPI_LATCH, HIGH);

        //software SPI
        digitalWrite(SO_LATCH, LOW);
        shiftOut(SO_DATA, SO_CLOCK, MSBFIRST, 1 << b);
        digitalWrite(SO_LATCH, HIGH);

        delay(100);
    }
}

