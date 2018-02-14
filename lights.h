#ifndef LIGHTS_H
#define LIGHTS_H

struct COLOR { uint8_t red, green, blue; };

const COLOR red = { 255, 0, 0 };
const COLOR orange = { 255, 128, 0 };
const COLOR yellow = { 255, 255, 0 };
const COLOR green = { 0, 255, 0 };
const COLOR blue = { 0, 0, 255 };
const COLOR lightBlue = { 119, 255, 255 };
const COLOR lightRed = { 255, 52, 86 };
const COLOR electricBlue = { 125, 249, 255 };
const COLOR bob = { 176, 177, 17 }; // 0xB0B111 :D
const COLOR dave = { 218, 94, 17 }; // 0xDA5E11 :D

const COLOR _readySetGo[] = { red, yellow, green };

const COLOR _nicePalette[] = { red, blue, lightBlue, lightRed, electricBlue, orange, bob, dave };
const short _nicePaletteCount = 8;

COLOR _offColor = blue;
COLOR _onColor = green;

COLOR RandomColor( int rand ) { return _nicePalette[ rand % _nicePaletteCount ]; }

const int _brightness = 5;

#endif
