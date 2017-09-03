/*
 * DBQuery.cpp
 *
 *  Created on: 3 sep. 2017
 *      Author: olaandersson
 */

#include "DBQuery.h"

DBQuery::DBQuery(QueueHandle_t Tx_q, QueueHandle_t Rx_q) {
	Send_queue = Tx_q;
	Recive_queue = Rx_q;

}

DBQuery::~DBQuery() {
	// TODO Auto-generated destructor stub
}


void DBQuery::Set_global(int _ID, int _Value)
{
	Queue_struct Mess_out;

	Mess_out.command = 11;
	Mess_out.ID = _ID;
	Mess_out.value = _Value;
	Mess_out.returnHandle = NULL;
    xQueueSendToBack(Send_queue,&Mess_out,100);
}

int DBQuery::Get_global(int _ID)
{
	Queue_struct Mess_in,Mess_out;

	Mess_out.command = 10;
	Mess_out.ID = _ID;
	Mess_out.returnHandle = Send_queue;
    xQueueSendToBack(Send_queue,&Mess_out,100);
	xQueueReceive(Recive_queue,&Mess_in,100);
	 return(Mess_in.value);

}

