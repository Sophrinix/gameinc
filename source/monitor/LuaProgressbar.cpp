#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProgressBar.h"
#include "NrpProgressBar.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaProgressBar >::RegType CLuaProgressBar::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaProgressBar ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetPosition ),
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetFillImage ),
	{0,0}
};

CLuaProgressBar::CLuaProgressBar(lua_State *L)	: ILuaGuiElement(L, "CLuaProgressBar")							//�����������
{}

int CLuaProgressBar::SetPosition( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetPosition need int parameter");

	int position = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setPosition( position );

	return 1;
}

int CLuaProgressBar::SetImage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetImage need string parameter");

	const char* textureName = lua_tostring( L, 2 );
	assert( textureName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* ptrTexture = CNrpEngine::Instance().GetVideoDriver()->getTexture( textureName );
		object_->setImage( ptrTexture );
	}

	return 1;	
}

int CLuaProgressBar::SetFillImage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetFillImage need string parameter");

	const char* textureName = lua_tostring( L, 2 );
	assert( textureName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* ptrTexture = CNrpEngine::Instance().GetVideoDriver()->getTexture( textureName );
		object_->setFillImage( ptrTexture );
	}

	return 1;	
}
}//namespace nrp