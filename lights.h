#ifndef LIGHTS_H
#define LIGHTS_H

struct COLOR { uint8_t red; uint8_t green; uint8_t blue; };

const COLOR red = { red: 255, green: 0, blue: 0 };
const COLOR orange = { red: 255, green: 128, blue: 0 };
const COLOR yellow = { red: 255, green: 255, blue: 0 };
const COLOR green = { red: 0, green: 255, blue: 0 };
const COLOR blue = { red: 0, green: 0, blue: 255 };
const COLOR lightBlue = { red: 119, green: 255, blue: 255 };
const COLOR lightRed = { red: 255, green: 52, blue: 86 };
const COLOR electricBlue = { red: 125, green: 249, blue: 255 };
const COLOR bob = { red: 176, green: 191, blue: 255 };

const COLOR _readySetGo[] = { red, yellow, green };

const COLOR _nicePalette[] = { red, blue, lightBlue, lightRed, electricBlue, orange, bob };
const short _nicePaletteCount = 7;

COLOR _offColor = blue;
COLOR _onColor = green;

COLOR RandomColor( int rand ) { return _nicePalette[ rand % _nicePaletteCount ]; }

const int _brightness = 5;

#endif
