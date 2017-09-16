/*
 * GlobalDB11.cpp
 *
 *  Created on: 3 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 *      Version 1.1
 *      Se h-file for info
 */

#include "GlobalDB11.h"


GlobalDB::GlobalDB()
{
//	Global_vars.Charger_state = 0;
}
GlobalDB::~GlobalDB()
{
}


void GlobalDB::Set_var(int ID, int Value)
{
	int * Temp_pointer;
	Temp_pointer = (int*)&Global_vars + (ID*2);
	*Temp_pointer = Value;
}

int GlobalDB::Get_var(int ID)
{
	int * Temp_pointer;
	Temp_pointer = (int*)&Global_vars + (ID * 2);
	return(*Temp_pointer);

}

void GlobalDB::Set_index(int Index, char Value)
{
	char * Temp_pointer;
	Temp_pointer = (char*)&Global_vars + (Index);
	*Temp_pointer = Value;
}

char GlobalDB::Get_index(int Index)
{
	char * Temp_pointer;
	Temp_pointer = (char*)&Global_vars + (Index);
	return(*Temp_pointer);

}


