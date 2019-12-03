///////////////////////////////////////////////////////////////////////////////
//
//  NEC/Renesas M62429 Digital Volume Control Driver Library for Arduino
//  Copyright (c) 2014, 2019 Roger A. Krupski <rakrupski@verizon.net>
//
//  Last update: 02 December 2019
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
		M62429 (uint8_t, uint8_t);
		uint16_t setLeft (int8_t);
		uint16_t setRight (int8_t);
		uint16_t setBoth (int8_t);
	private:
		uint16_t _setVolume (int8_t, uint8_t, uint8_t);
		uint8_t _CLK_BIT;
		volatile uint8_t *_CLK_OUT;
		volatile uint8_t *_CLK_DDR;
		uint8_t _DAT_BIT;
		volatile uint8_t *_DAT_OUT;
		volatile uint8_t *_DAT_DDR;
};

#endif // #ifndef M62429_H
