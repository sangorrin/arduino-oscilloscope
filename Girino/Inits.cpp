//-----------------------------------------------------------------------------
// Inits.cpp
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
// initPins()
//-----------------------------------------------------------------------------
void initPins(void)
{
	//---------------------------------------------------------------------
	// TCCR2A settings
	//---------------------------------------------------------------------
	// These bits control the Output Compare pin (OC2A) behavior. If one or
	// both of the COM2A1:0 bits are set, the OC2A output overrides the
	// normal port functionality of the I/O pin it is connected to.
	// However, note that the Data Direction Register (DDR) bit
	// corresponding to the OC2A pin must be set in order to enable the
	// output driver.
	// When OC2A is connected to the pin, the function of the COM2A1:0 bits
	// depends on the WGM22:0 bit setting.
	//
	// Fast PWM Mode
	//	COM2A1	COM2A0
	//	0	0	Normal port operation, OC2A disconnected.
	//	0	1	WGM22 = 0: Normal Port Operation, OC0A Disconnected.
	//			WGM22 = 1: Toggle OC2A on Compare Match.
	//	1	0	Clear OC2A on Compare Match, set OC2A at BOTTOM
	//	1	1	Clear OC2A on Compare Match, clear OC2A at BOTTOM
	cbi(TCCR2A,COM2A1);
	cbi(TCCR2A,COM2A0);
	sbi(TCCR2A,COM2B1);
	cbi(TCCR2A,COM2B0);

	// Combined with the WGM22 bit found in the TCCR2B Register, these bits
	// control the counting sequence of the counter, the source for maximum
	// (TOP) counter value, and what type of waveform generation to be used
	// Modes of operation supported by the Timer/Counter unit are:
	//	- Normal mode (counter),
	//	- Clear Timer on Compare Match (CTC) mode,
	//	- two types of Pulse Width Modulation (PWM) modes.
	//
	//	Mode	WGM22	WGM21	WGM20	Operation	TOP
	//	0	0	0	0	Normal		0xFF
	//	1	0	0	1	PWM		0xFF
	//	2	0	1	0	CTC		OCRA
	//	3	0	1	1	Fast PWM	0xFF
	//	4	1	0	0	Reserved	-
	//	5	1	0	1	PWM		OCRA
	//	6	1	1	0	Reserved	-
	//	7	1	1	1	Fast PWM	OCRA
	cbi(TCCR2B,WGM22);
	sbi(TCCR2A,WGM21);
	sbi(TCCR2A,WGM20);

	//---------------------------------------------------------------------
	// TCCR2B settings
	//---------------------------------------------------------------------
	// The FOC2A bit is only active when the WGM bits specify a non-PWM
	// mode.
	// However, for ensuring compatibility with future devices, this bit
	// must be set to zero when TCCR2B is written when operating in PWM
	// mode. When writing a logical one to the FOC2A bit, an immediate
	// Compare Match is forced on the Waveform Generation unit. The OC2A
	// output is changed according to its COM2A1:0 bits setting. Note that
	// the FOC2A bit is implemented as a strobe. Therefore it is the value
	// present in the COM2A1:0 bits that determines the effect of the
	// forced compare.
	// A FOC2A strobe will not generate any interrupt, nor will it clear
	// the timer in CTC mode using OCR2A as TOP.
	// The FOC2A bit is always read as zero.
	cbi(TCCR2B,FOC2A);
	cbi(TCCR2B,FOC2B);

	// The three Clock Select bits select the clock source to be used by
	// the Timer/Counter.
	//	CS22	CS21	CS20	Prescaler
	//	0	0	0	No clock source (Timer/Counter stopped).
	//	0	0	1	No prescaling
	//	0	1	0	8
	//	0	1	1	32
	//	1	0	0	64
	//	1	0	1	128
	//	1	1	0	256
	//	1	1	1	1024
	cbi(TCCR2B,CS22);
	cbi(TCCR2B,CS21);
	sbi(TCCR2B,CS20);

	pinMode( errorPin, OUTPUT );
	pinMode( thresholdPin, OUTPUT );

	analogWrite( thresholdPin, 127 );
}
  
