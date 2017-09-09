/*
 * TSSerial10.cpp
 *
 *  Created on: 4 sep. 2017
 *      Author: Ola Andersson
 */

#include "TSSerial10.h"

TSSerial::TSSerial(word BaudRate) {
	Serial_q = xQueueCreate(2 , sizeof(Queue_struct ) );
}

TSSerial::~TSSerial() {
	// TODO Auto-generated destructor stub
}

void TSSerial::Send_eprom_var()
/*{
  // Print all eprom variables in TunerStudio styel to serialport.
  // Need to match MegaTune-ini-file.
}*/
{
  int i;
  char *temppointer;
  for (i=0; i < PG1S; i++)     // i = 0 till page1size
  {
      Serial.write(Global_db_eprom_get(i,Serial_q));
  }
}

void TSSerial::Get_eprom_var()
{
  int i,j;
  char pos;
  char *temppointer;
  for (i=0; i < 6000; i++)				// Wait for next byte, this byte is the offset in eeprom (timeout = ?? 64000 tic ??) needs fix.
  if (Serial.available())				// See if there is something available on serial port
  {
	  pos = Serial.read();
	  for (j=0; j < 6000; j++)			// Wait for next byte, this byte is the value for the just received offset (timeout = ?? 64000 tic ??)
	  if (Serial.available())			// See if there is something available on serial port
	  {
		  Global_db_eprom_set(pos,Serial.read());          // Put the value in the position that temppointer points to.
		  break;						// Mark that we have got the byte.
		  j = 6001;						// Break inner loop if byte is resived before timeout (use break??)
	  }
	  break;
	  i = 6001;							// Break loop if byte is received before timeout (Use break ??)
  }

}

void TSSerial::Send_rev(char str[20])
{
	for ( int i = 0; str[i] != '\0' ; ++i )
	{
	Serial.write(str[i]);
	}}
void TSSerial::Send_sec(int Sec)
{
	Serial.print(Sec);
}

void TSSerial::Send_rpage()
{
	// print all variables in TunerStudio style to serial port.
	// Need to mach MegaTune-ini-file.
	// Serial.print("Sending RTD");
	// Serial.write((unsigned char *) _RPage, RTPS);     // Skicka v‰rdet pÂ den positionen

  int i;
  for (i=0; i < RTPS; i++)     // i = 0 till page1size
  {
      Serial.write(Global_db_get(i,Serial_q));
  }

}

