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

#include "main.h"

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
    if (ptr)
        free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}


// Pin mappings
#define PIN_RED   9
#define PIN_GREEN 10
#define PIN_BLUE  11

#define PIN_MICROPHONE A0

#define PIN_LID  2

#define PIN_SERVO_CONTROL 3
#define PIN_SERVO_POWER 4

//static Note s_pattern[] = { NoteC4, NoteE3, NoteG5, NoteE4, NoteG4  };
static Note s_pattern[] = { NoteD5, NoteA4, NoteD5, NoteA4, NoteB4, NoteD5 };

static NoteDetector    s_noteDetector;
static ServoController s_servoController;
static DebugInterface  s_debugInterface;

void setup()
{
    // Initialize debug interface
    Serial.begin( DEBUG_BAUD_RATE );
    s_debugInterface.initialize( &Serial, PIN_RED, PIN_GREEN, PIN_BLUE );

    // Initialize note detector
    s_noteDetector.initialize( PIN_MICROPHONE, &s_debugInterface );
    s_noteDetector.setVolumeThreshold( 650 );

    // Initialize servo controller
    s_servoController.initialize( PIN_SERVO_CONTROL, PIN_SERVO_POWER, &s_debugInterface );

    // Initialize pins
    pinMode( PIN_LID, INPUT );
}

bool readLidClosed()
{
    return digitalRead( PIN_LID ) == LOW;
}

bool waitForLid(bool toClose, uint32_t timeout)
{
    // Wait for some time for the lid to close
    uint32_t timoutAt   = millis() + timeout;

    while ( millis() < timoutAt )
    {
        // If the lid has been closed
        if ( readLidClosed() == toClose )
        {
            return true;
        }

        delay( 300 );
    }

    return false;
}

void openLock()
{
    s_debugInterface.setColor( 0x00FF00 );

    // Open the lock
    s_debugInterface.debug("Opening the lock...");
    s_servoController.open();
}

void closeLock()
{
    s_debugInterface.setColor( 0x00FF00 );

    // Open the lock
    s_debugInterface.debug("Closing the lock...");
    s_servoController.close();
}

void attemptPatternRecognition()
{
    // Attempt to recognize a pattern
    s_debugInterface.debug("Recognizing pattern of ", sizeof( s_pattern )/2 );
    s_debugInterface.setColor( 0x0000FF );

    bool  patternRecognized = true;
    uint32_t timeout;
    for ( uint8_t noteIx = 0; noteIx < sizeof( s_pattern )/2; noteIx++ )
    {
        if ( noteIx == 0 ) timeout = 5000;
        else timeout = 1500;

        if ( s_noteDetector.detect( s_pattern[ noteIx ], timeout ) == 0 )
        {
            patternRecognized = false;
            break;
        }
    }

    if ( patternRecognized )
    {
        s_debugInterface.debug("Pattern recognized");

        openLock();

        // If lid is still closed after some time
        delay( 3000 );
        if ( readLidClosed() )
        {
            // Close the lock back
            closeLock();
        }
    }
    else
    {
        s_debugInterface.debug("Pattern not recognized");
        s_debugInterface.setColor( 0xFF0000 );
    }
}


void attemptLidClose()
{
    // Attempt close a lid
    s_debugInterface.debug("Waiting for lid to close" );
    s_debugInterface.setColor( 0x0000FF );

    if ( waitForLid( true, 5000 ) )
    {
        delay( 200 );

        closeLock();
    }

}
void lifecycle()
{
    bool lidIsClosed = readLidClosed();

    // If (lid is closed)
    if ( lidIsClosed )
    {
        attemptPatternRecognition();
    }

    // Otherwise (lid is open)
    else
    {
        attemptLidClose();
    }

    // Wait a little to make a statement
    delay( 1000 );
}

int main(void)
{
    // Initialize Arduino library
	init();

	// Setup ourselves
	setup();

	// Do the work
	lifecycle();

	// Sleep forever
	s_debugInterface.setColor( 0 );
	su_sleep();

	return 0;
}
