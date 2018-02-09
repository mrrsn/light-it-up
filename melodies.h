#ifndef MELODIES_H
#define MELODIES_H

#include "pitches.h"

const int _helloMelody[] = { NOTE_C3, NOTE_C3, NOTE_G3, NOTE_C3, 0, NOTE_C4, NOTE_C4, NOTE_B3, NOTE_C4 };
const int _helloMelodyLen = 9;

const int _winMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0, 0, 0, 0, 0, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5 };
const int _winMelodyLen = 13;

const int _levelUpMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0, 0, 0, 0, 0, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4 };
const int _levelUpMelodyLen = 13;

const int _oopsMelody[] = { NOTE_C4, NOTE_F3 };
const int _oopsMelodyLen = 2;

const int _startTune[] = { NOTE_C4, NOTE_C4, NOTE_C5 };
const int _startTuneDuration[] = { 700, 700, 1000 };

const int _offTune[] = { NOTE_C2, NOTE_G2, NOTE_D2, NOTE_G2, NOTE_E2, NOTE_G2, NOTE_F2, NOTE_G2, NOTE_E2, NOTE_G2 };
const int _onTune[] = { NOTE_C3, NOTE_G3, NOTE_D3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_F3, NOTE_G3, NOTE_E3, NOTE_G3 };

#endif
