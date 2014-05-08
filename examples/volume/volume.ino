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
//
//  Note you will need to run this program with a real terminal emulator
//  and not the piece of junk Arduino IDE serial monitor. This is because
//  the Arduino monitor doesn't know how to handle carriage returns so the
//  spinner doesn't display properly.
//
//  Lines marked "// <----- important" are the only ones you really need to
//  use this driver. The rest of the code is just a demo.
//
///////////////////////////////////////////////////////////////////////////////

#include <M62429.h> // <------------------------ important

#define clock_pin 6 // volume control board clock pin connected to Arduino pin 6
#define data_pin 7 // volume control board data pin connected to Arduino pin 7

static M62429 VOL; // <------------------------ important

const char *spinner[] = {
	"/", "-", "\\", "|"
};

char c;
uint8_t x, y;

void setup (void)
{
	Serial.begin (9600);

	VOL.init (clock_pin, data_pin); // <------------------------ important

	Serial.println ("Volume control initialized");
}

void loop (void)
{
	Serial.print ("\r\n\r\n");
	Serial.print ("Which channel(s) to sweep?\r\n\r\n");
	Serial.print ("L == Left channel\r\n");
	Serial.print ("R == Right channel\r\n");
	Serial.print ("B == Both at once\r\n\r\n");
	Serial.print ("Your choice: ");

	while (! Serial.available ());

	c = toupper (Serial.read ());

	switch (c) {
		case 'L': {
			y = 0;
			break;
		}
		case 'R': {
			y = 1;
			break;
		}
		case 'B': {
			y = 2;
			break;
		}
		default: {
			Serial.print ("\r\n\r\n");
			Serial.print ("Unrecognized command - please type L, R or B\r\n");
			y = 255;
			break;
		}
	}

	for (x = 0; x <= 100; x++) {
		switch (y) {
			case 0: {
				VOL.setLeft (x); // <------------------------ important
				Serial.print ("\rLeft channel ");
				Serial.print (spinner[x%4]);
				break;
			}
			case 1: {
				VOL.setRight (x); // <------------------------ important
				Serial.print ("\rRight channel ");
				Serial.print (spinner[x%4]);
				break;
			}
			case 2: {
				VOL.setBoth (x); // <------------------------ important
				Serial.print ("\rBoth channels ");
				Serial.print (spinner[x%4]);
				break;
			}
			default: {
				x = 100; // break out fast
				break;
			}
		}
		_delay_ms (100);
	}

	while (x--) {
		switch (y) {
			case 0: {
				VOL.setLeft (x);
				Serial.print ("\rLeft channel ");
				Serial.print (spinner[x%4]);
				break;
			}
			case 1: {
				VOL.setRight (x);
				Serial.print ("\rRight channel ");
				Serial.print (spinner[x%4]);
				break;
			}
			case 2: {
				VOL.setBoth (x);
				Serial.print ("\rBoth channels ");
				Serial.print (spinner[x%4]);
				break;
			}
			default: {
				x = 0; // break out fast
				break;
			}
		}
		_delay_ms (100);
	}
}

