#ifndef _INCLUDE_LUAQUESTENGINE_
#define _INCLUDE_LUAQUESTENGINE_

#include "ILuaProject.h"
#include "NrpQuestEngine.h"

namespace nrp
{

class CLuaQuestEngine: public ILuaProject<CNrpQuestEngine>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaQuestEngine)
	static const char* ClassName();

	int GetActiveQuest( lua_State* L );
	int GetNumAQuests( lua_State* L );
	int GetQuest( lua_State* L );
	int StartQuest( lua_State* L );
	int ObsoleteQuest( lua_State* L );
	int AddResourceDirectory( lua_State* L );
    int AddActiveQuest( lua_State* L );
    int SetResult( lua_State* L );
    int GetResult( lua_State* L );

	//int AddResourceDirectory( lua_State* L );

	CLuaQuestEngine(lua_State *L, bool ex);

	~CLuaQuestEngine(void);
};

}

#endif