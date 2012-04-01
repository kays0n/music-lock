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
 * This class contains utilities for AVR deep sleeping
 *
 *  Created on: Feb 1, 2012
 *      Author: Basil Shikin
 */


#include "SleepUtils.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void su_sleep()
{
    // Stop serial communication
    Serial.end();

    // CPU Sleep Modes
     // SM2 SM1 SM0 Sleep Mode
     // 0    0  0 Idle
     // 0    0  1 ADC Noise Reduction
     // 0    1  0 Power-down
     // 0    1  1 Power-save
     // 1    0  0 Reserved
     // 1    0  1 Reserved
     // 1    1  0 Standby(1)

     cbi( SMCR,SE );      // sleep enable, power down mode
     cbi( SMCR,SM0 );     // power down mode
     sbi( SMCR,SM1 );     // power down mode
     cbi( SMCR,SM2 );     // power down mode

    ADCSRA |= (0<<ADEN); // Switch Analog to Digital converter OFF

    while ( true )
    {
        set_sleep_mode( SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }

    ADCSRA |= (1<<ADEN); // Switch Analog to Digital converter ON
}


