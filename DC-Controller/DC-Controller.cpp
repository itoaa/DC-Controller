#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "GlobalDB10.hpp"
#include "Led10.h"
#include "DBQuery.h"

/* ------------------------------------------------------------------
 * CD-Controller ver 2 utvecklas i Eclipse och byter kernel från
 * OS48 till FreeRTOS V9
 * Version 2.0 kommer att ta ett nytt grepp om globala variabler och
 * ersätta dessa med en databas-process som övriga processer kan kommunisera
 * med för att få tillgång till data i systemet.
 */

// Define for witch hardware to compile
 #define SDC-30a
// #define evFartreg-proto

#include "SDC-30a-HW.h"

QueueHandle_t Global_DB_Q;
QueueHandle_t Serial_Q;

void Serial_task(void *pvParameters);
void Global_db_task(void *pvParameters);
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
void DCC_task(void *pvParameters)
{
	for (;;) // A Task shall never return or exit.
	{
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
	Global_DB_Q = xQueueCreate(4,sizeof(Queue_struct));
	Serial_Q = xQueueCreate(2 , sizeof(Queue_struct ) );

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
	const  TickType_t xDelay = 5000 / portTICK_PERIOD_MS;	  // Set xDelay to one sec.
	Queue_struct messin;
	Queue_struct messout;

	messout.command = 11;
	messout.value = 0;
	messin.command = 0;
	messin.value = 0;
	DBQuery Gdb(Global_DB_Q,Serial_Q);
	for (;;) // A Task shall never return or exit.
	{
		Serial.print("Serial: Variable 2 = ");
		Serial.println(Gdb.Get_global(2));

		Serial.println("\nSerial: setting Variable 2 to 2274\n");
		Gdb.Set_global(1,1274);
		Gdb.Set_global(2,2274);
		Gdb.Set_global(3,3274);
		Serial.print("Serial: Now get variable 2 returns: ");
		Serial.println(Gdb.Get_global(2));
		Serial.println(Gdb.Get_global(1));
		Serial.println(Gdb.Get_global(2));
		Serial.println(Gdb.Get_global(3));

		vTaskDelay(xDelay);

	}
}


// Task to manage GlobalDatabase (GlobalDB). Uses class DB to store data.
// Class DBQueue is used for querying GlobalDB.
// Queue Global_DB_Q is used to get and set values.
void Global_db_task(void *pvParameters)
{
	(void) pvParameters;
	const  TickType_t xDelay = 5000 / portTICK_PERIOD_MS;	  // Set xDelay to one sec.
	Serial.print("Global is starting");

	Queue_struct messin,messout;
	GlobalDB myDB;

	messout.command = 11;
	messout.value = 0;
	messin.command = 0;
	messin.value = 0;


	for (;;) // A Task shall never return or exit.
	{
		if (xQueueReceive(Global_DB_Q,&messin,xDelay))
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
