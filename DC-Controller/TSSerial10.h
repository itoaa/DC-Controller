/*
 * TSSerial10.h
 *
 *  Created on: 4 sep. 2017
 *      Author: Ola Andersson
 *      TunderStudio Serial Class
 *      Version 1.0
 */

#ifndef TSSERIAL10_H_
#define TSSERIAL10_H_
#include <Arduino.h>
#include "DBQuery.h"
#include <stdint.h>

#define CodeString  F("MSII DCC 2.0        ")       // String sent to TuningStudio, use F() macro to store in progmem, not in sram.
#define CodeRev  F("DCC       2.0")                 // Code Revition to send to Megatune.
#define RTPS  80                                    // Real time data page size
#define PG1S  78         							// Eprom Page1 size


class TSSerial {
public:
	TSSerial(QueueHandle_t s_q);
	virtual ~TSSerial();

	void Send_sec(int Sec);							// Send number of seconds passed
	void Send_rev(char str[20]);							// Send code revision.
	void Send_eprom_var();							// Send EEprom variables to serial.
	void Get_eprom_var();							// Get EEprom variable from serial.
	void Send_rpage();								// Send real time data
private:
	QueueHandle_t Serial_q;
};

#endif /* TSSERIAL10_H_ */
