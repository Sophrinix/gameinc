#include "stdafx.h"
#include "NrpTranslate.h"

#include <string>
#include <map>
#include <stdio.h>

typedef std::map< std::string, std::string > TRANSLATE_MAP;
static TRANSLATE_MAP gloabalMapTranslates;

namespace nrp
{

namespace translate
{

void LoadLanguageFile( const char* fileName )
{
	const size_t bufferLen = 1024 * 1024;
	char* buffer = new char[ bufferLen ];
	memset( buffer, 0, bufferLen );
	GetPrivateProfileSection( "translate", buffer, bufferLen, fileName );

	std::string readLine = buffer;
	std::string name, valuel;
	while( readLine != "" )
	{
		name = readLine.substr( 0, readLine.find( '=' ) );
		valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );

		gloabalMapTranslates[ name ] = valuel;
		memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
		readLine = buffer;
	}

	delete [] buffer;
}

const char* GetTranslate( const char* name )
{
	if( name[ 0 ] == '#' )
	{
		TRANSLATE_MAP::iterator pIter = gloabalMapTranslates.find( name+1 );
		if( pIter != gloabalMapTranslates.end() )
			return pIter->second.c_str(); 
	}

	return name;
}

}

}