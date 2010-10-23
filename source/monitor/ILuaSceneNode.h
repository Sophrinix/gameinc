#pragma once
#include <assert.h>

#include "ILuaObject.h"
#include "StrConversation.h"

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
	ILuaSceneNode(lua_State *L, std::string luaName) : ILuaObject( L, luaName )
	{

	}

	virtual bool IsChildOfCurrentScene_() { return true; }

	int GetTypeName( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ObjectTypeName() + ":GetTypeName not need parameter" ).c_str() );

		std::string text;
		IF_OBJECT_NOT_NULL_THEN
		{
			switch( object_->getType() )
			{
			case scene::ESNT_TERRAIN:	text = "Terrain";	break;
			case scene::ESNT_MESH:		text = "Mesh";		break;		
			case scene::ESNT_SONAR:		text = "Sonar";		break;
			case scene::ESNT_TARGET:	text = "Target";	break;
			case scene::ESNT_CUBE:		text = "Cube";		break;
			case scene::ESNT_REGION:	text = "Region";	break;

			default: text = "Unknown";
			}
		}

		lua_pushstring( L, text.c_str() );
		return 1;		
	}

	int SetMaterialTexture( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, ("Function "+ ObjectTypeName() + ":SetMaterialTexture need 2 parameter").c_str() );

		int level = lua_tointeger( L, 2 );
		const char* fileTexture = lua_tostring( L, 3 );						//принимает булевое значение в качестве луа-параметра
		assert( fileTexture != NULL );

		IF_OBJECT_NOT_NULL_THEN	object_->setMaterialTexture( level, object_->getSceneManager()->getVideoDriver()->getTexture( fileTexture ) );									

		return 1;	
	}

	int SetMaterialFlag( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, ("Function " + ObjectTypeName() + ":SetMaterialFlag need 2 parameter").c_str() );

		video::E_MATERIAL_FLAG flagName = (video::E_MATERIAL_FLAG)lua_tointeger( L, 2 );						//принимает булевое значение в качестве луа-параметра
		bool flagValue = lua_toboolean( L, 3 ) > 0;

		IF_OBJECT_NOT_NULL_THEN	object_->setMaterialFlag( flagName, flagValue );									

		return 1;	
	}

	int SetTriangleSelector( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ("Function " + ObjectTypeName() + ":SetTriangleSelector need ITriangleSelector parameter").c_str() );

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
		luaL_argcheck(L, argc == 1, 1, ("Function " + ObjectTypeName() + ":GetName not need parameter" ).c_str() );

		std::string name = "";
	
		IF_OBJECT_NOT_NULL_THEN 
			if( IsChildOfCurrentScene_() )
				name = object_->getName();
		lua_pushstring( L, name.c_str() );									

		return 1;		
	}

	int SetName( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ( "Function " + ObjectTypeName() + ":SetName need string parameter" ).c_str() );

		const char* name = lua_tostring( L, 2 );						
		assert( name != NULL );

		IF_OBJECT_NOT_NULL_THEN	object_->setName( name );									

		return 1;	
	}

	int Remove( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ObjectTypeName() + ":Remove not need any parameter" ).c_str() );

		IF_OBJECT_NOT_NULL_THEN	object_->remove();

		return 1;
	}

	int GetVisible( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ObjectTypeName() + ":GetVisible not need any parameter" ).c_str() );

		bool visible = false;
		IF_OBJECT_NOT_NULL_THEN visible = object_->isVisible();
		lua_pushboolean( L, visible );

		return 1;
	}

	int SetVisible( lua_State *L )							//изменение видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ( "Function " + ObjectTypeName() + ":setVisible need bool parameter" ).c_str() );

		bool visible = lua_toboolean( L, 2 ) > 0;						//принимает булевое значение в качестве луа-параметра

		IF_OBJECT_NOT_NULL_THEN	object_->setVisible( visible );							

		return 1;
	}

	int GetPosition( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ObjectTypeName() + ":GetPosition not need any parameter" ).c_str() );

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
		luaL_argcheck( L, argc == 4, 4, ( "Function " + ObjectTypeName() + ":setRotation need three parameter" ).c_str() );

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
		luaL_argcheck( L, argc == 4, 4, ( "Function " + ObjectTypeName() + ":setScale need three parameter" ).c_str() );

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
		luaL_argcheck( L, argc == 4, 4, ( "Function " + ObjectTypeName() + ":setPosition need three parameter" ).c_str() );

		pos.X = (float)lua_tonumber( L, 2 );
		pos.Y = (float)lua_tonumber( L, 3 );
		pos.Z = (float)lua_tonumber( L, 4 );
		IF_OBJECT_NOT_NULL_THEN object_->setPosition( pos );

		return 1;
	}
};
}//namespace nrp