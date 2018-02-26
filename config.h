#ifndef CONFIG_H
#define CONFIG_H

const uint16_t _capThreshold = 20;
const uint8_t _leftCapPad = 1;
const uint8_t _rightCapPad = 12;

const int _leds = 10; // 10 LEDs per level
const int _levels = 10; // 10 levels per skill
const int _maxSkill = 9; // 9 skills per game
const int _skillWindow[] = { 700, 650, 600, 550, 550, 550, 500, 500, 500 }; // starting milliseconds per LED on level 1 of each skill
const float _speedUpPct = CircuitPlayground.slideSwitch() ? 0.08f : 0.09f; // percent of initial window to speed up for the current skill

#endif
