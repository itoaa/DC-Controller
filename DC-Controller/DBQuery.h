/*
 * DBQuery.h
 *
 *  Created on: 3 sep. 2017
 *      Author: Ola Andersson
 *		Version	1.0
 *			Used to communicate with GlobalDB task
 */

#ifndef DBQUERY_H_
#define DBQUERY_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

struct Queue_struct
{
	byte command;					// Type of command sent to queue
	int ID;							// ID of source and dest unit sent/recived the question
	int	value;						// Value of question / responce
	QueueHandle_t returnHandle;		// Potential return queue-handler.
};
extern QueueHandle_t GlobalQ;

class DBQuery {
public:
	DBQuery(QueueHandle_t Tx_q, QueueHandle_t Rx_q);
	virtual ~DBQuery();
	void 	Set_global(int _ID, int _Value);
	int 		Get_global(int _ID);
private:
	QueueHandle_t	Send_queue;
	QueueHandle_t	Recive_queue;

};

#endif /* DBQUERY_H_ */
