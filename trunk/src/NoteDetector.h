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
 * This class is used to detect various notes. Guaranteed detection range is from C3 to C6.
 *
 *  Created on: Jan 29, 2012
 *      Author: Basil Shikin
 */

#ifndef NOTEDETECTOR_H_
#define NOTEDETECTOR_H_

#include "Arduino.h"
#include "DebugInterface.h"

/**
 * This enum contains supported notes. Value of enum objects is note
 * frequency in Hz.
 */
enum Note {
    NoteC3  = 131,
    NoteC3s = 138,
    NoteD3  = 147,
    NoteD3s = 155,
    NoteE3  = 164,
    NoteF3  = 174,
    NoteF3s = 185,
    NoteG3  = 196,
    NoteG3s = 207,
    NoteA3  = 220,
    NoteA3s = 233,
    NoteB3  = 247,

    NoteC4  = 262,
    NoteC4s = 277,
    NoteD4  = 294,
    NoteD4s = 311,
    NoteE4  = 330,
    NoteF4  = 349,
    NoteF4s = 370,
    NoteG4  = 392,
    NoteG4s = 415,
    NoteA4  = 440,
    NoteA4s = 466,
    NoteB4  = 494,

    NoteC5  = 523,
    NoteC5s = 554,
    NoteD5  = 587,
    NoteD5s = 622,
    NoteE5  = 659,
    NoteF5  = 698,
    NoteF5s = 740,
    NoteG5  = 784,
    NoteG5s = 831,
    NoteA5  = 880,
    NoteA5s = 932,
    NoteB5  = 988
};

class NoteDetector
{
public:
	/**
	 * Create a new detector.
	 */
	NoteDetector();

	/**
	 * Detect a given note.
	 *
	 *     frequency  Frequency of a note to detect. It is recommended to use a value
	 *                from Note enum.
	 *
	 *     timeout    Number of milliseconds after which to terminate the detection
	 *
	 *   return frequency of detected note or 0 if nothing detected.
	 */
	uint16_t detect( uint16_t frequency, uint32_t timeout );

	/**
	 * Set threshold of volume after which detection begins.
	 *
     *     volumeThreshold  Number 0-1024 that identifies how loud the sound should be to start
     *                      note detection.
     *
	 */
	void setVolumeThreshold( int16_t volumeThreshold );

	/**
	 * Initialize note detector.
	 *
	 *     inputPin  Pin to read microphone input from.
	 *     debugInterface Inteface to use for debug output.
	 */
	void initialize( uint8_t inputPin, DebugInterface * debugInterface  );

	~NoteDetector();

private:
	int16_t m_volumeThreshold;
	uint8_t  m_inputPin;

	DebugInterface * m_debugInterface;

	inline int16_t readPin();
};

#endif /* NOTEDETECTOR_H_ */
