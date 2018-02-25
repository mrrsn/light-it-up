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
int _speedUp = 0;
int _timeWindow = _skillWindow[0];
bool _currLevel[] = { false, false, false, false, false, false, false, false, false, false };

void setup() {
    Serial.begin(9600);
    randomSeed( analogRead(0) );
    CircuitPlayground.begin( _brightness );
    PlayMusic( _helloMelody, _helloMelodyLen );

    _currSkill = JumpToSkill();
    _timeWindow = _skillWindow[ _currSkill ];

    while ( !CircuitPlayground.rightButton() ) {}
    Countdown();
}

void loop() {
    PlayGame();
}

void PlayGame() {
    if ( _done ) return;

    PlaySkill();

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

void PlaySkill() {
    _speedUp = _skillWindow[_currSkill] * _speedUpPct;

    for ( int level = 0; level < _levels; level++ ) {
        PlayLevel();

        _timeWindow -= _speedUp;
        PlayMusic( _levelUpMelody, _levelUpMelodyLen );
        InitCurrentLevel();
    }
}

void PlayLevel() {
    int led = 0;
    int change = 0;

    while ( !IsFull( _currLevel ) ) {
        CircuitPlayground.playTone(  _currLevel[led] ? _onTune[led] : _offTune[led], (int)_timeWindow*0.8 , false /* i.e. don't block */ );
        CircuitPlayground.setPixelColor( led, _onColor.red, _onColor.green, _onColor.blue );

        unsigned long previousMillis = millis();
        unsigned long currentMillis = millis();

        while( fabs( currentMillis - previousMillis ) < _timeWindow ) {
            currentMillis = millis();
            Serial.println(CircuitPlayground.readCap(1));
            Serial.println(CircuitPlayground.readCap(12));
            if ( LeftButton() && led >= 5 ||
                 RightButton() && led < 5 ) {
                _currLevel[led] = true;
            }
            else if ( LeftButton() && led < 5 ||
                      RightButton() && led >= 5 ) {
                LoseOne();
            }
        }

        EffectLightingDifficulty( _currSkill );

        if ( !_currLevel[led] ) {
            CircuitPlayground.setPixelColor( led, _offColor.red, _offColor.green, _offColor.blue );
        }

        if ( led == 0 ) change = 1;
        if ( led == 9 ) change = -1;

        led += change;
    }
}

void IncreaseDifficulty() {
    _timeWindow = _skillWindow[ ++_currSkill ];
}

void LoseOne() {
    if ( IsEmpty( _currLevel ) ) return;
    
    int p = random( _leds );
    while ( !_currLevel[p] ) { p = random( _leds ); }

    _currLevel[p] = false;
    CircuitPlayground.setPixelColor( p, _offColor.red, _offColor.green, _offColor.blue );

    CircuitPlayground.redLED( true );
    PlayMusic( _oopsMelody, _oopsMelodyLen );
    delay(250);
    CircuitPlayground.redLED( false );
}

void InitCurrentLevel() {
    _offColor = RandomColor( random( _nicePaletteCount ) );

    int i = _leds;
    while ( i --> 0 ) {
        _currLevel[i] = false;
        CircuitPlayground.setPixelColor( i, _offColor.red, _offColor.green, _offColor.blue );
        delay(50);
    }
}

void InitGame() {
    _currSkill = 0;
    _timeWindow = _skillWindow[0];
}

bool IsEmpty( bool arr[] ) {
    int i = _leds;
    while ( i --> 0 ) { if ( arr[i] ) return false; }
    return true;
}

bool IsFull( bool arr[] ) {
    int i = _leds;
    while ( i --> 0 ) { if ( !arr[i] ) return false; }
    return true;
}

void LightThemAll( const COLOR& c ) {
    int i = _leds;
    while ( i --> 0 ) CircuitPlayground.setPixelColor( i, c.red, c.green, c.blue );
}

void Countdown() {
    for ( int i = 0; i < 3; i++ ) {
        LightThemAll( _readySetGo[i] );
        CircuitPlayground.playTone( _startTune[i], _startTuneDuration[i], true /* i.e. block */ );
        delay(300);
    }

    delay(1000);
    LightThemAll( _offColor );
}

int JumpToSkill() {
    LightThemAll( red );

    int skill = 0;
    CircuitPlayground.setPixelColor( skill, electricBlue.red, electricBlue.green, electricBlue.blue );

    while ( !CircuitPlayground.rightButton() && skill < _maxSkill ) {
        if ( CircuitPlayground.leftButton() ) {
            CircuitPlayground.setPixelColor( ++skill, electricBlue.red, electricBlue.green, electricBlue.blue );
            delay( 250 );
        }
    }

    while ( !CircuitPlayground.rightButton() ) {}

    return skill;
}

void GameWonLightShow() {
    LightThemAll( green );
    while ( true ) {
        _offColor = RandomColor( random( _nicePaletteCount ) );
        CircuitPlayground.setPixelColor( random( _leds ), _offColor.red, _offColor.green, _offColor.blue );
        delay(100);
        if ( CircuitPlayground.rightButton() || CircuitPlayground.leftButton() ) break;
    }

    CircuitPlayground.clearPixels();
    _done = 1;
}

void EffectLightingDifficulty( int skill ) {
    COLOR randomColor = RandomColor( random( _nicePaletteCount ) );

    switch ( skill % 3 ) {
        case 2: // insane, color confuses more than helps, must play by sound alone
            CircuitPlayground.setPixelColor( random( _leds ), randomColor.red, randomColor.green, randomColor.blue );
            break;
        case 1: // _offColor will be mildly confusing
            _offColor = randomColor;
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

bool LeftButton() { return CircuitPlayground.leftButton() || CircuitPlayground.readCap( _leftCapPad ) > _capThreshold; }
bool RightButton() { return CircuitPlayground.rightButton() || CircuitPlayground.readCap( _rightCapPad ) > _capThreshold; }