//-----------------------------------------------------------------------------
// initADC()
//-----------------------------------------------------------------------------
void initADC(void)
{
	//---------------------------------------------------------------------
	// ADMUX settings
	//---------------------------------------------------------------------
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
	cbi(ADMUX,REFS1);
	sbi(ADMUX,REFS0);
	// The ADLAR bit affects the presentation of the ADC conversion result
	// in the ADC Data Register. Write one to ADLAR to left adjust the
	// result. Otherwise, the result is right adjusted. Changing the ADLAR
	// bit will affect the ADC Data Register immediately, regardless of any
	// ongoing conversions.
	sbi(ADMUX,ADLAR);
	// The value of these bits selects which analog inputs are connected to
	// the ADC. If these bits are changed during a conversion, the change
	// will not go in effect until this conversion is complete (ADIF in
	// ADCSRA is set).
	ADMUX |= ( ADCPIN & 0x07 );

	//---------------------------------------------------------------------
	// ADCSRA settings
	//---------------------------------------------------------------------
	// Writing this bit to one enables the ADC. By writing it to zero, the
	// ADC is turned off. Turning the ADC off while a conversion is in
	// progress, will terminate this conversion.
	cbi(ADCSRA,ADEN);
	// In Single Conversion mode, write this bit to one to start each
	// conversion. In Free Running mode, write this bit to one to start the
	// first conversion. The first conversion after ADSC has been written
	// after the ADC has been enabled, or if ADSC is written at the same
	// time as the ADC is enabled, will take 25 ADC clock cycles instead of
	// the normal 13. This first conversion performs initialization of the
	// ADC. ADSC will read as one as long as a conversion is in progress.
	// When the conversion is complete, it returns to zero. Writing zero to
	// this bit has no effect.
	cbi(ADCSRA,ADSC);
	// When this bit is written to one, Auto Triggering of the ADC is
	// enabled. The ADC will start a conversion on a positive edge of the
	// selected trigger signal. The trigger source is selected by setting
	// the ADC Trigger Select bits, ADTS in ADCSRB.
	sbi(ADCSRA,ADATE);
	// When this bit is written to one and the I-bit in SREG is set, the
	// ADC Conversion Complete Interrupt is activated.
	sbi(ADCSRA,ADIE);
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
	sbi(ADCSRA,ADPS2);
	sbi(ADCSRA,ADPS1);
	sbi(ADCSRA,ADPS0);

	//---------------------------------------------------------------------
	// ADCSRB settings
	//---------------------------------------------------------------------
	// When this bit is written logic one and the ADC is switched off
	// (ADEN in ADCSRA is zero), the ADC multiplexer selects the negative
	// input to the Analog Comparator. When this bit is written logic zero,
	// AIN1 is applied to the negative input of the Analog Comparator.
	cbi(ADCSRB,ACME);
	// If ADATE in ADCSRA is written to one, the value of these bits
	// selects which source will trigger an ADC conversion. If ADATE is
	// cleared, the ADTS2:0 settings will have no effect. A conversion will
	// be triggered by the rising edge of the selected Interrupt Flag. Note
	// that switching from a trigger source that is cleared to a trigger
	// source that is set, will generate a positive edge on the trigger
	// signal. If ADEN in ADCSRA is set, this will start a conversion.
	// Switching to Free Running mode (ADTS[2:0]=0) will not cause a
	// trigger event, even if the ADC Interrupt Flag is set.
	//	ADTS2	ADTS1	ADTS0	Trigger source
	//	0	0	0	Free Running mode
	//	0	0	1	Analog Comparator
	//	0	1	0	External Interrupt Request 0
	//	0	1	1	Timer/Counter0 Compare Match A
	//	1	0	0	Timer/Counter0 Overflow
	//	1	0	1	Timer/Counter1 Compare Match B
	//	1	1	0	Timer/Counter1 Overflow
	//	1	1	1	Timer/Counter1 Capture Event
	cbi(ADCSRB,ADTS2);
	cbi(ADCSRB,ADTS1);
	cbi(ADCSRB,ADTS0);

	//---------------------------------------------------------------------
	// DIDR0 settings
	//---------------------------------------------------------------------
	// When this bit is written logic one, the digital input buffer on the
	// corresponding ADC pin is disabled. The corresponding PIN Register
	// bit will always read as zero when this bit is set. When an analog
	// signal is applied to the ADC5..0 pin and the digital input from this
	// pin is not needed, this bit should be written logic one to reduce
	// power consumption in the digital input buffer.
	// Note that ADC pins ADC7 and ADC6 do not have digital input buffers,
	// and therefore do not require Digital Input Disable bits.
	sbi(DIDR0,ADC5D);
	sbi(DIDR0,ADC4D);
	sbi(DIDR0,ADC3D);
	sbi(DIDR0,ADC2D);
	sbi(DIDR0,ADC1D);
	sbi(DIDR0,ADC0D);
}

