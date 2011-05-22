#pragma once

#include "ILuaBaseProject.h"

namespace nrp
{
class CNrpGame;

class CLuaGame : public ILuaBaseProject<nrp::CNrpGame>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaGame)				
	static const char* ClassName();

	CLuaGame(lua_State *L, bool );		
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
	int GetAllTimeProfit( lua_State* L );
	int GetPrice( lua_State* L );
	int SetPrice( lua_State* L );
	int GetLastMonthProfit( lua_State* L );
	int GetCompany( lua_State* L );
	int GetDescriptionLink( lua_State* L );
	int GetRandomRecense( lua_State* L );

	int GetRating( lua_State* L );
	int GetGraphikRating( lua_State* L );
	int GetSoundRating( lua_State* L );
	int GetGameplayRating( lua_State* L );
	int GetBugsRating( lua_State* L );

private:
	template< class R > R GetImageLisParam_( lua_State* L, 
		                                     const NrpText& funcName, 
											 OPTION_NAME& name, R defValue );
	int GetImagePath_( lua_State* L, const NrpText& funcName, OPTION_NAME& nameParam );
};

}//namespace nrp