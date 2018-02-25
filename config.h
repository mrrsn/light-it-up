#ifndef CONFIG_H
#define CONFIG_H

const uint16_t _capThreshold = 20;
const uint8_t _leftCapPad = 1;
const uint8_t _rightCapPad = 12;

const int _leds = 10; // 10 LEDs per level
const int _levels = 10; // 10 levels per skill
const int _maxSkill = 8; // 0-8 skills per game
const int _skillWindow[] = { 700, 650, 600, 550, 550, 550, 500, 500, 500 }; // starting milliseconds per LED on level 1 of each skill
const float _speedUpPct = 0.08; // percent of initial window to speed up for the current skill

#endif
