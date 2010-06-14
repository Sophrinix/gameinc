#pragma once

#include "ILuaBaseProject.h"

namespace nrp
{

const std::string CLASS_LUAGAME( "CLuaGame" );
class CNrpGame;

class CLuaGame : public ILuaBaseProject<nrp::CNrpGame>
{
public:
	static Luna<CLuaGame>::RegType methods[];				//методы обертки

	CLuaGame(lua_State *L);		
	int HaveBox( lua_State* L );
	int GetName( lua_State* L );
	int IsMyBoxAddon( lua_State* L );
	int RemoveBoxAddon( lua_State* L );
	int AddBoxAddon( lua_State* L );
	int GetBoxAddonsNumber( lua_State* L );
	int GetBoxAddon( lua_State* L );
	int CreateBox( lua_State* L );
	int RemoveBox( lua_State* L );
	int GetBoxLevel( lua_State* L );
	int IsSaling( lua_State* L );
	int SetViewImage( lua_State* L );
	int GetViewImage( lua_State* L );
	int GetBoxImage( lua_State* L );
	int GetBoxImageNumber( lua_State* L );
	int GetScreenshotNumber( lua_State* L );
	int GetScreenshot( lua_State* L );
															//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return CLASS_LUAGAME.c_str(); }
	template< class R > R GetImageLisParam_( lua_State* L, 
		                                     std::string funcName, 
											 std::string name, R defValue );
	int GetImagePath_( lua_State* L, 
					   std::string funcName, 
					   std::string nameParam );
};

}//namespace nrp