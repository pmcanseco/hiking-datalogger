# hiking-datalogger
The code for an Arduino-based device that records GPS and Environmental Data to an SD card over time.

## Wiring
This project currently runs on an Arduino Mega 2560. These wiring instructions are intended for that board.
```
Sparkfun Venus GPS:
3v3 -> 3v3 *
GND -> GND
Tx0 -> Pin 10

Waveshare 1.54" E-Paper Display:
3v3  -> 3v3 *
GND  -> GND *
BUSY -> Pin 7
RST  -> Pin 8
DC   -> Pin 9
DIN  -> Pin 51
CLK  -> Pin 52
CS   -> Pin 53

* NOTE: You cannot plug in both the screen and the gps 
to a single Arduino's 3v3 pin because it won't be able 
to handle it and the board will freeze. Either use 3v3 
directly from a battery or some other more capable 3.3V 
DC power supply.
```
