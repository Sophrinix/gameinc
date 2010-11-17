#include "StdAfx.h"
#include "NrpPluginEngine.h"
#include "nrpScript.h"
#include <lua.hpp>

static nrp::plugin::CNrpPluginEngine* globalPluginEngine = NULL;
typedef void (*GetPluginName)(char*);
typedef void  (*RegisterFunctions)(lua_State*);
typedef void (*GetPluginVersion)( int &version );

namespace nrp
{

namespace plugin
{

CNrpPluginEngine::CNrpPluginEngine(void)
{
}

CNrpPluginEngine::~CNrpPluginEngine(void)
{
}

CNrpPluginEngine& CNrpPluginEngine::Instance()
{
	if( globalPluginEngine == NULL )
		globalPluginEngine = new CNrpPluginEngine();

	return *globalPluginEngine;
}

core::array< NrpText > CNrpPluginEngine::FindLibraries( const NrpText& pluginFolder )
{
	core::array< NrpText > libraries_;

	NrpText pluginPath = pluginFolder+"*";

	if( !pluginPath.size() || pluginPath.size() > MAX_PATH )
		return libraries_;

	WIN32_FIND_DATAW findData;
	HANDLE findFile = FindFirstFileW( pluginPath.ToWide(), &findData );

	if( findFile==INVALID_HANDLE_VALUE) return libraries_;

	for(;;)
	{
		if( wcscmp( findData.cFileName, L"." ) != 0 &&
			wcscmp( findData.cFileName, L".." ) != 0 && 
			(findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			wchar_t* rStr = wcsrchr( findData.cFileName, L'.' );

			if( wcscmp( rStr, L".plugin" ) == 0 )
				libraries_.push_back( pluginFolder + findData.cFileName );
		}

		if( !FindNextFileW(findFile, &findData) ) break;
	}

	FindClose( findFile );

	return libraries_;
}

void CNrpPluginEngine::RegisterLibraries(core::array< NrpText >& paths )
{
	for( u32 i=0; i < paths.size(); i++ )
		 RegisterLibrary_( paths[ i ] );
}

void CNrpPluginEngine::RegisterLibrary_( const NrpText& pathToDLL )
{
	HMODULE handleLib = LoadLibraryW( pathToDLL.ToWide() ); 

	if( handleLib == NULL)
	{
		NrpText error = NrpText( "Не удалось загрузить библиотеку " ) + pathToDLL;
		return;
	}

	GetPluginName pluginName = (GetPluginName)GetProcAddress( handleLib, "GetPluginName" );
	GetPluginVersion pluginVersion = (GetPluginVersion)GetProcAddress( handleLib, "GetPluginVersion" );

	if( pluginVersion && pluginName )
	{
		char libraryName[32];
		int version;
		
		pluginName( libraryName );
		pluginVersion( version );

		RegisterFunctions regfunc = (RegisterFunctions)GetProcAddress( handleLib, "RegisterFunctions" );

		regfunc( CNrpScript::Instance().GetVirtualMachine() );

		plugins_[ libraryName ] = handleLib;

		Log( PLUGIN, FATAL ) << L"Загружен плагин " << libraryName << term;
	}
	else
		Log( PLUGIN, FATAL ) << L"Неправильная версия плагина " << pathToDLL << term;
}

HMODULE CNrpPluginEngine::GetLibrary( const NrpText& name )
{
	if( plugins_.find( name ) != NULL ) 
		return plugins_[ name ];
	return NULL;
}
}//namespace plugin

}//namespace nrp