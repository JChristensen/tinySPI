/*----------------------------------------------------------------------*
 * tinySPI.h - Arduino hardware SPI master library for ATtiny44/84,     *
 * and ATtiny45/85.                                                     *
 * Runs about 15 times faster than shiftOut().                          *
 *                                                                      *
 * Jack Christensen 24Oct2013                                           *
 *                                                                      *
 * CC BY-SA:                                                            *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

#include "tinySPI.h"

/*----------------------------------------------------------------------*
 * Constructor.                                                         *
 *----------------------------------------------------------------------*/
 tinySPI::tinySPI()
 {
 }
 
void tinySPI::begin(void)
{
    SPI_DDR_PORT |= _BV(USCK_DD_PIN);   //set the USCK pin as output
    SPI_DDR_PORT |= _BV(DO_DD_PIN);     //set the DO pin as output
    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    //set the DI pin as input
}

uint8_t tinySPI::transfer(uint8_t spiData)
{
    USIDR = spiData;
    USISR = _BV(USIOIF);                //clear counter and counter overflow interrupt flag
    USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
    while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
    return USIDR;
}

tinySPI SPI = tinySPI();                //instantiate a tinySPI object
