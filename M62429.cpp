///////////////////////////////////////////////////////////////////////////////
//
//  NEC/Renesas M62429 Digital Volume Control Driver Library for Arduino
//  Copyright (c) 2014, 2017 Roger A. Krupski <rakrupski@verizon.net>
//
//  Last update: 16 February 2017
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#include "M62429.h"

M62429::M62429 (uint8_t clk_pin, uint8_t dat_pin, uint8_t vcc_pin, uint8_t gnd_pin)
{
	uint8_t x;

	// if a ground pin is specified, configure it
	if (gnd_pin != 99) {
		x = digitalPinToPort (gnd_pin); // pin -> port
		_GND_OUT = portOutputRegister (x); // set gnd pin as output
		_GND_DDR  = portModeRegister (x); // get gnd pin's DDR
		_GND_BIT = digitalPinToBitMask (gnd_pin); // get gnd pin's bitmask
		*_GND_OUT &= ~_GND_BIT; // set gnd pin low
		*_GND_DDR |= _GND_BIT; // set gnd pin DDR to output
	}

	// if a vcc pin is specified, configure it
	if (vcc_pin != 99) {
		x = digitalPinToPort (vcc_pin); // pin -> port
		_VCC_OUT  = portOutputRegister (x); // set vcc pin as output
		_VCC_DDR   = portModeRegister (x); // get vcc pin's DDR
		_VCC_BIT  = digitalPinToBitMask (vcc_pin); // get vcc pin's bitmask
		*_VCC_OUT |= _VCC_BIT; // set vcc pin high
		*_VCC_DDR |= _VCC_BIT; // set vcc pin DDR to output
		__builtin_avr_delay_cycles ((F_CPU / 1e3) * 250); // let chip settle
	}

	_init (clk_pin, dat_pin);
}

uint16_t M62429::setLeft (int8_t vol) // set CH1 (pins 1 [in] and 2 [out])
{
	return _setVolume (vol, 0, 1);
}

uint16_t M62429::setRight (int8_t vol) // set CH2 (pins 8 [in] and 7 [out])
{
	return _setVolume (vol, 1, 1);
}

uint16_t M62429::setBoth (int8_t vol) // both simultaneously
{
	return _setVolume (vol, 0, 0);
}

void M62429::_init (uint8_t clk_pin, uint8_t dat_pin)
{
	uint8_t x;

	x = digitalPinToPort (clk_pin); // pin 5 on the M62429
	_CLK_OUT = portOutputRegister (x);
	_CLK_DDR = portModeRegister (x);
	_CLK_BIT = digitalPinToBitMask (clk_pin);

	x = digitalPinToPort (dat_pin); // pin 4 on the M62429
	_DAT_OUT = portOutputRegister (x);
	_DAT_DDR = portModeRegister (x);
	_DAT_BIT = digitalPinToBitMask (dat_pin);

	*_DAT_OUT &= ~_DAT_BIT; // digitalWrite low
	*_CLK_OUT &= ~_CLK_BIT; // digitalWrite low

	*_DAT_DDR |= _DAT_BIT; // pinmode output
	*_CLK_DDR |= _CLK_BIT; // pinmode output
}

uint16_t M62429::_setVolume (int8_t volume, uint8_t chan, uint8_t both)
{
	uint8_t bits; // 11 bit control
	uint16_t atten; // volume converted to attenuation
	uint16_t data; // control word is built by OR-ing in the bits

	// constrain volume to 0...100
	volume = volume < 0 ? 0 : volume > 100 ? 100 : volume;

	// convert volume 0...100 to attenuation range 0...84
	atten = ((volume * 84) / 100);

	// initialize (clear) data
	data = 0;

	data |= chan ? (1 << 0) : (0 << 0); // D0 (channel select: 0=ch1, 1=ch2)
	data |= both ? (1 << 1) : (0 << 1); // D1 (individual/both select: 0=both, 1=individual)
	data |= (atten & (0b11111 << 2)); // D2...D6 (0...84 in steps of 4)
	data |= ((atten << 7) & (0b11 << 7)); // D7 & D8 (0...3)
	data |= (1 << 9); // D9 and...
	data |= (1 << 10); // ...D10 must both be 1

	for (bits = 0; bits < 11; bits++) { // send out 10 control bits
		__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
		*_DAT_OUT &= ~_DAT_BIT;
		__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
		*_CLK_OUT &= ~_CLK_BIT;
		__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
		data & _BV(bits) ? *_DAT_OUT |= _DAT_BIT : *_DAT_OUT &= ~_DAT_BIT;
		__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
		*_CLK_OUT |= _CLK_BIT;
	}
	// send final (11th) bit
	__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
	*_DAT_OUT |= _DAT_BIT; // final clock latches data in
	__builtin_avr_delay_cycles ((F_CPU / 1e6) * 2);
	*_CLK_OUT &= ~_CLK_BIT;

	return data; // return bit pattern in case you want it :)
}
