#include "StdAfx.h"
#include "LuaEvent.h"
#include "NrpText.h"
#include <IEventReceiver.h>

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAEVENT( "CLuaEvent" );

BEGIN_LUNA_METHODS( CLuaEvent )
	LUNA_ILUAOBJECT_HEADER( CLuaEvent )
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetUserData1 )
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetLogText )
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetTime )
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetChar )
	LUNA_AUTONAME_FUNCTION( CLuaEvent, IsKeyDown )
	LUNA_AUTONAME_FUNCTION( CLuaEvent, GetGuiCaller )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaEvent)
END_LUNA_PROPERTIES

CLuaEvent::CLuaEvent(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUAEVENT, ex) {}

int CLuaEvent::GetUserData1( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetUserData1 not need any parameter");

	int uData = -9999;

	IF_OBJECT_NOT_NULL_THEN uData = _object->UserEvent.UserData1;
	lua_pushinteger( L, uData );

	return 1;
}

int CLuaEvent::GetLogText( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetLogText not need any parameter");

	NrpText text;
	IF_OBJECT_NOT_NULL_THEN text = ((Log*)_object->UserEvent.UserData2)->Text.str().c_str();
	lua_pushstring( L, text );

	return 1;
}

int CLuaEvent::GetTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetTime not need any parameter");

	char text[MAX_PATH];
	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME time = ((Log*)_object->UserEvent.UserData2)->createTime;
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
	IF_OBJECT_NOT_NULL_THEN	WideCharToMultiByte( CP_ACP, 0, &(_object->KeyInput.Char), 1, text, 1, "", false );
 
	lua_pushstring( L, text );

	return 1;
}

int CLuaEvent::IsKeyDown( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetChar not need any parameter");

	int isDown = 0;
	IF_OBJECT_NOT_NULL_THEN	isDown = _object->KeyInput.PressedDown ? 1 : 0;
	lua_pushboolean( L, isDown );

	return 1;	
}

int CLuaEvent::GetGuiCaller( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaEvent:GetGuiCaller not need any parameter");

	gui::IGUIElement* caller = NULL;
	IF_OBJECT_NOT_NULL_THEN	caller = _object->GUIEvent.Caller;

	lua_pushlightuserdata( L, caller );

	return 1;
}

const char* CLuaEvent::ClassName()
{
	return ( CLASS_LUAEVENT );
}
}//namespace nrp