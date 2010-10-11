#include "stdafx.h"
#include <sstream>
#include <lua.hpp>
#include <luna.h>
#include <windows.h>
#include <time.h>
#include <irrlicht.h>
#include <assert.h>
#include <typeinfo>

#include "nrpEngine.h"
#include "nrpScript.h"
#include "nrpScriptCore.h"
#include "nrpTerrainMaterial.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"
#include "nrpScene.h"
#include "NrpPluginEngine.h"
#include "NrpApplication.h"
#include "NrpPlant.h"
#include "NrpTranslate.h"
#include "HTMLEngine.h"
#include "LuaPlant.h"
#include "LuaGuiEnvironment.h"
#include "LuaApplication.h"
#include "LuaBrowser.h"
#include "LuaDriver.h"
#include "LuaSceneManager.h"
#include "OpFileSystem.h"

using namespace irr;

namespace nrp
{

NumArray *check( lua_State *vm, int index )
{
	void *user = luaL_checkudata( vm,index,"array" );
	luaL_argcheck( vm,user!=0,index,"array expected" );
	return (NumArray*) user;
}

double &getvalue( lua_State *vm )
{
	NumArray *a = check( vm,1 );
	int index = luaL_checkint( vm,2 );
	luaL_argcheck( vm, 1 <= index && index <= a->size, 2,"out of range" );
	return a->values[index-1];
}

int NewArray( lua_State *vm )
{
	int n = luaL_checkint( vm,1 );
	size_t nbytes = sizeof( NumArray ) + ( n-1 )*sizeof( double );
	NumArray * a = (NumArray*) lua_newuserdata( vm,nbytes );
	luaL_getmetatable( vm,"array" );
	lua_setmetatable( vm,-2 );
	a->size = n;
	return 1;
} 

int SetArray( lua_State *vm )
{
	getvalue(vm) = (double) lua_tonumber( vm,3 );
	return 0;
}

int GetArray( lua_State *vm )
{
	lua_pushnumber( vm,(lua_Number)getvalue(vm) );
	return 1;
}

int GetSize( lua_State *vm )
{
	NumArray *a = check( vm,1 );
	lua_pushnumber( vm,a->size );
	return 1;
}

int tostring( lua_State *vm )
{
	NumArray *a = check( vm,1 );
	lua_pushfstring( vm,"array(%d)",a->size );
	return 1;
}


int AboutRobots(lua_State *vm)
{
	::MessageBox(0, "NerpaScript - простой скриптовый язык на основе свободно \nраспространяемого интерпретатора Lua 5.1.\n\nРазработчик - d.a. (glukensoft@gmail.com)", "О роботах", 0);
	return 1;
}

int SocketOpen( lua_State *vm )
{
	int port = lua_tointeger( vm,1 );
	::MessageBox(0, "Don't yet work :(", "", 0 );
	return 1;
}

int SocketClose( lua_State *vm )
{
	int port = lua_tointeger( vm,1 );
	::MessageBox(0, "Don't yet work :(", "", 0 );
	return 1;
}

void PrintCmdHelp(std::string cmd, std::string help)
{
	//InfoLog(gfx) << all << "*\t\t" << cmd << term;
	//InfoLog(gfx) << all << "\t\t\t\t" << help  << term;
}

int Help(lua_State *vm)
{
	PrintCmdHelp("help()", "Печатает это сообщение");
	return 1;
}

int CreateTerrainShader( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CreateTerrain need 7 parameter");

	scene::ISceneManager* smgr = CNrpEngine::Instance().GetSceneManager();
	video::IVideoDriver* driver = smgr->getVideoDriver();

	const char* vertexFilename = lua_tostring(vm, 1);
	assert( vertexFilename != NULL );
	const char* vertexFuncname = lua_tostring(vm, 2);	
	assert( vertexFuncname != NULL );
	const char* fragFilename = lua_tostring(vm, 3);	
	assert( fragFilename != NULL );
	const char* fragFuncname = lua_tostring(vm, 4);	
	assert( fragFuncname != NULL );

	scene::ISceneNode* terrain = (scene::ITerrainSceneNode*)lua_touserdata(vm, 5);
	const char* seaFloorFilename = lua_tostring(vm, 6);	
	assert( seaFloorFilename != NULL );
	float waterPosY = (float)lua_tonumber( vm, 7 );

	video::CNrpTerrainMaterial* terra_shader = new video::CNrpTerrainMaterial( CNrpEngine::Instance().GetDevice(), 
																						 terrain, 
																						 driver->getTexture( seaFloorFilename ),
																						 waterPosY );

	s32 ter_mat = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
															vertexFilename, vertexFuncname, video::EVST_VS_2_0, 
															fragFilename, fragFuncname, video::EPST_PS_2_0,
															terra_shader,
															video::EMT_TRANSPARENT_ALPHA_CHANNEL );

	lua_pushinteger( vm, ter_mat );
	return 1;
}

