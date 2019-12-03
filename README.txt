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


How to use the driver (also see the example sketch in the package):

(1) Include the library in your sketch:

#include <M62429.h>


(2) Create an instance of the driver. Supply the Arduino pins you are
    using for CLOCK and DATA. For example, if you connected the CLOCK
    pin to Arduino pin 6 and the DATA pin to Arduino pin 7, you would
    initialize the driver as follows:

#define CLK 6 
#define DAT 7
static M62429 VOL (CLK, DAT);
  -- or simply --
static M62429 VOL (6, 7);

(note: "VOL" can be anything you like)


(3) Set the volume of either channel individually or both channels at
    the same time. Note the "Left" channel is CH1 and "Right" is CH2.

VOL.setLeft (volume); // sets CH1 to "volume", 0...100

VOL.setRight (volume); // sets CH2 to "volume", 0...100

VOL.setBoth (volume); // sets BOTH CH1 and CH2 to "volume", 0...100

Note: "setBoth" sets both channels __simultaneously__ to provide glitch-free
volume adjustment in stereo audio applications.


///////////////////////////////////////////////////////////////////////////////

Some information on the M62429 chip (see the M62429.pdf datasheet for full info)

The M62429 chip ATTENUATES the input signal by the specified "volume" value.
Therefore, "0" is "full volume" while "83" is no volume (muted). This is not
intuitive, so the driver reverses the way "volume" works and expand it's range
from 0...83 to 0...100 so that "0" if full quiet while 100 is full volume.

This makes the function a lot easier to understand and use.

A nice little board that has the M62429 chip on it is the Sure Electronics
rotary digital encoder volume control, sold by Parts Express and also directly
by Sure.

The volume control comes in two parts: A rotary encoder with a PIC microcontroller
on board to read the encoder and drive the digital volume control chip. The other
part is a small board with 5 volts and ground, two analog inputs, two analog outputs
and a 4 pin header for the digital interface (5V, GND, CLOCK and DATA). It is easy
to use the volume control board in a custom Arduino project by connecting it to the
Arduino board and using this driver.

You may wish to replace two capacitors on this board (C1 and C4) because they are
tiny little ceramic chips that can't be more than 0.01uF or so and they don't pass
low frequencies very well. You may want to use 1uF or 10uF capacitors instead (be
sure to observe polarity!)


URL for Parts Express:
http://www.parts-express.com/rotary-encoder-volume-control-board-for-select-amplifier-boards--320-318

URL for Sure Electronics:
http://www.sureelectronics.net/goods.php?id=1161


Pinout of the Sure Electronics digital volume control board (component side)

        +--------------------------------+
        |            C  D  G  +          |
        |            L  A  N  5          |
        |            K  T  D  V          |
        |                                |
        |  +5V        +-----+       GND  |
        |             | M62 |            |
        |  GND        | 429 |   CH1 OUT  | <-- "Left" Channel
        |             +-----+            |
        |  CH1 IN                   GND  |
        |                                |
        |  GND                  CH2 OUT  | <-- "Right" Channel
        |               ___              |
        |  CH2 IN      /   \        GND  |
        |              \___/             |
        |  GND       capacitor      +5V  |
        |                                |
        +--------------------------------+


-- end of file --
