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

#ifndef M62429_H
#define M62429_H

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

class M62429
{
	public:
		M62429 (uint8_t, uint8_t, uint8_t=99, uint8_t=99);
		uint16_t setLeft (int8_t);
		uint16_t setRight (int8_t);
		uint16_t setBoth (int8_t);

	private:
		void _init (uint8_t, uint8_t);
		uint16_t _setVolume (int8_t, uint8_t, uint8_t);
		uint8_t _CLK_BIT;
		volatile uint8_t *_CLK_OUT;
		volatile uint8_t *_CLK_DDR;
		uint8_t _DAT_BIT;
		volatile uint8_t *_DAT_OUT;
		volatile uint8_t *_DAT_DDR;
		uint8_t _VCC_BIT;
		volatile uint8_t *_VCC_OUT;
		volatile uint8_t *_VCC_DDR;
		uint8_t _GND_BIT;
		volatile uint8_t *_GND_OUT;
		volatile uint8_t *_GND_DDR;
};

#endif // #ifndef M62429_H