int ScriptLog( lua_State *vm )
{
	int argc = lua_gettop(vm);

	if (!lua_istable(vm, 1))
		return 0;

	// Этот экземпляр структуры Log отойдет в БД
	Log myLog;

	// Получаем значение источника сообщения
	lua_pushstring(vm, "src");
	lua_gettable(vm, 1);
	myLog.Source = (SubSystem)lua_tointeger(vm, -1);
	lua_pop(vm, 1);

	// Получаем значение устройства назначения
	lua_pushstring(vm, "dev");
	lua_gettable(vm, 1);
	int luaDev = (int)lua_tointeger(vm, -1);
	myLog.Device = luaDev ? luaDev : ODS | CON | NRPDB;
	lua_pop(vm, 1);

	// Получаем значение важности сообщения
	lua_pushstring(vm, "sev");
	lua_gettable(vm, 1);
	myLog.Sev = (SeverityLevel)lua_tointeger(vm, -1);
	lua_pop(vm, 1);

	const char* msgText1 = lua_tostring(vm, 2);

	for (int nArg = 2; nArg <= argc; nArg ++)
	{		
		const char* param = lua_tostring(vm, nArg);
		myLog << param;
	}
	myLog << term;
	return 1;
}

int GetGuiEnvironment( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function NrpGetGuiEnvironment not need any parameter");

	lua_pushlightuserdata( vm, (void*)CNrpEngine::Instance().GetGuiEnvironment() );
	Luna< CLuaGuiEnvironment >::constructor( vm );

	return 1;
}

int GetBrowser( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetBrowser not need any parameter");

	lua_pushlightuserdata( vm, &HTMLEngine::Instance() );
	Luna< CLuaBrowser >::constructor( vm );

	return 1;
}

int GetVideoDriver( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetVideoDriver not need any parameter");

	lua_pushlightuserdata( vm, (void*)CNrpEngine::Instance().GetVideoDriver() );
	Luna< CLuaDriver >::constructor( vm );

	return 1;
}

int GetSceneManager( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetSceneManager not need any parameter");

	lua_pushlightuserdata( vm, (void*)CNrpEngine::Instance().GetSceneManager() );
	Luna< CLuaSceneManager >::constructor( vm );

	return 1;
}

int AddScene( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function AddScene need 2 parameter");

	const char* name = lua_tostring( vm, 1 );
	assert( name != NULL );
	const char* fileName = lua_tostring( vm, 2 );
	assert( fileName != NULL );

	CNrpEngine::Instance().AddScene( name, fileName );

	return 1;
}

int SetNextScene( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function SetNextScene need 1 parameter");

	const char* name = lua_tostring( vm, 1 );
	assert( name != NULL );

	CNrpEngine::Instance().SetStatus( CNrpEngine::LOAD_SCENE, (void*)name );

	return 1;
}

int InitializeLoginScene( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function InitializeLoginScene need 1 parameter");

	CNrpEngine::Instance().InitializeScene( CNrpEngine::LOGIN_SCENE );

	return 1;
}

int InitializeWorkScene( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function InitializeWorkScene need 1 parameter");

	CNrpEngine::Instance().InitializeScene( CNrpEngine::WORK_SCENE );

	return 1;
}

int SetSceneOption( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function AddScene need 2 parameter");

	const char* name = lua_tostring( vm, 1 );
	assert( name != NULL );
	bool amount = lua_tointeger( vm, 2 ) > 0;

	CNrpEngine::Instance().GetCurrentScene()->SetOption( name, amount );

	return 1;
}

int LoadPlugins( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function NrpLoadPlugins need 1 parameter");

	const char* folderName = lua_tostring( vm, 1 );
	assert( folderName != NULL );

	std::vector< std::string > paths = plugin::CNrpPluginEngine::Instance().FindLibraries( folderName );
	plugin::CNrpPluginEngine::Instance().RegisterLibraries( paths );

	return 1;
}

int ApplicationClose( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function ApplicationClose not need any parameter");

	CNrpEngine::Instance().SetStatus( CNrpEngine::CLOSE, NULL );

	return 1;
}

int ApplicationSave( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function ApplicationClose not need any parameter");

	CNrpApplication::Instance().SaveProfile();

	return 1;
}

int GetApplication( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetApplication not need any parameter");

	lua_pushlightuserdata( vm, (void*)&CNrpApplication::Instance() );
	Luna< CLuaApplication >::constructor( vm );

	return 1;	
}

int GetPlant( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetPlant not need any parameter");

	lua_pushlightuserdata( vm, (void*)&CNrpPlant::Instance() );
	Luna< CLuaPlant >::constructor( vm );

	return 1;	
}

int ApplicationGetTranslate( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function LoadLanguageFile need string parameter");

	const char* name = lua_tostring( vm, 1 );
	assert( name != NULL );

	const char* resultt = translate::GetTranslate( name );

	lua_pushstring( vm, resultt );
	return 1;
}

int ApplicationGetTickCount( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function GetTickCount not need any parameter");

	lua_pushinteger( vm, GetTickCount() );

	return 1;	
}

int ApplicationGetSender( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function ApplicationGetSender not need parameter");

	lua_pushlightuserdata( vm, CNrpScript::Instance().GetSender() );
	
	return 1;
}

int ApplicationRemoveFile( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function RemoveFile need path parameter");

	const char* pathh = lua_tostring( vm, 1 );
	assert( pathh != NULL );

	if( pathh != NULL )
		OpFileSystem::Remove( pathh );

	return 1;
}
}//namespace nrp