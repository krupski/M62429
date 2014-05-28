///////////////////////////////////////////////////////////////////////////////
//
//  NEC/Renesas M62429 Digital Volume Control Driver Library for Arduino
//  Copyright (c) 2014 Roger A. Krupski <rakrupski@verizon.net>
//
//  Last update: 08 May 2014
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

void M62429::init (uint8_t clock, uint8_t data)
{
	_CLK_PIN = clock; // pin 5 on the M62429
	_DAT_PIN = data; // pin 4 on the M62429
	digitalWrite (_DAT_PIN, LOW);
	digitalWrite (_CLK_PIN, LOW);
	pinMode (_DAT_PIN, OUTPUT);
	pinMode (_CLK_PIN, OUTPUT);
	_delay_ms (100); // give chip time to stabilize
}

uint16_t M62429::setLeft (uint8_t volume) // set CH1 (pins 1 [in] and 2 [out])
{
	return setVolume (volume, false, true);
}

uint16_t M62429::setRight (uint8_t volume) // set CH2 (pins 8 [in] and 7 [out])
{
	return setVolume (volume, true, true);
}

uint16_t M62429::setBoth (uint8_t volume) // both simultaneously
{
	return setVolume (volume, false, false);
}

uint16_t M62429::setVolume (uint8_t volume, bool channel, bool both)
{
	uint8_t bits;
	uint16_t data = 0; // control word is built by OR-ing in the bits

	// convert attenuation to volume
	volume = (volume > 100) ? 0 : (((volume * 83) / -100) + 83); // remember 0 is full volume!

	data |= (channel << 0); // D0 (channel select: 0=ch1, 1=ch2)
	data |= (both << 1); // D1 (individual/both select: 0=both, 1=individual)
	data |= ((21 - (volume / 4)) << 2); // D2...D6 (ATT1: coarse attenuator: 0,-4dB,-8dB, etc.. steps of 4dB)
	data |= ((3 - (volume % 4)) << 7); // D7...D8 (ATT2: fine attenuator: 0...-1dB... steps of 1dB)
	data |= (0b11 << 9); // D9...D10 // D9 & D10 must both be 1

	for (bits = 0; bits < 11; bits++) { // send out 11 control bits
		_delay_us (2); // pg.4 - M62429P/FP datasheet
		digitalWrite (_DAT_PIN, 0);
		_delay_us (2);
		digitalWrite (_CLK_PIN, 0);
		_delay_us (2);
		digitalWrite (_DAT_PIN, (data >> bits) & 0x01);
		_delay_us (2);
		digitalWrite (_CLK_PIN, 1);
	}

	_delay_us (2);
	digitalWrite (_DAT_PIN, 1); // final clock latches data in
	_delay_us (2);
	digitalWrite (_CLK_PIN, 0);

	return data; // return bit pattern in case you want it :)
}
