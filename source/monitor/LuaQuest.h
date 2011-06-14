#ifndef _INCLUDE_LUAQUEST_
#define _INCLUDE_LUAQUEST_

#include "ILuaProject.h"
#include "NrpQuest.h"

namespace nrp
{

class CLuaQuest : public ILuaProject<CNrpQuest>
{
public:
		//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaQuest)
	static const char* ClassName();

	int GetX( lua_State* L );
	int SetX( lua_State* L );
	int GetY( lua_State* L );
	int SetY( lua_State* L );
	int SetFuncStart( lua_State* L );
    int Obsolete( lua_State* L );
    int SetResult( lua_State* L );

	CLuaQuest(lua_State *L, bool ex);
	~CLuaQuest(void);
};

}

#endif

