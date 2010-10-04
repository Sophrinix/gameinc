/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:07
	file base:	LuaFunctionality
	file ext:	h
	author:		Dalerank
	
	purpose:	����� ������ � ����������� ������� Lua, ���������� 
				� �������� ���-������, ���������� ���-������
*********************************************************************/
#pragma once
#include <string>
#include <vector>
#include "nrpScript.h"

class ILuaFunctionality
{
protected:
	typedef struct SFunctionLink
	{ 
		int actionType;
		std::string funcName;

		SFunctionLink( int aType, const std::string& fName )
		{
			actionType = aType;
			funcName = fName;
		}
	} *PFunctionLink;

	typedef std::vector< PFunctionLink > FunctionArray;
	FunctionArray luaFunctions_;

public:
	ILuaFunctionality(void) {};
	virtual	~ILuaFunctionality(void) 
	{
		FunctionArray::iterator& pIter = luaFunctions_.begin();

		if( pIter != luaFunctions_.end() )
		{
			delete (*pIter);
			pIter++;
		}
		
		luaFunctions_.clear();
	};


	FunctionArray::iterator FindLuaFunction( const int actionType, const std::string& funcName )
	{
		FunctionArray::iterator pIter = luaFunctions_.begin();

		for(; pIter != luaFunctions_.end(); pIter++ )				//���������� �� ������ ������������ �������
		{
			if( (*pIter)->actionType == actionType &&		//����� �� ���� ������� � ���������� ����� � ������	
				(*pIter)->funcName == funcName )
				return pIter;								//�� ��������� ���������� �������
		}

		return luaFunctions_.end();
	}
	/*
	���������� ���������������� ������� 
	*/
	virtual void AddLuaFunction( const int actionType, const std::string& funcName ) 
	{
		assert( actionType != 0 );
		if( FindLuaFunction(actionType, funcName) != luaFunctions_.end() )
			return;

		luaFunctions_.push_back( new SFunctionLink( actionType, funcName ) );
	}

	/*
	�������� ���������������� ������� �� ������
	*/
	virtual void RemoveLuaFunction( const int actionType, const std::string& funcName ) 
	{
		FunctionArray::iterator& pIter = FindLuaFunction( actionType, funcName );

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
			for( size_t cnt=0; cnt < luaFunctions_.size(); cnt++ )
			{
				PFunctionLink pLink = luaFunctions_[ cnt ];
				if( pLink->actionType == funcType )
				{
					nrp::CNrpScript::Instance().SetSender( (void*)param );
					const char* tmp = pLink->funcName.c_str();

					if( *tmp == '.' && *(tmp+1) == '/')
						nrp::CNrpScript::Instance().DoString( tmp+2 );
					else
						nrp::CNrpScript::Instance().CallFunction( tmp, (void*)param );

				}
			}

		}
		catch(...)
		{

		}
	}

};
