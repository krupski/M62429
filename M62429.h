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

#ifndef _M62429_H
#define _M62429_H

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

class M62429
{
public:
	void init (uint8_t, uint8_t);
	uint16_t setLeft (uint8_t);
	uint16_t setRight (uint8_t);
	uint16_t setBoth (uint8_t);

private:
	uint8_t _CLK_PIN;
	uint8_t _DAT_PIN;
	uint16_t setVolume (uint8_t, bool, bool);
};

#endif

