#include "stdafx.h"
#include <lua.hpp>
#include <luna.h>
#include "nrpScriptCore.h"
#include "nrpScript.h"
#include "LuaTerrain.h"
#include "LuaButton.h"
#include "LuaWindow.h"
#include "LuaDriver.h"
#include "LuaMiniMap.h"
#include "LuaCamera.h"
#include "LuaScrollBar.h"
#include "LuaSceneManager.h"
#include "LuaGuiEnvironment.h"
#include "LuaTable.h"
#include "LuaTexture.h"
#include "LuaConsole.h"
#include "LuaSceneNode.h"
#include "LuaElement.h"
#include "LuaEvent.h"
#include "LuaChart.h"
#include "LuaComboBox.h"
#include "LuaEdit.h"
#include "LuaImage.h"
#include "LuaLabel.h"
#include "LuaCompany.h"
#include "LuaApplication.h"
#include "LuaUser.h"
#include "LuaBank.h"
#include "LuaProgressBar.h"
#include "LuaProject.h"
#include "LuaLinkBox.h"
#include "LuaGameEngine.h"
#include "LuaGameProject.h"
#include "LuaTechnology.h"
#include "LuaListBox.h"

static nrp::CNrpScript* global_script_engine = NULL;

namespace nrp
{

CNrpScript::CNrpScript() : INrpConfig( "CNrpScript", "nrpScript" ), vm_(0)
{
	Load_( "" );
	// NULL ���� ���� ������ ��������� ������
	if (!(vm_ = luaL_newstate()))
		Log(SCRIPT, FATAL) << "������������� �������������� NerpaScript �� ������� (������������ ������)" << term;

	// �� ���������
	luaL_openlibs(vm_);

	// ������������ ������� �� ���������� � http://www.lua.org/pil/	
	// �� ���� ������� �������� ���������� ��� �������� ������
	if (!luaL_newmetatable(vm_, "array"))
		Log(SCRIPT, FATAL) << "�������� ����������� ��� ��������� �������� ������ ��������." << term;
	
	try
	{
		// ������� ��� ��������� �������� � Lua
		// ����� ������ "m"
		lua_pushstring(vm_,"__tostring");
		lua_pushcfunction(vm_, tostring);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		// ����� ������ "m"
		lua_pushstring(vm_, "__index");
		lua_pushcfunction(vm_, GetArray);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		// ����� ������ "m"
		lua_pushstring(vm_ ,"__newindex");
		lua_pushcfunction(vm_, SetArray);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		lua_register(vm_, "array", NewArray);


		// ������� ������� :-)
		lua_register(vm_, "NrpAboutRobots", AboutRobots);

		// ��������� �����
		lua_register(vm_, "NrpSocketOpen", SocketOpen);

		// ��������� �����
		lua_register(vm_, "NrpSocketClose", SocketClose);
		
		lua_register( vm_, "NrpCreateTerrainShader", CreateTerrainShader );

		lua_register( vm_, "NrpGetGuiEnvironment", GetGuiEnvironment );

		lua_register( vm_, "NrpGetApplication", GetApplication );
		// ������ � ������ �� Lua
		lua_register(vm_, "Log", ScriptLog );

		lua_register( vm_, "NrpGetVideoDriver", GetVideoDriver );				//��������� �������� ��� ������ � �����������

		lua_register( vm_, "NrpGetSceneManager", GetSceneManager );				//�������� ���� irrlicht

		lua_register( vm_, "NrpAddScene", AddScene );							//������������ ����� ��� ������

		lua_register( vm_, "NrpSetNextScene", SetNextScene );					//������������� ����� ��� ��������		

		lua_register( vm_, "NrpSetSceneOption", SetSceneOption );			//��������� ������� ��� ���������� ����� ������� �����

		lua_register( vm_, "NrpHelp", Help);										//???			

		lua_register( vm_, "NrpInitializeLoginScene", InitializeLoginScene );   //������� �����-����� 

		lua_register( vm_, "NrpInitializeWorkScene", InitializeWorkScene );		//������� �������-�����

		lua_register( vm_, "NrpLoadPlugins", LoadPlugins );

		lua_register( vm_, "NrpApplicationClose", ApplicationClose );

		lua_register( vm_, "NrpApplicationSave", ApplicationSave );

		RegisterLuaClasses_();
	}
	catch (...)
	{
		Log(SCRIPT, FATAL) << "����������� ��������� ������ � NerpaScript ������ ��������." << term;
	}

	// �������� ���������� �� ��������� � ���� ������ ������� ��� Lua
	DoString("package.path = package.path..\";./scripts/?.lua\"");
}

void CNrpScript::RegisterLuaClasses_()
{
	Luna< CLuaWindow >::Register( vm_ );									//�������� ��� ����
	Luna< CLuaButton >::Register( vm_ );									//�������� ��� ������
	Luna< CLuaTerrain >::Register( vm_ );									//�����������		
	Luna< CLuaDriver >::Register( vm_ );									//�������
	Luna< CLuaMiniMap >::Register( vm_ );									//���������
	Luna< CLuaCamera >::Register( vm_ );									//������		
	Luna< CLuaScrollBar >::Register( vm_ );									//���������
	Luna< CLuaSceneManager >::Register( vm_ );								//���� ��������
	Luna< CLuaGuiEnvironment >::Register( vm_ );							//���
	Luna< CLuaTable >::Register( vm_ );										//�������		
	Luna< CLuaTexture >::Register( vm_ );									//��������
	Luna< CLuaConsole >::Register( vm_ );
	Luna< CLuaSceneNode >::Register( vm_ );
	Luna< CLuaElement >::Register( vm_ );
	Luna< CLuaEvent >::Register( vm_ );
	Luna< CLuaChart >::Register( vm_ );
	Luna< CLuaComboBox >::Register( vm_ );
	Luna< CLuaEdit >::Register( vm_ );
	Luna< CLuaImage >::Register( vm_ );
	Luna< CLuaLabel >::Register( vm_ );
	Luna< CLuaCompany >::Register( vm_ );
	Luna< CLuaApplication >::Register( vm_ );
	Luna< CLuaUser >::Register( vm_ );
	Luna< CLuaBank >::Register( vm_ );
	Luna< CLuaBank >::Register( vm_ );
	Luna< CLuaProject >::Register( vm_ );
	Luna< CLuaProgressBar >::Register( vm_ );
	Luna< CLuaLinkBox >::Register( vm_ );
	Luna< CLuaGameEngine >::Register( vm_ );
	Luna< CLuaGameProject >::Register( vm_ );
	Luna< CLuaTechnology >::Register( vm_ );
	Luna< CLuaListBox >::Register( vm_ );
}

CNrpScript::~CNrpScript()
{
	// ���������� �� ������ ����������� ����������
	try
	{
		lua_close(vm_);
		// �������������� ���������
		vm_ = 0;
	}
	catch (...)
	{
		// ������ �� ������, ��� ��� ������
	}


}

void CNrpScript::Reload()
{
	LoadFile( GetValue<std::string>( LOAD_FUNCTIONS_FILENAME ).c_str() );
}

void CNrpScript::LoadFile( const char* fileName )
{
	std::string fn( fileName );
	SetValue<std::string>( LOAD_FUNCTIONS_FILENAME, fn );
	if (luaL_loadfile(vm_, fileName ) != 0)
	{
		// ����������� ��������� �� ������
		const char* errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		if (errMsg)
			Log(SCRIPT, FATAL)  << errMsg  << term;
		else
			Log(SCRIPT, FATAL)  << "����������� ������ ������� \"" << fileName << "\"" << term;
	}
	else
		lua_pcall( vm_, 0, LUA_MULTRET, 0 );
}

void CNrpScript::DoFile( const char* fileName )
{
	// !0 � ������ ������ � �������
	if (luaL_dofile(vm_, fileName) != 0)
	{
		// ����������� ��������� �� ������
		const char* errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		if( errMsg )
			Log(SCRIPT, FATAL)  << "����������� ������ ������� \"" + std::string( fileName ) << term;
		else
			Log(SCRIPT, FATAL)  << errMsg  << term;

#ifdef _DEBUG
		OutputDebugString( errMsg );
		OutputDebugString( "\n" );
#endif
	}
}

void CNrpScript::DoString( const char* s )
{
	if (luaL_dostring(vm_, s) != 0)
	{
		// ����������� ��������� �� ������
		const char* errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		Log(SCRIPT, FATAL) << errMsg << term;
	}
}

CNrpScript& CNrpScript::Instance()
{
	if( global_script_engine == NULL )
		global_script_engine = new CNrpScript();
	
	return *global_script_engine;
}

void CNrpScript::CallFunction( const char* funcName, void* userData )
{
	lua_getfield( vm_, LUA_GLOBALSINDEX, funcName );
	lua_pushlightuserdata( vm_, userData );

	if( GetValue<bool>( SHOW_CALL_FUNCTION_NAME ) )
	{
#ifdef _DEBUG
		char text[MAX_PATH];
		sprintf_s( text, MAX_PATH, "LuaFuncCall: %s\n", funcName );
		//OutputDebugString( text );
#endif
	}

	if( lua_pcall( vm_, 1, LUA_MULTRET, 0 ) != 0 )
	{
		// ����������� ��������� �� ������
		const char* errMsg = lua_tostring(vm_, -1);
		assert( errMsg != NULL );

		lua_pop(vm_, -1);
		Log(SCRIPT, FATAL) << errMsg << term;

#ifdef _DEBUG
		OutputDebugString( errMsg );
		OutputDebugString( "\n" );
#endif
	}
}

void CNrpScript::Load_( char* file_name )
{
	CreateValue<std::string>( LOAD_FUNCTIONS_FILENAME, "" );
	CreateValue<bool>( SHOW_CALL_FUNCTION_NAME, true );
}
}//namespace nrp