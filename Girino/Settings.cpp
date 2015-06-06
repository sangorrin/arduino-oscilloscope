//-----------------------------------------------------------------------------
// Settings.cpp
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
// Start elements
//-----------------------------------------------------------------------------
void startADC( void )
{
	// Enable ADC
	sbi(ADCSRA,ADEN);
	// Start conversion
	sbi(ADCSRA,ADSC);
}
void stopADC( void )
{
	// Disable ADC and stop Free Running Conversion Mode
	cbi(ADCSRA,ADEN);
}

//-----------------------------------------------------------------------------
// startAnalogComparator()
//-----------------------------------------------------------------------------
void startAnalogComparator( void )
{
	// Enable Analog Comparator Interrupt
	sbi(ACSR,ACIE);
}
void stopAnalogComparator( void )
{
	// Disable Analog Comparator interrupt
	cbi( ACSR,ACIE );
}

//-----------------------------------------------------------------------------
// Set and modify ADC prescaler
//-----------------------------------------------------------------------------
void setADCPrescaler( uint8_t Prescaler )
{
	dshow("# setADCPrescaler()");
	dprint(Prescaler);
	// These bits determine the division factor between the system clock
	// frequency and the input clock to the ADC.
	//	ADPS2	ADPS1	ADPS0	Division Factor
	//	0	0	0	2
	//	0	0	1	2
	//	0	1	0	4
	//	0	1	1	8
	//	1	0	0	16
	//	1	0	1	32
	//	1	1	0	64
	//	1	1	1	128
	switch (Prescaler)
	{
	case 2:
		cbi(ADCSRA,ADPS2);
		cbi(ADCSRA,ADPS1);
		sbi(ADCSRA,ADPS0);
		break;
	case 4:
		cbi(ADCSRA,ADPS2);
		sbi(ADCSRA,ADPS1);
		cbi(ADCSRA,ADPS0);
		break;
	case 8:
		cbi(ADCSRA,ADPS2);
		sbi(ADCSRA,ADPS1);
		sbi(ADCSRA,ADPS0);
		break;
	case 16:
		sbi(ADCSRA,ADPS2);
		cbi(ADCSRA,ADPS1);
		cbi(ADCSRA,ADPS0);
		break;
	case 32:
		sbi(ADCSRA,ADPS2);
		cbi(ADCSRA,ADPS1);
		sbi(ADCSRA,ADPS0);
		break;
	case 64:
		sbi(ADCSRA,ADPS2);
		sbi(ADCSRA,ADPS1);
		cbi(ADCSRA,ADPS0);
		break;
	case 128:
		sbi(ADCSRA,ADPS2);
		sbi(ADCSRA,ADPS1);
		sbi(ADCSRA,ADPS0);
		break;
	default:	// Set 128
		sbi(ADCSRA,ADPS2);
		sbi(ADCSRA,ADPS1);
		sbi(ADCSRA,ADPS0);
	}
}

//-----------------------------------------------------------------------------
// Set and modify Voltage Reference
//-----------------------------------------------------------------------------
void setVoltageReference( uint8_t reference )
{
	dshow("# setVoltageReference()");
	dprint(reference);
	// These bits select the voltage reference for the ADC. If these bits
	// are changed during a conversion, the change will not go in effect
	// until this conversion is complete (ADIF in ADCSRA is set). The
	// internal voltage reference options may not be used if an external
	// reference voltage is being applied to the AREF pin.
	//	REFS1	REFS0	Voltage reference
	//	0	0	AREF, Internal Vref turned off
	//	0	1	AVCC with external capacitor at AREF pin
	//	1	0	Reserved
	//	1	1	Internal 1.1V Voltage Reference with external
	//			capacitor at AREF pin
	switch (reference)
	{
	case 0:
		cbi(ADMUX,REFS1);
		cbi(ADMUX,REFS0);
		break;
	case 2:
		sbi(ADMUX,REFS1);
		sbi(ADMUX,REFS0);
		break;
	case 1:
	default:
		cbi(ADMUX,REFS1);
		sbi(ADMUX,REFS0);
	}
}

//-----------------------------------------------------------------------------
void setTriggerEvent( uint8_t TriggerEvent )
{
	dshow("# setTriggerEvent()");
	dprint(event);
	// These bits determine which comparator events that trigger the Analog
	// Comparator interrupt.
	//	ACIS1	ACIS0	Mode
	//	0	0	Toggle
	//	0	1	Reserved
	//	1	0	Falling edge
	//	1	1	Rising edge
	switch (TriggerEvent)
	{
	case 0:
		cbi(ACSR,ACIS1);
		cbi(ACSR,ACIS0);
		break;
	case 2:
		sbi(ACSR,ACIS1);
		cbi(ACSR,ACIS0);
		break;
	case 3:
	default:
		sbi(ACSR,ACIS1);
		sbi(ACSR,ACIS0);
	}
}
