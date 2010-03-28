#include "StdAfx.h"
#include <string>
#include "LuaEvent.h"
#include "StrConversation.h"
#include <IEventReceiver.h>

using namespace irr;

namespace nrp
{

Luna< CLuaEvent >::RegType CLuaEvent::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaEvent ),
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetUserData1 ),
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetLogText ),
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetTime ),
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetChar ),
	LUNA_AUTONAME_FUNCTION( CLuaEvent, IsKeyDown ),
	{0,0}
};

CLuaEvent::CLuaEvent(lua_State *L) : ILuaObject(L, "CLuaEvent") {}

int CLuaEvent::GetUserData1( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetUserData1 not need any parameter");

	int uData = -9999;

	IF_OBJECT_NOT_NULL_THEN uData = object_->UserEvent.UserData1;
	lua_pushinteger( L, uData );

	return 1;
}

int CLuaEvent::GetLogText( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetLogText not need any parameter");

	std::string text;
	IF_OBJECT_NOT_NULL_THEN text = ((Log*)object_->UserEvent.UserData2)->Text.str();
	lua_pushstring( L, text.c_str() );

	return 1;
}

int CLuaEvent::GetTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetTime not need any parameter");

	char text[MAX_PATH];
	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME time = ((Log*)object_->UserEvent.UserData2)->createTime;
		snprintf( text, MAX_PATH - 1, "%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond );	
	}
	lua_pushstring( L, text );

	return 1;
}

int CLuaEvent::GetChar( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetChar not need any parameter");

	char text[ 10 ] = { 0 };
	IF_OBJECT_NOT_NULL_THEN	WideCharToMultiByte( CP_ACP, 0, &(object_->KeyInput.Char), 1, text, 1, "", false );
 
	lua_pushstring( L, text );

	return 1;
}

int CLuaEvent::IsKeyDown( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetChar not need any parameter");

	int isDown = 0;
	IF_OBJECT_NOT_NULL_THEN	isDown = object_->KeyInput.PressedDown ? 1 : 0;
	lua_pushboolean( L, isDown );

	return 1;	
}
}//namespace nrp