// tinySPI.h - Arduino hardware SPI master library for
// ATtiny44/84, ATtiny45/85, ATtiny461/861, ATtiny2313/4313.
//
// Jack Christensen 24Oct2013
//
// https://github.com/JChristensen/tinySPI
//
// "Arduino tinySPI Library" by Jack Christensen is licensed under
// CC BY-SA 4.0, https://creativecommons.org/licenses/by-sa/4.0/
 
#ifndef TINYSPI_H_INCLUDED
#define TINYSPI_H_INCLUDED

#include <util/atomic.h>

// USI ports and pins
// (Thanks to nopnop2002 for adding the definitions for ATtiny461/861 and 2313/4313.)
#if defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define SPI_DDR_PORT DDRA
  #define USCK_DD_PIN DDA4
  #define DO_DD_PIN DDA5
  #define DI_DD_PIN DDA6
#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define SPI_DDR_PORT DDRB
  #define USCK_DD_PIN DDB2
  #define DO_DD_PIN DDB1
  #define DI_DD_PIN DDB0
#elif defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)
  #define SPI_DDR_PORT DDRB
  #define USCK_DD_PIN DDB2
  #define DO_DD_PIN DDB1
  #define DI_DD_PIN DDB0
#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny4313__)
  #define SPI_DDR_PORT DDRB
  #define USCK_DD_PIN DDB7
  #define DO_DD_PIN DDB6
  #define DI_DD_PIN DDB5
#else
  #error "tinySPI does not support this microcontroller."
#endif

// SPI data modes
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04

class tinySPI
{
    public:
        inline static void begin();
        inline static void setDataMode(uint8_t spiDataMode);
        inline static uint8_t transfer(uint8_t spiData);
        inline static void end();
};

void tinySPI::begin()
{
    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
    SPI_DDR_PORT |= _BV(USCK_DD_PIN);   // set the USCK pin as output
    SPI_DDR_PORT |= _BV(DO_DD_PIN);     // set the DO pin as output
    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    // set the DI pin as input
}

void tinySPI::setDataMode(uint8_t spiDataMode)
{
    if (spiDataMode == SPI_MODE1)
        USICR |= _BV(USICS0);
    else
        USICR &= ~_BV(USICS0);
}

uint8_t tinySPI::transfer(uint8_t spiData)
{
    USIDR = spiData;
    USISR = _BV(USIOIF);                // clear counter and counter overflow interrupt flag
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
    {
        while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
    }
    return USIDR;
}

void tinySPI::end()
{
    USICR &= ~(_BV(USIWM1) | _BV(USIWM0));
}

tinySPI SPI;

#endif
