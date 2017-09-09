#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "GlobalDB10.hpp"
#include "DBQuery.h"
#include "Volt20.h"
#include "Current20.h"
#include "Filter10.h"
#include "Pot10.h"
#include "TSSerial10.h"

/* ------------------------------------------------------------------
 * CD-Controller ver 2 utvecklas i Eclipse och byter kernel från
 * OS48 till FreeRTOS V9
 * Version 2.0 kommer att ta ett nytt grepp om globala variabler och
 * ersätta dessa med en databas-process som övriga processer kan kommunisera
 * med för att få tillgång till data i systemet.
 */

// Define for witch hardware to compile
 #define SDC30a
// #define evFartreg-proto

#include "SDC-30a-HW.h"

// Define objects used
Volt Volt_in(InputVoltPin,6400  );                        // Pin to measure inputvoltage, Max Voltage * 100
Pot Pot(PotPin);                                              // Pin to measure pot position, 0-100.
Current Out_current(OutputCurrentPin,3);                        // Pin to measure outeput current .
//mEEPROM storage;
//Temperature mosfetTemp(TemperaturPin,2);      // pin, Type 1. (type not implemented yet, only DS18x support).
Filter Driver_temp_filtered(13);
Filter Tps_filtered(7);

//RPageVarsStruct RPage;
//Page1DataStruct pg1;

//MegaTuneSerial mySerial(BaudRate);

QueueHandle_t Global_db_q;

void Serial_task(void *pvParameters);
void Global_db_task(void *pvParameters);
void DCC_task(void *pvParameters);

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = (TCCR0B & 0b11111000) | mode;
    } else {
      TCCR1B = (TCCR1B & 0b11111000) | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = (TCCR2B & 0b11111000) | mode;
  }
}

void setup()
{
//    analogReference(EXTERNAL);					// Set ADC reference voltage to external reference

 //   pinMode(pwmPin, OUTPUT);					// sets pwm pin as output
//    setPwmFrequency(pwmPin, 1);					// pin,prescaler   pin9 default freq 32k.
    // setPwmFrequency(pwmPin, 8);					// pin,prescaler   pin9 default freq 3.9k.

    Serial.begin(BaudRate);
	while (!Serial)  { ; }						// wait for serial port to connect.
	Serial.println("serup...");
	Global_db_q = xQueueCreate(4,sizeof(Queue_struct));

	xTaskCreate(
	    Serial_task
	    ,  (const portCHAR *)"SerialTask"   // A name just for humans
	    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
	    ,  NULL
	    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	    ,  NULL );

	xTaskCreate(
	    Global_db_task
	    ,  (const portCHAR *)"GlobalDBTask"   // A name just for humans
	    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
	    ,  NULL
	    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	    ,  NULL );


	xTaskCreate(
	    DCC_task
	    ,  (const portCHAR *)"DCCTask"   // A name just for humans
	    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
	    ,  NULL
	    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	    ,  NULL );


}
void loop() {

}

void Serial_task(void *pvParameters)
{
	(void) pvParameters;
	const  TickType_t Ms_delay = 1 / portTICK_PERIOD_MS;	  // Set Ms_delay to one msec.
	char Serial_byte;
	int Sec = 0;
/*
	Global_db_set(1,1274);
	Global_db_set(2,2274);
	Global_db_set(3,3274);
*/
	TSSerial mySerial(115200);
	for (;;) // A Task shall never return or exit.
	{
		if (Serial.available() > 0)
		{
			Serial_byte = Serial.read();          // get the new byte:
			switch(Serial_byte)                // Evaluate resived data
			{
				case 'A':
					mySerial.Send_rpage();                    // If "A" is resived, send Realtimedata
					break;
				case 'C':
					mySerial.Send_sec(Sec);               // If "C" is resived, send Seconds (comm test)
					break;
				case 'S':
					mySerial.Send_rev("DCC       2.0");                     // If "S" is resived, send firmware version.
					break;
				case 'Q':
					mySerial.Send_rev("MSII DCC 2.0        ");                  // If "Q" is resived, send code string. Used to determing serial protocol.
					break;
				case 'F':
					mySerial.Send_rev("001");                       // If "F" is resived, send serial protocol ver??
					break;
				case 'W':
					mySerial.Get_eprom_var();
					break;
				case 'B':
//					storage.saveConfig(pg1);                        // If "B" is resived, write pg1_var to EEPROM.
					break;
				case 'V':
					mySerial.Send_eprom_var();                   // If "V" is resived, send pg1_var to tunerstudio.
					break;

				default: break;
			}

		}
		vTaskDelay(Ms_delay*10);
		++Sec;
	}
}


void Global_db_task(void *pvParameters)
{
	// Task to manage GlobalDatabase (GlobalDB). Uses class DB to store data.
	// Class DBQueue is used for querying GlobalDB.
	// Queue Global_DB_Q is used to get and set values.

	(void) pvParameters;
	const  TickType_t xDelay = 5000 / portTICK_PERIOD_MS;	  // Set xDelay to one sec.
	Serial.println("Global starting");
	Queue_struct messin,messout;
	GlobalDB myDB;

	messout.command = 11;
	messout.value = 0;
	messin.command = 0;
	messin.value = 0;


	for (;;) // A Task shall never return or exit.
	{
		if (xQueueReceive(Global_db_q,&messin,xDelay))
		{
			if (messin.command == 10)						// Some process ask for a value
			{
				messout.value = myDB.Get_var(messin.ID);
				messout.command = 11;
				messout.value = myDB.Get_var(messin.ID);
				messout.ID = messin.ID;

				xQueueSendToBack(messin.returnHandle,&messout,100);
			}
			if (messin.command == 11)						// Some process sent a value
			{
				myDB.Set_var(messin.ID,messin.value);
			}

		}
	}

}

void DCC_task(void *pvParameters)
{
	const  TickType_t xDelay = 500 / portTICK_PERIOD_MS;

	QueueHandle_t DCC_q = xQueueCreate(2 , sizeof(Queue_struct ) );

	int i = 1;

	uint16_t uInt;
	int16_t sInt;

	for (;;) // A Task shall never return or exit.
	{
		Global_db_set(1,i);
		Global_db_set(2,(i*2));
		Global_db_set(3,(1000+i));
		vTaskDelay(xDelay);
		i++;

/*
		Tps_filtered.addValue(Pot.getTPS());

		Global_db_set(1, Tps_filtered.getFilteredValue() );			// Ändra 1 till def för Tps

		Global_db_set(2, Pot.getLastADCValue() );					// Ändra 2 till def för Tps_adc

		Global_db_set(3, Out_current.readCurrent() );				// Ändra 3 till def för Out_current

		Global_db_set(4, Out_current.getLastADCValue() );				// Ändra 4 till Out_current_adc_index

		Global_db_set(5, Volt_in.readVolt()/100 );					// Ändra 5 till In_volt_index

// 		RPage.PWM = RPage.TPSPos;

		analogWrite(pwmPin ,Global_db_get(1, DCC_q) );						// Ändra 1 till Tps_index
*/
	}
}
