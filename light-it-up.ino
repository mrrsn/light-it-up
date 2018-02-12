//Light It Up
//Copyright (c) 2018 Rob Morrison
//MIT License, include above text in redistribution
//
//

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>
#include "config.h"
#include "lights.h"
#include "melodies.h"

int _done = 0; // hit the reset button to start over
int _currSkill = 0;
int _timeWindow = _skillWindow[0];
bool _currLevel[] = { false, false, false, false, false, false, false, false, false, false };

void setup() {
    randomSeed( analogRead(0) );
    CircuitPlayground.begin( _brightness );
    PlayMusic( _helloMelody, _helloMelodyLen );

    _currSkill = JumpToSkill();
    _timeWindow = _skillWindow[ _currSkill ];

    while ( !CircuitPlayground.rightButton() ) {}
    Countdown();
}

void loop() {
    if ( _done ) return;

    int change = 0;
    for ( int level = 0; level < _levels; level++ ) {
        int led = 0;
        while ( !IsFull( _currLevel ) ) {
            CircuitPlayground.playTone(  _currLevel[led] ? _onTune[led] : _offTune[led], (int)_timeWindow*0.8 , false /* i.e. don't block */ );
            CircuitPlayground.setPixelColor( led, _onColor );

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

            EffectLightingDifficulty( _currSkill );

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

    if ( _currSkill < _maxSkill ) {
        IncreaseDifficulty();
    }
    else
    {
        InitGame();
        PlayMusic( _winMelody, _winMelodyLen );
        GameWonLightShow();
    }
}

void IncreaseDifficulty() {
    _timeWindow = _skillWindow[ ++_currSkill ];
}

void LoseOne() {
    if ( IsEmpty( _currLevel ) ) return;
    
    int p = random(10);
    while ( !_currLevel[p] ) { p = random(10); }

    _currLevel[p] = false;
    CircuitPlayground.setPixelColor( p, _offColor );

    CircuitPlayground.redLED( true );
    PlayMusic( _oopsMelody, _oopsMelodyLen );
    delay(250);
    CircuitPlayground.redLED( false );
}

void InitCurrentLevel() {
    _offColor = RandomColor( random( _niceColorsCount ) );
    int i = 10;
    while ( i --> 0 ) {
        _currLevel[i] = false;
        CircuitPlayground.setPixelColor( i, _offColor );
        delay(50);
    }
}

void InitGame() {
    _currSkill = 0;
    _timeWindow = _skillWindow[0];
}

bool IsEmpty( bool arr[] ) {
    int i = 10;
    while ( i --> 0 ) { if ( arr[i] ) return false; }
    return true;
}

bool IsFull( bool arr[] ) {
    int i = 10;
    while ( i --> 0 ) { if ( !arr[i] ) return false; }
    return true;
}

void LightThemAll( const unsigned int color ) {
    COLOR c;
    SplitColor( color, c );
    LightThemAll( c );
}

void LightThemAll( const COLOR& c ) {
    int i = 10;
    while ( i --> 0 ) CircuitPlayground.setPixelColor( i, c.red, c.green, c.blue );
}

void SplitColor( const unsigned int color, COLOR &c ) {
    c.red = (color >> 16) & 255;
    c.green = (color >> 8) & 255;
    c.blue = color & 255;
}

void Countdown() {
    for ( int i = 0; i < 3; i++ ) {
        LightThemAll( _waitWaitGo[i] );
        CircuitPlayground.playTone( _startTune[i], _startTuneDuration[i], true /* i.e. block */ );
        delay(300);
    }

    delay(1000);
    LightThemAll( _offColor );
}

int JumpToSkill() {
    LightThemAll( red );

    int skill = 0;
    CircuitPlayground.setPixelColor( skill, ELECTRIC_BLUE );

    while ( !CircuitPlayground.rightButton() && skill < _maxSkill ) {
        if ( CircuitPlayground.leftButton() ) {
            CircuitPlayground.setPixelColor( ++skill, ELECTRIC_BLUE );
            delay( 250 );
        }
    }

    while ( !CircuitPlayground.rightButton() ) {}

    return skill;
}

void GameWonLightShow() {
    LightThemAll( GREEN );
    while ( true ) {
        _offColor = RandomColor( random( _niceColorsCount ) );
        CircuitPlayground.setPixelColor( random( 10 ), _offColor );
        delay(100);
        if ( CircuitPlayground.rightButton() || CircuitPlayground.leftButton() ) break;
    }

    CircuitPlayground.clearPixels();
    _done = 1;
}

void EffectLightingDifficulty( int skill ) {
    switch ( skill % 3 ) {
        case 2: // insane, color confuses more than helps, must play by sound alone
            CircuitPlayground.setPixelColor( random( 10 ), RandomColor( random( _niceColorsCount ) ) );
            break;
        case 1: // _offColor will be mildly confusing
            _offColor = RandomColor( random( _niceColorsCount ) );
            break;
        default:
            break;
    }
}

void PlayMusic( const int melody[], const int len ) {
    for ( int note = 0; note < len; note++ ) {
        CircuitPlayground.playTone( melody[note], 100, true /* i.e. block */ );
        delay(20);
    }
}
