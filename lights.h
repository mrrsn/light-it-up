#ifndef LIGHTS_H
#define LIGHTS_H

const unsigned int _waitWaitGo[] = { 0xFF0000, 0xFFFF00, 0x00FF00 };
const unsigned int _nicePalette[] = { 0xFF0000, 0x0000FF, 0x77FFFF, 0xFF3456, 0x5555FF, 0xFF7700, 0xB0BFFF };
const short _niceColorsCount = 7;

unsigned int _offColor = 0x0000FF;
unsigned int _onColor = 0x00FF00;

const int _brightness = 1;

unsigned int RandomColor( int rand ) { return _nicePalette[ rand % _niceColorsCount ]; }

#endif
