/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:07
	file base:	LuaFunctionality
	file ext:	h
	author:		Dalerank
	
	purpose:	����� ������ � ����������� ������� Lua, ���������� 
				� �������� ���-������, ���������� ���-������
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
	���������� ���������������� ������� 
	*/
	virtual void AddLuaFunction( int actionType, int funcRef );

	/*
	�������� ���������������� ������� �� ������
	*/
	virtual void RemoveLuaFunction( int actionType, int m );

	virtual void DoLuaFunctionsByType( int funcType, void* sender, void* param = NULL );
};

#endif
