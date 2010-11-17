#pragma once
#include <assert.h>

#include "ILuaObject.h"
#include "NrpText.h"

namespace nrp
{

#define LUNA_ILUASCENENODE_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class),\
											LUNA_AUTONAME_FUNCTION(class, SetVisible ),\
											LUNA_AUTONAME_FUNCTION(class, GetVisible ),\
											LUNA_AUTONAME_FUNCTION(class, SetPosition ),\
											LUNA_AUTONAME_FUNCTION(class, SetScale ),\
											LUNA_AUTONAME_FUNCTION(class, SetRotation ),\
											LUNA_AUTONAME_FUNCTION(class, GetPosition ),\
											LUNA_AUTONAME_FUNCTION(class, Remove ),\
											LUNA_AUTONAME_FUNCTION(class, SetName ),\
											LUNA_AUTONAME_FUNCTION(class, GetName ),\
											LUNA_AUTONAME_FUNCTION(class, GetTypeName ),\
											LUNA_AUTONAME_FUNCTION(class, SetMaterialFlag ),\
											LUNA_AUTONAME_FUNCTION(class, SetTriangleSelector),\
											LUNA_AUTONAME_FUNCTION(class, SetMaterialTexture )

template< class T > class ILuaSceneNode : public ILuaObject< T >
{
public:
	ILuaSceneNode(lua_State *L, stringw luaName) : ILuaObject( L, luaName )
	{

	}

	virtual bool IsChildOfCurrentScene_() { return true; }

	int GetTypeName( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( ":GetTypeName not need parameter" ) );

		NrpText text;
		IF_OBJECT_NOT_NULL_THEN
		{
			switch( object_->getType() )
			{
			case scene::ESNT_TERRAIN:	text = L"Terrain";	break;
			case scene::ESNT_MESH:		text = L"Mesh";		break;		
			case scene::ESNT_SONAR:		text = L"Sonar";	break;
			case scene::ESNT_TARGET:	text = L"Target";	break;
			case scene::ESNT_CUBE:		text = L"Cube";		break;
			case scene::ESNT_REGION:	text = L"Region";	break;

			default: text = "Unknown";
			}
		}

		lua_pushstring( L, text );
		return 1;		
	}

	int SetMaterialTexture( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, _ErrStr(":SetMaterialTexture need 2 parameter") );

		int level = lua_tointeger( L, 2 );
		NrpText fileTexture = lua_tostring( L, 3 );						//принимает булевое значение в качестве луа-параметра

		IF_OBJECT_NOT_NULL_THEN	object_->setMaterialTexture( level, object_->getSceneManager()->getVideoDriver()->getTexture( fileTexture ) );									

		return 1;	
	}

	int SetMaterialFlag( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, _ErrStr(":SetMaterialFlag need 2 parameter") );

		video::E_MATERIAL_FLAG flagName = (video::E_MATERIAL_FLAG)lua_tointeger( L, 2 );						//принимает булевое значение в качестве луа-параметра
		bool flagValue = lua_toboolean( L, 3 ) > 0;

		IF_OBJECT_NOT_NULL_THEN	object_->setMaterialFlag( flagName, flagValue );									

		return 1;	
	}

	int SetTriangleSelector( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr(":SetTriangleSelector need ITriangleSelector parameter") );

		scene::ITriangleSelector* selector = (scene::ITriangleSelector*)lua_touserdata( L, 2 );

		IF_OBJECT_NOT_NULL_THEN
		{
			object_->setTriangleSelector( selector );
			selector->drop();
		}

		return 1;
	}

	int GetName( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr(":GetName not need parameter" ) );

		NrpText name = "";
	
		IF_OBJECT_NOT_NULL_THEN 
			if( IsChildOfCurrentScene_() )
				name = object_->getName();

		lua_pushstring( L, name );									

		return 1;		
	}

	int SetName( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr(":SetName need string parameter" ) );

		NrpText name = lua_tostring( L, 2 );						

		IF_OBJECT_NOT_NULL_THEN	object_->setName( name.ToStr() );									

		return 1;	
	}

	int Remove( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( ":Remove not need any parameter" ) );

		IF_OBJECT_NOT_NULL_THEN	object_->remove();

		return 1;
	}

	int GetVisible( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( ":GetVisible not need any parameter" ) );

		bool visible = false;
		IF_OBJECT_NOT_NULL_THEN visible = object_->isVisible();
		lua_pushboolean( L, visible );

		return 1;
	}

	int SetVisible( lua_State *L )							//изменение видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr(":setVisible need bool parameter" ) );

		bool visible = lua_toboolean( L, 2 ) > 0;						//принимает булевое значение в качестве луа-параметра

		IF_OBJECT_NOT_NULL_THEN	object_->setVisible( visible );							

		return 1;
	}

	int GetPosition( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr(":GetPosition not need any parameter" ) );

		core::vector3df pos;
		IF_OBJECT_NOT_NULL_THEN pos = object_->getPosition();
		lua_pushnumber( L, pos.X );
		lua_pushnumber( L, pos.Y );
		lua_pushnumber( L, pos.Z );

		return 3;
	}

	int SetRotation( lua_State *L )
	{
		core::vector3df rot;
		int argc = lua_gettop(L);
		luaL_argcheck( L, argc == 4, 4, _ErrStr(":setRotation need three parameter" ) );

		rot.X = (float)lua_tonumber( L, 2 );
		rot.Y = (float)lua_tonumber( L, 3 );
		rot.Z = (float)lua_tonumber( L, 4 );
		IF_OBJECT_NOT_NULL_THEN object_->setRotation( rot );

		return 1;
	}

	int SetScale( lua_State *L )
	{
		core::vector3df scale;
		int argc = lua_gettop(L);
		luaL_argcheck( L, argc == 4, 4, _ErrStr(":setScale need three parameter" ));

		scale.X = (float)lua_tonumber( L, 2 );
		scale.Y = (float)lua_tonumber( L, 3 );
		scale.Z = (float)lua_tonumber( L, 4 );
		IF_OBJECT_NOT_NULL_THEN object_->setScale( scale );

		return 1;
	}

	int SetPosition( lua_State *L )
	{
		core::vector3df pos;
		int argc = lua_gettop(L);
		luaL_argcheck( L, argc == 4, 4, _ErrStr(":setPosition need three parameter" ) );

		pos.X = (float)lua_tonumber( L, 2 );
		pos.Y = (float)lua_tonumber( L, 3 );
		pos.Z = (float)lua_tonumber( L, 4 );
		IF_OBJECT_NOT_NULL_THEN object_->setPosition( pos );

		return 1;
	}
};
}//namespace nrp