//-----------------------------------------------------------------------------
// initAnalogComparator()
//-----------------------------------------------------------------------------
void initAnalogComparator(void)
{
	//---------------------------------------------------------------------
	// ACSR settings
	//---------------------------------------------------------------------
	// When this bit is written logic one, the power to the Analog
	// Comparator is switched off. This bit can be set at any time to turn
	// off the Analog Comparator. This will reduce power consumption in
	// Active and Idle mode. When changing the ACD bit, the Analog
	// Comparator Interrupt must be disabled by clearing the ACIE bit in
	// ACSR. Otherwise an interrupt can occur when the bit is changed.
	cbi(ACSR,ACD);
	// When this bit is set, a fixed bandgap reference voltage replaces the
	// positive input to the Analog Comparator. When this bit is cleared,
	// AIN0 is applied to the positive input of the Analog Comparator. When
	// the bandgap referance is used as input to the Analog Comparator, it
	// will take a certain time for the voltage to stabilize. If not
	// stabilized, the first conversion may give a wrong value.
	cbi(ACSR,ACBG);
	// When the ACIE bit is written logic one and the I-bit in the Status
	// Register is set, the Analog Comparator interrupt is activated.
	// When written logic zero, the interrupt is disabled.
	cbi(ACSR,ACIE);
	// When written logic one, this bit enables the input capture function
	// in Timer/Counter1 to be triggered by the Analog Comparator. The
	// comparator output is in this case directly connected to the input
	// capture front-end logic, making the comparator utilize the noise
	// canceler and edge select features of the Timer/Counter1 Input
	// Capture interrupt. When written logic zero, no connection between
	// the Analog Comparator and the input capture function exists. To
	// make the comparator trigger the Timer/Counter1 Input Capture
	// interrupt, the ICIE1 bit in the Timer Interrupt Mask Register
	// (TIMSK1) must be set.
	cbi(ACSR,ACIC);
	// These bits determine which comparator events that trigger the Analog
	// Comparator interrupt.
	//	ACIS1	ACIS0	Mode
	//	0	0	Toggle
	//	0	1	Reserved
	//	1	0	Falling edge
	//	1	1	Rising edge
	sbi(ACSR,ACIS1);
	sbi(ACSR,ACIS0);

	//---------------------------------------------------------------------
	// DIDR1 settings
	//---------------------------------------------------------------------
	// When this bit is written logic one, the digital input buffer on the
	// AIN1/0 pin is disabled. The corresponding PIN Register bit will
	// always read as zero when this bit is set. When an analog signal is
	// applied to the AIN1/0 pin and the digital input from this pin is not
	// needed, this bit should be written logic one to reduce power
	// consumption in the digital input buffer.
	sbi(DIDR1,AIN1D);
	sbi(DIDR1,AIN0D);
}
