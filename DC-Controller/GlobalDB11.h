/*
 * GlobalDB11.h
 *
 *  Created on: 3 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 *      	First version that only use Object oriented DB. GlobalDB task in main app is still C, and handles the Queue
 *      	Needs another class for communication to GlobalDB-task from other tasks.
 *      Version 1.1
 *      	Try to do a slim version only for DCC
 */

#ifndef GLOBALDB11_HPP_
#define GLOBALDB11_HPP_


#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


struct GlobalVarStruct
{
	int	Seconds;
	int	DCC_pwm;										// DC-Controller status
	int	DCC_rpm;
	int	DCC_tps_pos;
	int	DCC_tps_raw;
	int	DCC_out_amp;
	int	DCC_out_amp_raw;
	int	DCC_in_volt;
	int	DCC_in_amp;
	int	DCC_temp;
	int	DCC_temp_raw;
	int	DCC_driver_volt;
};

class GlobalDB {
public:
	GlobalDB();								// Set up comm to GlobalDB
	virtual ~GlobalDB();
	void Set_var(int ID,int Value);		// Update global variable "ID" with value "Value"
	int Get_var(int ID);					// Get Global Variable "ID". Returns "Value"

	void Set_index(int Index,char Value);		// Update global variable "ID" with value "Value"
	char Get_index(int Index);					// Get Global Variable "ID". Returns "Value"

	int test();
	const int ChargeMaxVoltIndex = 0;
	const int ChargeMaxAmpIndex = 2;
	const int ChargeMaxBatteryTempIndex = 4;
	const int ChargeMinBatteryTempIndex = 6;
	const int CharteMagTimeMinIndex = 8;
	const int ChargeFastChargeCutoffAmpIndex = 10;
	const int ChargeStateIndex = 12;
private:
	GlobalVarStruct Global_vars;

};

#endif /* GLOBALDB11_HPP_ */
