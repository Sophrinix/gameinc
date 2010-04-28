#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class INrpProject;
}

namespace nrp
{

class CLuaProject : public ILuaProject<nrp::INrpProject>
{
public:
	static Luna<CLuaProject>::RegType methods[];				//������ �������

	CLuaProject(lua_State *L);	
	int SetName( lua_State* vm );
																//������� �� ��� ������
	static const char* StaticGetLuaName() { return "CLuaProject"; }
};

}//namespace nrp