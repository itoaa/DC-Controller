/*
 * GlobalDB.cpp
 *
 *  Created on: 3 sep. 2017
 *      Author: Ola Andersson
 *      Version 1.0
 */

#include "GlobalDB10.hpp"


GlobalDB::GlobalDB()
{
	Global_vars.Charger_state = 0;
}
GlobalDB::~GlobalDB()
{
}


void GlobalDB::Set_var(int _ID, int _Value)
{
	int * Temp_pointer;
	Temp_pointer = (int*)&Global_vars + (_ID*2);
	*Temp_pointer = _Value;
}

int GlobalDB::Get_var(int _ID)
{
	int * Temp_pointer;
	Temp_pointer = (int*)&Global_vars + (_ID * 2);
	return(*Temp_pointer);

}



