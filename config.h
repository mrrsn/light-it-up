#ifndef CONFIG_H
#define CONFIG_H

const int _leds = 10; // 10 LEDs per level
const int _levels = 10; // 10 levels per skill
const int _maxSkill = 8; // 0-8 skills per game
const int _skillWindow[] = { 700, 650, 600, 550, 550, 550, 500, 500, 500 }; // starting milliseconds per LED on level 1 of each skill
const int _speedUp = 45; // milliseconds to speed up after each level

#endif
