#ifndef _INCLUDE_LUASOUNDENGINE_
#define _INCLUDE_LUASOUNDENGINE_

#include "NrpSoundEngine.h"
#include "ILuaProject.h"

namespace nrp
{
	
class CLuaSoundEngine : public ILuaProject<CNrpSoundEngine>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaSoundEngine)
	static const char* ClassName();

	int PlayBackground( lua_State* L );
	int Play( lua_State* L );

	int AddResourceDirectory( lua_State* L );

	CLuaSoundEngine(lua_State *L, bool ex);

	~CLuaSoundEngine(void);
};

}//end namespace nrp

#endif 