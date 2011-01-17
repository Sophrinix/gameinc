#ifndef LUNA_H
#define LUNA_H 1

#include <lua.hpp>
//#include "nrpScript.h"
//обрати внимание на запятую в конце
#define LUNA_AUTONAME_FUNCTION(class, name) {#name, &class::name}, 
//и тут
#define LUNA_AUTONAME_PROPERTY(class, name, getr, setr) {name, &class::getr, &class::setr},

static void DumpStack(lua_State *L) 
{   
    int i, type;   
    int top = lua_gettop(L);   
	char dd[ MAX_PATH ];
    OutputDebugString("++++++++++++\n");   
    for(i=-1; i >= (-top); --i) { /* repeat for each level */   
        type = lua_type(L, i); 
		snprintf( dd, MAX_PATH, "pos bottom: %d: pos top: %d: = ",top+i+1,i );
        OutputDebugString( dd );   
        switch (type) {   
        case LUA_TSTRING:   
            snprintf( dd, MAX_PATH, "'%s'\n", lua_tostring(L, i));  
			OutputDebugString( dd );   
            break;   
        case LUA_TBOOLEAN:   
			OutputDebugString(  lua_toboolean(L, i)? "true":"false" );   
            break;   
        case LUA_TNUMBER:   
            snprintf( dd, MAX_PATH, "%g\n", lua_tonumber(L, i));   
			OutputDebugString( dd );   
            break;   
        default:                 /* all other values */   
            snprintf( dd, MAX_PATH, "%s\n", lua_typename(L, type));   
			OutputDebugString( dd );   
            break;   
        }   
    }   
    OutputDebugString("++++++++++++\n");   
}  

// convenience macros
template < class T > class Luna 
{
    typedef struct 
	{
		T              *pT;
    } userdataType;
    
  public:

    enum { NUMBER, STRING };

    struct PropertyType 
	{
		const char     *name;
		int             (T::*getter) (lua_State *);
		int             (T::*setter) (lua_State *);
    };

    struct FunctionType 
	{
		const char     *name;
		int             (T::*function) (lua_State *);
    };

/*
  @ check
  Arguments:
    * L - Lua State
    * narg - Position to check

  Description:
    Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
    This function will raise an exception if the argument is not of the correct type.
*/
static T       *check(lua_State * L, int narg) 
{
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_checkudata(L, -1, T::ClassName));
		    if (!ud)
			    luaL_typerror(L, narg+1, T::ClassName);
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    luaL_typerror(L, narg+1, T::ClassName);
	    } 
}

/*
  @ lightcheck
  Arguments:
    * L - Lua State
    * narg - Position to check

  Description:
    Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
    This function will return NULL if the argument is not of the correct type.  Useful for supporting
    multiple types of arguments passed to the function
*/ 
static T       *lightcheck(lua_State * L, int narg) 
{
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_testudata(L, -1, T::ClassName));
		    if (!ud)
			    return NULL; // lightcheck returns NULL if not found.
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    return NULL;
	    } 
}

/*
  @ Register
  Arguments:
    * L - Lua State
    * namespac - Namespace to load into

  Description:
    Registers your class with Lua.  Leave namespac "" if you want to load it into the global space.
*/
    // REGISTER CLASS AS A GLOBAL TABLE 
static void     Register(lua_State * L, const char *namespac="") 
{
	if ( strcmp(namespac, "") != 0) 
	{
	    lua_getglobal(L, namespac);
	    lua_pushcfunction(L, &Luna < T >::constructor);
	    lua_setfield(L, -2, T::ClassName() );
	    lua_pop(L, 1);
	} 
	else 
	{
	    lua_pushcfunction(L, &Luna < T >::constructor);
	    lua_setglobal(L, T::ClassName() );
	}

	luaL_newmetatable(L, T::ClassName() );
	int metatable = lua_gettop(L);

	lua_pushstring(L, "__gc"); 
	lua_pushcfunction(L, &Luna < T >::gc_obj);
	lua_settable(L, metatable);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, &Luna < T >::property_getter);
	lua_settable(L, metatable);

	lua_pushstring(L, "__setindex"); 
	lua_pushcfunction(L, &Luna < T >::property_setter);
	lua_settable(L, metatable);

	lua_pop( L, 1 );
}

