/*
 * GlobalDB.hpp
 *
 *  Created on: 3 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 *      	First version that only use Object oriented DB. GlobalDB task in main app is still C, and handles the Queue
 *      	Needs another class for communication to GlobalDB-task from other tasks.
 */

#ifndef GLOBALDB10_HPP_
#define GLOBALDB10_HPP_


#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


struct GlobalVarStruct
{
	int Seconds;
	int Charge_max_volt;
	int	Charge_max_amp;					// ID är 4.
	int	Charge_max_battery_temp;
	int Charge_min_battery_temp;
	int Charte_max_time_min;
	int Charge_fastcharge_cutoff_amp;

	int	Charger_state;
	int Charger_pwm;
	int	Charger_out_volt;
	int Charger_out_amp;
	int Charger_temp1;
	int Charger_external_temp1;
	int	Charger_in_volt;
	int Charger_in_amp;

	int BP_max_charge_volt;							// Batterypack constatnts
	int BP_max_charge_amp;
	int BP_max_charge_temp;
	int BP_min_charge_temp;
	int BP_max_discharge_amp;
	int BP_max_discharge_temp;
	int BP_min_discharge_temp;
	int BP_min_discharge_volt;

	int BP_volt;										// Battreypack status
	int BP_amp;
	int BP_temp;
	int BP_cell1_volt;
	int BP_cell2_volt;
	int BP_cell3_volt;

	int	DCC_pwm;										// DC-Controller status
	int	DCC_rpm;
	int	DCC_tps_pos;
	int	DCC_tps_raw;
	int	DCC_out_amp;
	int DCC_out_amp_raw;
	int	DCC_in_volt;
	int DCC_in_amp;
	int DCC_temp;
	int DCC_temp_raw;
	int DCC_driver_volt;
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

#endif /* GLOBALDB10_HPP_ */
