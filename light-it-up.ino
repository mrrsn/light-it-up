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

void setup() {
    //Serial.begin(9600);
    randomSeed( analogRead(0) );
    CircuitPlayground.begin( _brightness );
    PlayMusic( _helloMelody, _helloMelodyLen );
    PlayGame();
    PlayMusic( _winMelody, _winMelodyLen );
    GameWonLightShow();
}

void loop() {
}

void PlayGame() {
    int skillJump = JumpToSkill();
    while ( !CircuitPlayground.rightButton() ) {}
    Countdown();

    for ( int skill = skillJump; skill < _maxSkill; skill++ ) {
        PlaySkill( skill );
    }
}

void PlaySkill( int skill ) {
    int timeWindow = _skillWindow[skill];
    int speedUp = (int)( timeWindow * _speedUpPct );

    for ( int level = 0; level < _levels; level++ ) {
        PlayLevel( skill, level, timeWindow );
        PlayMusic( _levelUpMelody, _levelUpMelodyLen );
        timeWindow -= speedUp;
    }
}

void PlayLevel( int skill, int level, int timeWindow ) {
    int led = 0;
    int change = 0;
    float partTime = 0.8f;
    bool currLevel[] = { false, false, false, false, false, false, false, false, false, false };

    while ( !IsFull( currLevel ) ) {
        CircuitPlayground.playTone(  currLevel[led] ? _onTune[led] : _offTune[led], (int)( timeWindow * partTime ), false /* i.e. don't block */ );
        CircuitPlayground.setPixelColor( led, _onColor.red, _onColor.green, _onColor.blue );
        EffectLightingDifficulty( skill );

        unsigned long previousMillis = millis();
        unsigned long currentMillis = millis();

        while( fabs( currentMillis - previousMillis ) < timeWindow ) {
            currentMillis = millis();
            //Serial.println(CircuitPlayground.readCap(1));
            //Serial.println(CircuitPlayground.readCap(12));
            if ( LeftButton() && led >= 5 ||
                 RightButton() && led < 5 ) {
                currLevel[led] = true;
            }
            else if ( LeftButton() && led < 5 ||
                      RightButton() && led >= 5 ) {
                LoseOne( currLevel );
            }
        }

        if ( !currLevel[led] ) {
            CircuitPlayground.setPixelColor( led, _offColor.red, _offColor.green, _offColor.blue );
        }

        if ( led == 0 ) change = 1;
        if ( led == 9 ) change = -1;

        led += change;
    }

    InitCurrentLevel( currLevel );
}

void LoseOne( bool currLevel[] ) {
    if ( IsEmpty( currLevel ) ) return;
    
    int p = random( _leds );
    while ( !currLevel[p] ) { p = random( _leds ); }

    currLevel[p] = false;
    CircuitPlayground.setPixelColor( p, _offColor.red, _offColor.green, _offColor.blue );

    CircuitPlayground.redLED( true );
    PlayMusic( _oopsMelody, _oopsMelodyLen );
    delay(250);
    CircuitPlayground.redLED( false );
}

void InitCurrentLevel( bool currLevel[] ) {
    _offColor = RandomColor( random( _nicePaletteCount ) );

    int i = _leds;
    while ( i --> 0 ) {
        currLevel[i] = false;
        CircuitPlayground.setPixelColor( i, _offColor.red, _offColor.green, _offColor.blue );
        delay(50);
    }
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
}

void EffectLightingDifficulty( int skill ) {
    COLOR randomColor = RandomColor( random( _nicePaletteCount ) );

    switch ( skill % 3 ) {
        case 2:
        case 1:
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

