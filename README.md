# Arduino tinySPI Library
http://github.com/JChristensen/tinySPI  
README file  
Jack Christensen  
Oct 2013  

## License
Arduino tinySPI Library Copyright (C) 2018 Jack Christensen GNU GPL v3.0

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License v3.0 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/gpl.html>

## Description
*tinySPI* is an Arduino SPI master library for ATtiny microcontrollers that utilizes the USI hardware in the ATtiny MCUs. Supported MCUs include ATtiny24/44/84, 25/45/85, 261/461/861, 2313/4313.

Because the USI hardware is not as sophisticated as the dedicated SPI hardware in the ATmega MCUs, *tinySPI* is more limited than the [standard Arduino SPI library](http://arduino.cc/en/Reference/SPI) but still has several advantages over software bit-bang approaches.

As compared to the [Arduino shiftOut() function](http://arduino.cc/en/Reference/ShiftOut), *tinySPI*

- is about 15 times faster,
- has a smaller program memory footprint, and
- has a consistent bit clock period.
 
*tinySPI* does not support

- sending the least-significant bit first,
- SPI data modes 2 and 3, or
- user-selectable SPI clock frequencies.
 
*tinySPI* will generate an SPI bit clock that is about one-tenth the MCU system clock frequency. For example, given a 1MHz system clock, the SPI clock will be about 100kHz, and it will take about 80μs to transfer one byte. 

Interrupts are inhibited during each byte transfer to ensure a consistent bit clock period.

Included with the library are example sketches that demonstrate driving a 74HC595 shift register from an ATtiny84 and from an ATtiny85.

*tinySPI* has been tested with [Arduino 1.8.5](http://arduino.cc/en/Main/Software) and the [ATTiny Core](https://github.com/SpenceKonde/ATTinyCore/).

## Methods
* begin()
* end()
* setDataMode()
* transfer()

### begin()
##### Description
Initializes the SPI bus, sets the USCK and DO pins to outputs, sets the DI pin to input.
##### Syntax
SPI.begin();
##### Parameters
None.
##### Returns
None.

### end()
##### Description
Disables the SPI bus (leaving the pin modes unchanged).
##### Syntax
SPI.end();
##### Parameters
None.
##### Returns
None.

### setDataMode()
##### Description
Sets the SPI data mode, i.e. clock polarity and phase. See the [Wikipedia article on SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus#Clock_polarity_and_phase) for details.
##### Syntax
SPI.setDataMode(mode)
##### Parameters
mode: SPI_MODE0 or SPI_MODE1 (modes 2 and 3 are not supported)
##### Returns
None.

### transfer()
##### Description
Transfers one byte over the SPI bus, both sending and receiving. Interrupts are inhibited during each byte transfer to ensure a consistent bit clock period.
##### Syntax
SPI.transfer(val);
##### Parameters
val: the byte to send over the bus _(byte)_
##### Returns
the byte read from the bus _(byte)_
