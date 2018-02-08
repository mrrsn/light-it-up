//Light It Up
//Copyright (c) 2018 Rob Morrison
//MIT License, include above text in redistribution
//
//

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>
#include "config.h"
#include "melodies.h"

int _currSkill = 0;
int _timeWindow = _skillWindow[0];
bool _currLevel[] = { false, false, false, false, false, false, false, false, false, false };

void setup() {
    randomSeed( analogRead(0) );
    CircuitPlayground.begin( _brightness );
    PlayMusic( _helloMelody, _helloMelodyLen );

    while ( !CircuitPlayground.rightButton() ) {}
    Countdown();
}

void loop() {
    int change = 0;
    for ( int level = 0; level < _levels; level++ ) {
        int led = 0;
        while ( !IsFull( _currLevel ) ) {
            CircuitPlayground.playTone(  _currLevel[led] ? _onTune[led] : _offTune[led], (int)_timeWindow*0.8 , false /* i.e. don't block */ );
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
        CircuitPlayground.playTone( _startTune[i], noteDuration[i], true /* i.e. block */ );
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
    for ( int note = 0; note < len; note++ ) {
        CircuitPlayground.playTone( melody[note], 100, true /* i.e. block */ );
        delay(20);
    }
}
