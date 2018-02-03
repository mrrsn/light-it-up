//Light It Up
//Copyright (c) 2018 Rob Morrison
//MIT License, include above text in redistribution
//
//

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>
#include "pitches.h"
uint32_t _offColor = 0x0000FF;
uint32_t _onColor = 0x00FF00;
const int _brightness = 1;
const int _helloMelody[] = { NOTE_C3, NOTE_C3, NOTE_G3, NOTE_C3, 0, NOTE_C4, NOTE_C4, NOTE_B3, NOTE_C4 };
const int _helloMelodyLen = 9;
const int _winMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0, 0, 0, 0, 0, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5 };
const int _winMelodyLen = 13;
const int _levelUpMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 0, 0, 0, 0, 0, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4 };
const int _levelUpMelodyLen = 13;
const int _oopsMelody[] = { NOTE_C4, NOTE_F3 };
const int _oopsMelodyLen = 2;
const int _startTune[] = { NOTE_C4, NOTE_C4, NOTE_C5 };
const int _offTune[] = { NOTE_C2, NOTE_G2, NOTE_D2, NOTE_G2, NOTE_E2, NOTE_G2, NOTE_F2, NOTE_G2, NOTE_E2, NOTE_G2 };
const int _onTune[] = { NOTE_C3, NOTE_G3, NOTE_D3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_F3, NOTE_G3, NOTE_E3, NOTE_G3 };
const int _LEDs = 10;
const int _levels = 10;
int _currSkill = 0;
const int _maxSkill = 8; // 0-8
const int _skillWindow[] = { 700, 625, 550 };
const int _speedUp = 45;
int _timeWindow = _skillWindow[0];
bool _currLevel[] = { false, false, false, false, false, false, false, false, false, false };

void setup() {
    randomSeed( analogRead(0) );
    CircuitPlayground.begin( _brightness );

    if ( CircuitPlayground.leftButton() && CircuitPlayground.rightButton() ) _timeWindow = _skillWindow[2];
    else if ( CircuitPlayground.leftButton() ) _timeWindow = _skillWindow[1];

    delay(500);
    PlayMusic( _helloMelody, _helloMelodyLen );
    delay(500);
}

void loop() {
    while ( !CircuitPlayground.rightButton() ) {}
    Countdown();
    
    int change = 0;
    
    for ( int level = 0; level < _levels; level++ ) {
        int led = 0;
        while ( !IsFull( _currLevel ) ) {
            if ( CircuitPlayground.slideSwitch() ) {
                CircuitPlayground.playTone(  _currLevel[led] ? _onTune[led] : _offTune[led], (int)_timeWindow*0.8 , false /* i.e. don't block */ );
            }
            CircuitPlayground.setPixelColor( led, _onColor );
            EffectLightingDifficulty( _currSkill );

            unsigned long previousMillis = millis();
            unsigned long currentMillis = millis();

            while( fabs( currentMillis - previousMillis ) < _timeWindow ) {
                currentMillis = millis();
                if ( CircuitPlayground.leftButton() && led >= 5 ||
                     CircuitPlayground.rightButton() && led < 5 ) {
                    _currLevel[led] = true;
                }
                else if ( ( CircuitPlayground.leftButton() && led < 5 ) || // wow, such wrong button
                          ( CircuitPlayground.rightButton() && led >= 5 ) ) {
                    LoseOne();
                }
            }

            if ( !_currLevel[led] ) {
                CircuitPlayground.setPixelColor( led, _offColor );
            }
            
            if ( led == 0 ) change = 1;
            if ( led == 9 ) change = -1;

            led += change;
        }

        _timeWindow -= _speedUp;
        PlayMusic( _levelUpMelody, _levelUpMelodyLen );
        InitCurrentLevel();
    }

    if ( _currSkill == _maxSkill ) {
        PlayMusic( _winMelody, _winMelodyLen );
        GameWonLightShow();
        _currSkill = 0;
    }
    else {
        IncreaseDifficulty();
    }
}

void IncreaseDifficulty() {
    int skill = ++_currSkill;
    _timeWindow = _skillWindow[skill % 3];
}

void LoseOne() {
    if ( IsEmpty( _currLevel ) ) return;
    
    int p = random(10);
    while ( !_currLevel[p] ) { p = random(10); }

    _currLevel[p] = false;
    CircuitPlayground.setPixelColor( p, _offColor );
    PlayMusic( _oopsMelody, _oopsMelodyLen );
    delay(250);
}

void InitCurrentLevel() {
    _offColor = RandomColor();
    int i = _LEDs;
    while ( i --> 0 ) {
        _currLevel[i] = false;
        CircuitPlayground.setPixelColor( i, _offColor );
        delay(50);
    }
}

uint32_t RandomColor() {
    uint32_t okRandomish[] = { 0xFF0000, 0x0000FF, 0x77FFFF, 0xFF3456, 0x5555FF, 0xFF7700, 0xB0BFFF };
    return okRandomish[random(7)];
}

bool IsEmpty( bool arr[] ) {
    int i = _LEDs;
    while ( i --> 0 ) { if ( arr[i] ) return false; }
    return true;
}

bool IsFull( bool arr[] ) {
    int i = _LEDs;
    while ( i --> 0 ) { if ( !arr[i] ) return false; }
    return true;
}

void LightThemAll( uint32_t color ) {
    int i = _LEDs;
    while ( i --> 0 ) CircuitPlayground.setPixelColor( i, color );
}

void Countdown() {
    uint32_t waitWaitGo[] = { 0xFF0000, 0xFFFF00, 0x00FF00 };
    int noteDuration[] = { 700, 700, 1000 };
    
    for ( int i = 0; i < 3; i++ ) {
        LightThemAll( waitWaitGo[i] );
        if ( CircuitPlayground.slideSwitch() ) {
            CircuitPlayground.playTone( _startTune[i], noteDuration[i], true /* i.e. block */ );
        }
        delay(300);
    }

    delay(1000);
    LightThemAll( _offColor );
}

void GameWonLightShow() {
    LightThemAll( 0x00FF00 );
    while ( true ) {
        _offColor = RandomColor();
        CircuitPlayground.setPixelColor( random(_LEDs), _offColor );
        delay(100);
        if ( CircuitPlayground.rightButton() ) break;
    }
}

void EffectLightingDifficulty( int skill ) {
    if ( skill < 3 ) return;
    
    if ( skill > 5 ) {
        // insane, color confuses more than helps, must play by sound alone
        CircuitPlayground.setPixelColor( random(_LEDs), RandomColor() );
    }
    else {
        // _offColor will be mildly confusing
        _offColor = RandomColor();
    }
}

void PlayMusic( const int melody[], const int len ) {
    if ( !CircuitPlayground.slideSwitch() ) return;
    
    for ( int note = 0; note < len; note++ ) {
        CircuitPlayground.playTone( melody[note], 100, true /* i.e. block */ );
        delay(20);
    }
}

