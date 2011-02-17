#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpGameProject;
	class CNrpTechnology;
}

namespace nrp
{
class CLuaGameProject : public ILuaProject<nrp::CNrpGameProject>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaGameProject)
	static const char* ClassName();

	CLuaGameProject(lua_State *L, bool);		
	int SetGameEngine( lua_State* L); 
	int GetGameEngine( lua_State* L ); 

	int GetNumber( lua_State* L );

	int IsMyTech( lua_State* L );
	int GetTech( lua_State* L );
	int AddTech( lua_State* L );
	int RemoveTech( lua_State* L );

	int GetCodeVolume( lua_State* L );
	int GetCodeQuality( lua_State* L );
	int GetScenario( lua_State* L );
	int SetScenario( lua_State* L );
	int GetLicense( lua_State* L );

	int GetPlatform( lua_State* L ); 
	int IsMyPlatform( lua_State* L );
	int AddPlatform( lua_State* L );
	int RemovePlatform( lua_State* L );

	int IsProjectReady( lua_State* L );
	int HaveLicense( lua_State* L );
	int HaveScenario( lua_State* L );
	int SetScriptEngine( lua_State* L ); int GetScriptEngine( lua_State* L );
	int GetMiniGameEngine( lua_State* L ); int SetMiniGameEngine( lua_State* L );

	int GetPhysicEngine( lua_State* L ); int SetPhysicEngine( lua_State* L);
	
	int GetVideoQuality( lua_State* L ); int SetVideoQuality( lua_State* L );

	int GetSoundQuality( lua_State* L ); int SetSoundQuality( lua_State* L );

	int GetEngineExtend( lua_State* L );
	int GetCpuUse( lua_State* L );
	int GetMemoryUse( lua_State* L );

	int Create( lua_State* L );
	int Remove( lua_State* L );
															
private:
	template< class T >
	int _SetNamedTech( lua_State* L, const NrpText& funcName, const NrpText& paramName );
	
	template< class Param, class T > int _XAny( lua_State*,
											    const NrpText&, 
											    void (T::*Method)( const Param* ) );

	template< class B, class A, class T > int _GetAny( lua_State* L,
											   const NrpText& funcName, 
											   A* (T::*Method)( PROJECT_TYPE, int ) );

	template< class T, class B >
	int _XInitialize( lua_State* L, const NrpText& funcName, OPTION_NAME& paramName );
};

}//namespace nrp