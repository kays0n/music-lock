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
 * ServoController.cpp
 *
 *  Created on: Feb 4, 2012
 *      Author: Basil Shikin
 */

#include "ServoController.h"


void ServoController::initialize( uint8_t servoPin, uint8_t powerPin, DebugInterface * debugInterface )
{
    m_servoPin = servoPin;
    m_powerPin = powerPin;

    m_debugInterface = debugInterface;

    pinMode( m_powerPin, OUTPUT );
}

void ServoController::open()
{
    // Turn on servo
    digitalWrite( m_powerPin, LOW );
    delay( 2000 );

    m_servo.attach(m_servoPin);
    while (!m_servo.attached() );

    m_debugInterface->debug("Servo attached");

    for (int pos = 180; pos > 0; pos -= 1)
    {
        m_servo.write(pos);
        delay(15);
    }
    m_servo.detach();

    m_debugInterface->debug("Servo positioned");

    // Turn off servo
    delay( 200 );
    digitalWrite( m_powerPin, HIGH );
}

void ServoController::close()
{
    // Turn on servo
    digitalWrite( m_powerPin, LOW );
    delay( 2000 );

    m_servo.attach(m_servoPin);
    while (!m_servo.attached() );

    m_debugInterface->debug("Servo attached");

    for (int pos = 0; pos < 180; pos += 1)
    {
        m_servo.write(pos);
        delay(15);
    }
    m_servo.detach();

    m_debugInterface->debug("Servo positioned");

    // Turn off servo
    delay( 200  );
    digitalWrite( m_powerPin, HIGH );
}



ServoController::ServoController() {}


