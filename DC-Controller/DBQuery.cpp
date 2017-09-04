/*
 * DBQuery.cpp
 *
 *  Created on: 3 sep. 2017
 *      Author: olaandersson
 */

#include "DBQuery.h"

void Global_db_eprom_set(int _ID, int _Value) {}
int Global_db_eprom_get(int _ID, QueueHandle_t Recive_queue) {return(0);}


void Global_db_set(int _ID, int _Value)
{
	Queue_struct Mess_out;

	Mess_out.command = 11;
	Mess_out.ID = _ID;
	Mess_out.value = _Value;
	Mess_out.returnHandle = NULL;
    xQueueSendToBack(Global_db_q,&Mess_out,100);
}

int Global_db_get(int _ID, QueueHandle_t Recive_queue)
{
	Queue_struct Mess_in,Mess_out;

	Mess_out.command = 10;
	Mess_out.ID = _ID;
	Mess_out.returnHandle = Recive_queue;
    xQueueSendToBack(Global_db_q,&Mess_out,100);
	xQueueReceive(Recive_queue,&Mess_in,100);
	return(Mess_in.value);

}

