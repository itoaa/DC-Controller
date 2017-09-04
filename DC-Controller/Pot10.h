/*
 * Pot10.h
 *
 *  Created on: 4 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 */
#include <Arduino.h>
#include "Filter10.h"

#ifndef POT10_H_
#define POT10_H_

class Pot {
public:
    Pot( unsigned int pin);								// pin = arduino pin where pot-box is connected.
	virtual ~Pot();
	unsigned int    getTPS();								// Get tps value. 0 - 100
	unsigned int    getLastADCValue();						// Get ADC value from last getTPS();
	void            setZeroTPS(unsigned int);				// set value for zero TPS. Pedal not pressed.
	void            setFullTPS(unsigned int);				// set value for full TPS. Full throtle


private:
	unsigned int	Analog_in;
	int				Max_raw_value;							// Max value to read if everyting is working.
	int				Min_raw_value;							// Min value to read if everyting is working.
	unsigned int	Last_adc_value;
	unsigned int mapint(unsigned int x, unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max);

};

#endif /* POT10_H_ */
