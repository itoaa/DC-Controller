/*
 * SDC-30a-HW.h
 *
 *  Created on: 2 sep. 2017
 *      Author: Ola Andersson
 *
 *      All definitions spesific to SDC-30a PCB
 */

#ifndef SDC_30A_HW_H_
#define SDC_30A_HW_H_


// Setup Hardware constatns for Smedby DC Controller rev a PCB with CAN-Bus
// *************************************************************************************************
#ifdef  SDC30a					// Parameter definition for SmedbyCharger 1.1 without CAN buss.
  #define     SerialEnabled				// Enable serial ouput on this hardware.
  #define     BaudRate 115200			// Sepeed for cerial comunication
  int  InputVoltPin  	= 	PC4;			// Pin used to measure Battery-volt
  int  OutputCurrentPin 	= 	PC1;			// Pin used to measure Motor-volt
  int  OutputVoltPin 	= 	PC2;			// Try to measure output-volt
  int  DriverVoltPin 	= 	PC3;			// Pin used to measure 12 volt rail
  int  TemperaturPin 	= 	PC5;			// Pin used to measure mosfet-temperatur
  int  PotPin 			= 	A0;			// Change to PCx pin
  int  pwmPin 			= 	PD6;			// Pin used for PWM-charge-output
  int  Temp_Measure_type = 	1;			// Type 1 = NTC-resistor, 2 = DS1820 temp sensor.
  int  Current_Measure_type = 3;			// Type 1 = 5A, 2 = 20A, 3 = 30A              ( Defines witch type of current sense sensor is used.

 #endif




#endif /* SDC_30A_HW_H_ */
