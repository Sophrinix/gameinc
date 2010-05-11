#pragma once

#include "ILuaObject.h"

namespace nrp
{

class CNrpGameBox;

class CLuaGameBox : public ILuaObject<nrp::CNrpGameBox>
{
public:
	static Luna<CLuaGameBox>::RegType methods[];				//������ �������

	CLuaGameBox(lua_State *L);		
																//������� �� ��� ������
	static const char* StaticGetLuaName() { return "CLuaGameBox"; }
};

}//namespace nrp