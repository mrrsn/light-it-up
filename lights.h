#ifndef LIGHTS_H
#define LIGHTS_H

#define RED 0xFF0000
#define ORANGE 0xFF7700
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define LIGHT_BLUE 0x77FFFF
#define CANDY_RED 0xFF3456
#define ELECTRIC_BLUE 0x7DF9FF
#define BOB 0xB0BFFF

struct COLOR {
    int red, green, blue;
};

const COLOR red = { red: 255, green: 0, blue: 0 };
const COLOR yellow = { red: 255, green: 255, blue: 0 };
const COLOR green = { red: 0, green: 255, blue: 0 };

const COLOR _waitWaitGo[] = { red, yellow, green };
const unsigned int _nicePalette[] = { RED, BLUE, LIGHT_BLUE, CANDY_RED, ELECTRIC_BLUE, ORANGE, BOB };
const short _niceColorsCount = 7;

unsigned int _offColor = BLUE;
unsigned int _onColor = GREEN;

const int _brightness = 5;

unsigned int RandomColor( int rand ) { return _nicePalette[ rand % _niceColorsCount ]; }

#endif
