/*
 * Pot10.cpp
 *
 *  Created on: 4 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 */

#include "Pot10.h"


Pot::Pot(unsigned int pin)
{
	Analog_in = pin;
	pinMode(Analog_in,INPUT);
	Max_raw_value = 598;
	Min_raw_value = 281;
	Last_adc_value = 0;

}

Pot::~Pot() {
	// TODO Auto-generated destructor stub
}

unsigned int	Pot::getTPS()
{
	// Reads volt on pin, check for errors and calculate TPS according to setup variables.

	const long l=281,h=598;
	unsigned int  Raw_value;
	unsigned int Tps_1 = 0;
	Filter Raw(13);									// Use 13 position buffer
	for(int i=0;i<13;i++)							// Fill all potitions with newly read values
	{
		Raw_value = analogRead(Analog_in);			// Read ADC-Value
		Raw.addValue(Raw_value);					// put in to filter buffer
	}
	Raw_value = Raw.getFilteredValue();				// Get aveage value
	Raw_value = 1023 - Raw_value;					// Switch so higher value = more acceleration
	Last_adc_value = Raw_value;						// Store ADC value.

    Tps_1 = map(Raw_value, Min_raw_value, Max_raw_value, 0, 100);
    return(Tps_1);
}
void			Pot::setFullTPS(unsigned int value)
{
	Max_raw_value = value;
}

void			Pot::setZeroTPS(unsigned int value)
{
	Min_raw_value = value;
}

unsigned int 	Pot::getLastADCValue()
{
    return (Last_adc_value);
}

unsigned int 	Pot::mapint(unsigned int x, unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
