#pragma once

#include "ILuaBaseProject.h"

namespace nrp
{
CLASS_NAME CLASS_LUAGAME( "CLuaGame" );

class CNrpGame;

class CLuaGame : public ILuaBaseProject<nrp::CNrpGame>
{
public:
	static Luna<CLuaGame>::RegType methods[];				//методы обертки

	CLuaGame(lua_State *L);		
	int Create( lua_State* L );
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
	int GetLastMonthSales( lua_State* L );
	int GetAllTimeSales( lua_State* L );
	int GetPrice( lua_State* L );
	int SetPrice( lua_State* L );
	int GetCurrentMonthSales( lua_State* L );
	int GetCompany( lua_State* L );

	static const char* ClassName() { return CLASS_LUAGAME.c_str(); }
private:
	template< class R > R GetImageLisParam_( lua_State* L, 
		                                     std::string funcName, 
											 std::string name, R defValue );
	int GetImagePath_( lua_State* L, 
					   std::string funcName, 
					   std::string nameParam );
};

}//namespace nrp