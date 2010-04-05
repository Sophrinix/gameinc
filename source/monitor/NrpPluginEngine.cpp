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

std::vector< std::string > CNrpPluginEngine::FindLibraries( std::string pluginFolder )
{
	std::vector< std::string > libraries_;

	std::string pluginPath = pluginFolder+"*";

	if( pluginPath.empty() || pluginPath.size() > MAX_PATH )
		return libraries_;

	WIN32_FIND_DATA findData;
	HANDLE findFile = FindFirstFile( pluginPath.c_str(), &findData );

	if( findFile==INVALID_HANDLE_VALUE) return libraries_;

	for(;;)
	{
		if( strcmp( findData.cFileName, "." ) != 0 &&
			strcmp( findData.cFileName, ".." ) != 0 && 
			(findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			char* rStr = strrchr( findData.cFileName, '.' );

			if( strcmp( rStr, ".plugin" ) == 0 )
				libraries_.push_back( pluginFolder + findData.cFileName );
		}

		if( !FindNextFile(findFile, &findData) ) break;
	}

	FindClose( findFile );

	return libraries_;
}

void CNrpPluginEngine::RegisterLibraries( std::vector< std::string >& paths )
{
	std::vector< std::string >::iterator pIter = paths.begin();

	for( ; pIter != paths.end(); pIter++ )
		 RegisterLibrary_( *pIter );
}

void CNrpPluginEngine::RegisterLibrary_( std::string pathToDLL )
{
	HMODULE handleLib = LoadLibrary( pathToDLL.c_str() ); 

	if( handleLib == NULL)
	{
		std::string error = "Не удалось загрузить библиотеку " + pathToDLL;
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

		Log( PLUGIN, FATAL ) << "Загружен плагин " << libraryName << term;
	}
	else
		Log( PLUGIN, FATAL ) << "Неправильная версия плагина " << pathToDLL.c_str() << term;
}

HMODULE CNrpPluginEngine::GetLibrary( std::string name )
{
	return ( plugins_.find( name ) != plugins_.end() ) ? plugins_[ name ] : NULL;
}
}//namespace plugin

}//namespace nrp