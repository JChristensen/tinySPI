// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Arduino hardware SPI master library for
// ATtiny24/44/84, ATtiny25/45/85, ATtiny261/461/861, ATtiny2313/4313.
//
// https://github.com/JChristensen/tinySPI
// Jack Christensen 24Oct2013

#ifndef TINYSPI_H_INCLUDED
#define TINYSPI_H_INCLUDED

#include <util/atomic.h>

// USI ports and pins
// (Thanks to nopnop2002 for adding the definitions for ATtiny461/861 and 2313/4313.)
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define SPI_DDR_PORT DDRA
  #define USCK_DD_PIN DDA4
  #define DO_DD_PIN DDA5
  #define DI_DD_PIN DDA6
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define SPI_DDR_PORT DDRB
  #define USCK_DD_PIN DDB2
  #define DO_DD_PIN DDB1
  #define DI_DD_PIN DDB0
#elif defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)
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
        static void begin();
        static void setDataMode(uint8_t spiDataMode);
        static uint8_t transfer(uint8_t spiData);
        static void end();
};

extern tinySPI SPI;

#endif