/*
  @ constructor (internal)
  Arguments:
    * L - Lua State
*/
static int      constructor(lua_State * L) 
{
	T *obj = new T(L, true);

	lua_newtable(L);

	int newtable = lua_gettop(L);

	lua_pushnumber(L, 0);
	T **a = (T **) lua_newuserdata(L, sizeof(T *));
	*a = obj;

	int userdata = lua_gettop(L);

	luaL_getmetatable(L, T::ClassName() );

	lua_setmetatable(L, userdata);

	lua_settable(L, newtable);

	luaL_getmetatable(L, T::ClassName() );
	lua_setmetatable(L, newtable);

	luaL_getmetatable(L, T::ClassName() );

	for (int i = 0; T::props[i].name; i++) 
	{
	    lua_pushstring(L, T::props[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	lua_pop(L, 1);

	for (int i = 0; T::methods[i].name; i++) 
	{
	    lua_pushstring(L, T::methods[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return 1;
}

/*
  @ createNew
  Arguments:
    * L - Lua State

  Description:
    Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
*/
static T *createNew(lua_State * L) 
{
	lua_newtable(L);

	int             newtable = lua_gettop(L);

	lua_pushnumber(L, 0);

	T             **a = (T **) lua_newuserdata(L, sizeof(T *));
	T              *obj = new T(L, false);
	obj->isExisting = false;
	*a = obj;

	int             userdata = lua_gettop(L);

	luaL_getmetatable(L, T::ClassName);

	lua_setmetatable(L, userdata);

	lua_settable(L, newtable);

	luaL_getmetatable(L, T::ClassName);
	lua_setmetatable(L, newtable);

	luaL_getmetatable(L, T::ClassName);

	for (int i = 0; T::props[i].name; i++) 
	{
	    // ADD NAME KEY 
	    lua_pushstring(L, T::Properties[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	lua_pop(L, 1);

	for (int i = 0; T::methods[i].name; i++) 
	{
	    lua_pushstring(L, T::methods[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return obj;
}

/*
  @ createFromExisting
  Arguments:
    * L - Lua State
    * existingobj - Existing instance of object

  Description:
    Loads an instance of the class into the Lua stack, instead using an existing object rather than creating a new one.
    Returns the existing object.
*/
static T* createFromExisting(lua_State * L, T * existingobj) 
{

	lua_newtable(L);

	int             newtable = lua_gettop(L);

	lua_pushnumber(L, 0);

	T             **a = (T **) lua_newuserdata(L, sizeof(T *));
	T              *obj = existingobj;
	obj->isExisting = true;
	*a = obj;

	int             userdata = lua_gettop(L);


	luaL_getmetatable(L, T::ClassName);

	lua_setmetatable(L, userdata);

	lua_settable(L, newtable);

	luaL_getmetatable(L, T::ClassName);
	lua_setmetatable(L, newtable);

	luaL_getmetatable(L, T::ClassName);

	for (int i = 0; T::props[i].name; i++) 
	{
	    lua_pushstring(L, T::props[i].name);
	    lua_pushnumber(L, i);
	    lua_settable(L, -3);
	}

	lua_pop(L, 1);

	for (int i = 0; T::methods[i].name; i++) {
	    lua_pushstring(L, T::methods[i].name);
	    lua_pushnumber(L, i);
	    lua_pushcclosure(L, &Luna < T >::function_dispatch, 1);
	    lua_settable(L, newtable);
	}

	return obj;
}

/*
  @ property_getter (internal)
  Arguments:
    * L - Lua State
*/
static int      property_getter(lua_State * L) 
{
	//_eScript.DumpStack();
	lua_pushvalue(L, 2); //_eScript.DumpStack();

	lua_getmetatable(L, 1); //_eScript.DumpStack();

	lua_pushvalue(L, 2); //_eScript.DumpStack();
	lua_rawget(L, -2); //_eScript.DumpStack();

	if (lua_isnumber(L, -1)) 
	{
	    int _index = static_cast< int >( lua_tonumber(L, -1) );

	    lua_pushnumber(L, 0);
	    lua_rawget(L, 1);

	    T **obj = static_cast < T ** >(lua_touserdata(L, -1));

	    lua_pushvalue(L, 3);

	    const PropertyType *_properties = (*obj)->T::props;

		int result = ((*obj)->*(T::props[_index].getter)) (L);

	    return result;
	}
	// PUSH NIL 
	lua_pushnil(L);

	return 1;
}

/*
  @ property_setter (internal)
  Arguments:
    * L - Lua State
*/
static int      property_setter(lua_State * L) 
{
	lua_getmetatable(L, 1); 

	lua_pushvalue(L, 2); 
	lua_rawget(L, -2);

	if (lua_isnil(L, -1)) 
	{

	    lua_pop(L, 2);

	    lua_rawset(L, 1);

	    return 0;
	} 
	else 
	{
	    int _index = static_cast< int >( lua_tonumber(L, -1) );

	    lua_pushnumber(L, 0);
	    lua_rawget(L, 1);

	    T             **obj =
		static_cast < T ** >(lua_touserdata(L, -1));

	    lua_pushvalue(L, 3);

	    const PropertyType *_properties = (*obj)->T::props;

	    return ((*obj)->*(T::props[_index].setter)) (L);

	}
}

/*
  @ function_dispatch (internal)
  Arguments:
    * L - Lua State
*/
static int      function_dispatch(lua_State * L) 
{
	int             i = (int) lua_tonumber(L, lua_upvalueindex(1));
	lua_pushnumber(L, 0);
	lua_rawget(L, 1);
	T             **obj = static_cast < T ** >(lua_touserdata(L, -1));
	lua_pop(L, 1);

	return ((*obj)->*(T::methods[i].function)) (L);
}

/*
  @ gc_obj (internal)
  Arguments:
    * L - Lua State
*/
static int      gc_obj(lua_State * L) 
{
	T             **obj =
	    static_cast < T ** >(luaL_checkudata(L, -1, T::ClassName() ));

	if (!(*obj)->IsExist() && !(*obj)->IsPrecious() )
	{
		//qDebug() << "Cleaning up a " << T::ClassName() << "." << endl;
		delete(*obj);
	}

	return 0;
}

};

#endif /* LUNA_H */

