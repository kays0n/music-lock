/**
 * Copyright (c) 2012 Basil Shikin, PianoBox Project
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "NoteDetector.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define SAMPLE_RATE  22260.8
//#define SAMPLE_RATE  22505.0
#define SAMPLE_COUNT 2048

NoteDetector::NoteDetector() {
}

NoteDetector::~NoteDetector() {
}

uint16_t NoteDetector::detect( uint16_t frequency, uint32_t timeout )
{
    //
    // This implementation of Goertzel algorithm
    // is taken from Wikipedia (http://en.wikipedia.org/wiki/Goertzel_algorithm).
    //
    double curr     = 0;
    double prev     = 0;
    double prevPrev = 0;
    double result   = 0;

    double sampleRate = SAMPLE_RATE;
    double targetFrequeny = (double)frequency;

    double normalizedFrequency = targetFrequeny / sampleRate;
    double coeff               = 2*cos(2*PI*normalizedFrequency);

    m_debugInterface->debug("Target frequency: ", targetFrequeny );
    m_debugInterface->debug("Normalized frequency: ", normalizedFrequency );

    uint32_t timoutAt   = millis() + timeout;
    uint16_t sampleNo = 0;
    int16_t  reading  = 0;

    // Wait until there is a loud sound

    while ( millis() < timoutAt )
    {
        while ( readPin() < m_volumeThreshold && millis() < timoutAt );

        sampleNo = 0;
        prev     = 0;
        prevPrev = 0;

        while ( sampleNo < SAMPLE_COUNT )
        {
            reading = readPin();

            curr     = reading + coeff*prev - prevPrev;

            prevPrev = prev;
            prev     = curr;

            sampleNo += 1;
        }

        result = prevPrev*prevPrev + prev*prev - coeff*prev*prevPrev;

        // If our frequency is prominent enough return it
        if ( result/10000000 > 200.0  )


        {
            m_debugInterface->debug("Detected ", frequency );
            m_debugInterface->debug("Result ", result/10000000 );

            return frequency;
        }
    }

    m_debugInterface->debug("Nothing detected" );
    // Timed out, nothing detected
    return 0;
}

void NoteDetector::initialize( uint8_t inputPin, DebugInterface * debugInterface )
{
    m_inputPin = inputPin;
    m_debugInterface = debugInterface;

    // Use Vcc as reference voltage
    sbi(ADMUX, REFS0);

    // Enable ADC
    sbi(ADCSRA, ADEN);

    // Set pre-scaler to 16
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);

    m_debugInterface->debug("Note detector initialized");
}

void NoteDetector::setVolumeThreshold( int16_t volumeThreshold )
{
    m_volumeThreshold = volumeThreshold;
}

int16_t inline NoteDetector::readPin()
{
    return analogRead( m_inputPin );
}

