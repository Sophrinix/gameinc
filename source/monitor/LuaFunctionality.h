/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:07
	file base:	LuaFunctionality
	file ext:	h
	author:		Dalerank
	
	purpose:	Класс работы с виртуальной машиной Lua, размещение 
				и удаление луа-линков, выполнение луа-линков
*********************************************************************/
#pragma once
#include <irrlicht.h>
#include "nrpScript.h"
#include "nrpText.h"

using irr::core::list;

class ILuaFunctionality
{
protected:
	typedef struct SFunctionLink
	{ 
		int actionType;
		nrp::NrpText funcName;

		SFunctionLink( int aType, const nrp::NrpText& fName )
		{
			actionType = aType;
			funcName = fName;
		}
	} *PFunctionLink;

	typedef list< PFunctionLink > FunctionArray;
	FunctionArray luaFunctions_;

public:
	ILuaFunctionality(void) {};
	virtual	~ILuaFunctionality(void) 
	{
		FunctionArray::Iterator pIter = luaFunctions_.begin();

		if( pIter != luaFunctions_.end() )
		{
			delete (*pIter);
			pIter++;
		}
		
		luaFunctions_.clear();
	};


	FunctionArray::Iterator FindLuaFunction( const int actionType, const nrp::NrpText& funcName )
	{
		FunctionArray::Iterator pIter = luaFunctions_.begin();

		for(; pIter != luaFunctions_.end(); pIter++ )				//пробежимся по списку подключенных функций
		{
			if( (*pIter)->actionType == actionType &&		//чтобы не было функций с одинаковым типом и именем	
				(*pIter)->funcName == funcName )
				return pIter;								//не добавляем одинаколые функции
		}

		return luaFunctions_.end();
	}
	/*
	Добавление пользовательской функции 
	*/
	virtual void AddLuaFunction( const int actionType, const nrp::NrpText& funcName ) 
	{
		assert( actionType != 0 );
		if( FindLuaFunction(actionType, funcName) != luaFunctions_.end() )
			return;

		luaFunctions_.push_back( new SFunctionLink( actionType, funcName ) );
	}

	/*
	Удаление пользовательской функции из списка
	*/
	virtual void RemoveLuaFunction( const int actionType, const nrp::NrpText& funcName ) 
	{
		FunctionArray::Iterator pIter = FindLuaFunction( actionType, funcName );

		if( pIter != luaFunctions_.end() )
		{
			delete (*pIter);
			luaFunctions_.erase( pIter );
		}
	}

	template< class T > void DoLuaFunctionsByType( int funcType, T* param = NULL )
	{
		try
		{
			for( FunctionArray::Iterator pIter=luaFunctions_.begin(); 
				 pIter != luaFunctions_.end(); pIter++ )
			{
				if( (*pIter)->actionType == funcType )
				{
					nrp::CNrpScript::Instance().SetSender( (void*)param );
					const wchar_t* tmp = (*pIter)->funcName.ToWide();

					if( *tmp == L'.' && *(tmp+1) == L'/')
						nrp::CNrpScript::Instance().DoString( tmp+2 );
					else
						nrp::CNrpScript::Instance().CallFunction( tmp, (void*)param );

				}
			}
		}
		catch(...)
		{}
	}
};
