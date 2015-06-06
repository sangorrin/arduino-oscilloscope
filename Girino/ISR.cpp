//-----------------------------------------------------------------------------
// ISR.cpp
//-----------------------------------------------------------------------------
// Copyright 2012 Cristiano Lino Fontana
//
// This file is part of Girino.
//
//	Girino is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	Girino is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with Girino.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Girino.h"

//-----------------------------------------------------------------------------
// ADC Conversion Complete Interrupt
//-----------------------------------------------------------------------------
ISR(ADC_vect)
{
	// When ADCL is read, the ADC Data Register is not updated until ADCH
	// is read. Consequently, if the result is left adjusted and no more
	// than 8-bit precision is required, it is sufficient to read ADCH.
	// Otherwise, ADCL must be read first, then ADCH.
	ADCBuffer[ADCCounter] = ADCH;

	ADCCounter = ( ADCCounter + 1 ) % ADCBUFFERSIZE;

	if ( wait )
	{
		if ( stopIndex == ADCCounter )
		{
			// Freeze situation
			// Disable ADC and stop Free Running Conversion Mode
			cbi( ADCSRA, ADEN );

			freeze = true;
		}
	}
}

//-----------------------------------------------------------------------------
// Analog Comparator interrupt
//-----------------------------------------------------------------------------
ISR(ANALOG_COMP_vect)
{
	// Disable Analog Comparator interrupt
	cbi( ACSR,ACIE );

	// Turn on errorPin
	//digitalWrite( errorPin, HIGH );
	sbi( PORTB, PORTB5 );

	wait = true;
	stopIndex = ( ADCCounter + waitDuration ) % ADCBUFFERSIZE;
}
