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
/*
 * DebugInterface.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: Basil Shikin
 */

#include "DebugInterface.h"

DebugInterface::DebugInterface() {}

void DebugInterface::initialize( Stream * stream, uint8_t rPin, uint8_t gPin, uint8_t bPin)
{
    m_stream = stream;

    m_rPin = rPin;
    m_gPin = gPin;
    m_bPin = bPin;

    pinMode(m_rPin, OUTPUT);
    pinMode(m_gPin, OUTPUT);
    pinMode(m_bPin, OUTPUT);

    setColor( 0x0000FF );
    blink( 2 );
    setColor( 0x000000 );

    if ( SERIAL_DEBUG )
    {
        m_stream->println("");
        m_stream->println("----- Staring Piano Box -----");
        m_stream->println("");
    }
}
void DebugInterface::setColor( uint32_t argb )
{
    m_rOn = (argb >> 16) & 0xFF;
    m_gOn = (argb >> 8) & 0xFF;
    m_bOn = (argb >> 0) & 0xFF;

    digitalWrite(m_rPin, m_rOn > 0 ? HIGH : LOW);
    digitalWrite(m_gPin, m_gOn > 0 ? HIGH : LOW);
    digitalWrite(m_bPin, m_bOn > 0 ? HIGH : LOW);
}

void DebugInterface::blink( uint8_t times )
{
    for (uint8_t i = 0; i < times; i++)
    {
        digitalWrite(m_rPin, LOW);
        digitalWrite(m_gPin, LOW);
        digitalWrite(m_bPin, LOW);

        delay(FLASH_INTERVAL);

        digitalWrite(m_rPin, m_rOn > 0 ? HIGH : LOW);
        digitalWrite(m_gPin, m_gOn > 0 ? HIGH : LOW);
        digitalWrite(m_bPin, m_bOn > 0 ? HIGH : LOW);

        delay(FLASH_INTERVAL);
    }
}

void DebugInterface::debug( const char * message )
{
    if ( SERIAL_DEBUG )
    {
        m_stream->println( message );
    }
}

void DebugInterface::debug( const char * message, uint16_t number )
{
    if ( SERIAL_DEBUG )
    {
        m_stream->print( message );
        m_stream->println( number, DEC);
    }
}

void DebugInterface::debug( const char * message, double number )
{
    if ( SERIAL_DEBUG )
    {
        m_stream->print( message );
        m_stream->println( number, 6 );
    }
}
