/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:07
	file base:	LuaFunctionality
	file ext:	h
	author:		Dalerank
	
	purpose:	Класс работы с виртуальной машиной Lua, размещение 
				и удаление луа-линков, выполнение луа-линков
*********************************************************************/
#ifndef _ILuaFunctionality_H_
#define _ILuaFunctionality_H_

#include "nrpText.h"
#include <map>

class ILuaFunctionality
{
protected:
	typedef std::map< int, int > FunctionMap;
	FunctionMap luaFunctions_;

public:
	ILuaFunctionality(void) {};
	virtual	~ILuaFunctionality(void);

	/*
	Добавление пользовательской функции 
	*/
	virtual void AddLuaFunction( int actionType, int funcRef );

	/*
	Удаление пользовательской функции из списка
	*/
	virtual void RemoveLuaFunction( int actionType, int m );

	virtual void DoLuaFunctionsByType( int funcType, void* sender, void* param = NULL );
};

#endif
