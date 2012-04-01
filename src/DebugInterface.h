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
 * This class represents debug interface used in PianoBox project
 *
 *  Created on: Jan 31, 2012
 *      Author: Basil Shikin
 */

#ifndef DEBUGINTERFACE_H_
#define DEBUGINTERFACE_H_

#include "Arduino.h"


#define DEBUG_BAUD_RATE 9600
#define SERIAL_DEBUG true

#define FLASH_INTERVAL 200


class DebugInterface
{
public:
    DebugInterface();

    /**
     * Initialize debug interface
     */
    void initialize( Stream * stream, uint8_t rPin, uint8_t gPin, uint8_t bPin);

    /**
     * Blink debug LED given number of times
     */
    void blink( uint8_t times );

    /**
     * Set color of an LED
     */
    void setColor( uint32_t argb );

    /**
     * Write a debug message into program logs
     *
     * @param message Message to log
     */
    void debug( const char * message );

    /**
     * Write a debug message into program logs
     *
     * @param message Message to log
     * @param number  Number that is relevant to the message
     */
    void debug( const char * message, uint16_t number );

    /**
     * Write a debug message into program logs
     *
     * @param message Message to log
     * @param number  Number that is relevant to the message
     */
    void debug( const char * message, double number );

private:
    Stream * m_stream;
    uint8_t  m_rPin;
    uint8_t  m_gPin;
    uint8_t  m_bPin;

    uint8_t  m_rOn;
    uint8_t  m_gOn;
    uint8_t  m_bOn;
};

#endif /* DEBUGINTERFACE_H_ */
