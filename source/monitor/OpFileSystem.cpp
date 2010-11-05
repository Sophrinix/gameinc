#include "StdAfx.h"
#include "OpFileSystem.h"
#include <ShellAPI.h>
#include "nrpEngine.h"
#include "IniFile.h"
#include "NrpApplication.h"

#include <io.h>
#include <assert.h>

namespace nrp
{
	
OpFileSystem::OpFileSystem(void)
{
}

OpFileSystem::~OpFileSystem(void)
{
}

void OpFileSystem::Remove( const std::string& pathTo )
{
	if( !IsFolder( pathTo ) ) //файл можно удалить сразу
		DeleteFile( pathTo.c_str() );
	else
	{
		std::string mStr = CheckEndSlash( pathTo );
		_finddata_t fdata;	
		intptr_t hFile;

		assert( IsExist( mStr ) );
		if( IsExist( mStr ) )
		{
			hFile = _findfirst( (mStr+"\\*.*").c_str(), &fdata);
			while( hFile > 0 )
			{
				if ( !( strcmp( fdata.name, ".") == 0 || strcmp( fdata.name, ".." ) == 0 ) )// это удалять не надо
					if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
					{
						Remove( mStr + "/" + std::string( fdata.name ) );
					}
					else// иначе найден файл
					{
						DeleteFile( ( CheckEndSlash( mStr ) + std::string( fdata.name )).c_str() );
					}

					if( _findnext( hFile, &fdata) != 0 )
						break;
			}
		}

		_findclose( hFile );
		RemoveDirectory( mStr.c_str() );
	}
}

void OpFileSystem::Move( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = RemoveEndSlash( CNrpApplication::Instance().GetString( WORKDIR ) + pathOld );
	std::string mStr2 = RemoveEndSlash( CNrpApplication::Instance().GetString( WORKDIR ) + pathNew );

	SHFILEOPSTRUCT sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCT ) );
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_MOVE;

	char from[ MAX_PATH ] = { 0 };
	strncpy_s( from, MAX_PATH-1, mStr.c_str(), mStr.length() );

	char to[ MAX_PATH ] = { 0 };
	strncpy_s( to, MAX_PATH - 1, mStr2.c_str(), mStr2.length() );

	sh.pFrom  = from;
	sh.pTo    = to;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::Copy( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = RemoveEndSlash( pathOld ), 
				mStr2 = RemoveEndSlash( pathNew );

	SHFILEOPSTRUCT sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCT ) );
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_COPY;
	char from[ MAX_PATH ] = { 0 };
	strncpy_s( from, MAX_PATH-1, mStr.c_str(), mStr.length() );

	char to[ MAX_PATH ] = { 0 };
	strncpy_s( to, MAX_PATH-1, mStr2.c_str(), mStr2.length() );
	sh.pFrom  = from;
	sh.pTo    = to;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::CreateDirectorySnapshot( const std::string& directory,
											const std::string& saveFile, 
											const std::string& templateName,
											const std::string& itemName )
{
	int number= IniFile::Read( "options", templateName + "Number", (int)0, saveFile );
	_finddata_t fdata;	
	intptr_t hFile;

	assert( directory.size() );
	if( directory.size() )
	{
		hFile = _findfirst( (directory+"\\*.*").c_str(), &fdata);
		while( hFile )
		{
			if ( !( strcmp( fdata.name, ".") == 0 || strcmp( fdata.name, ".." ) == 0 ) )// это удалять не надо
				if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
				{
					CreateDirectorySnapshot( directory + "/" + std::string( fdata.name ), saveFile, templateName, itemName );
				}
				else// иначе найден файл
				{
					if( _stricmp( itemName.c_str(), fdata.name ) == 0 )
					{
						IniFile::Write( "options", templateName + conv::ToStr( number ), directory+"/"+std::string( fdata.name ), saveFile );
						number++;
						IniFile::Write( "options", templateName + "Number", number, saveFile );
					}
				}
			
			if( _findnext( hFile, &fdata) != 0 )
				break;
		}
	}

	_findclose( hFile );
}

std::string OpFileSystem::CheckEndSlash( std::string pathTo )
{
	char endsym = pathTo[ pathTo.length() - 1 ];
	if( endsym != '/' && endsym != '\\' )
		pathTo += "/";

	return pathTo;
}

std::string OpFileSystem::RemoveEndSlash( std::string pathTo )
{
	char endsym = pathTo[ pathTo.length() - 1 ];
	if( endsym == '/' || endsym == '\\' )
		pathTo.erase( pathTo.end()-1 );

	return pathTo;
}

void OpFileSystem::CreateDirectory( std::string pathTo )
{
	pathTo = CheckEndSlash( pathTo );
	if( _access( pathTo.c_str(), 0 ) == -1 )
		::CreateDirectory( pathTo.c_str(), NULL );

	assert( _access( pathTo.c_str(), 0 ) != -1 );
}

bool OpFileSystem::IsExist( const std::string& pathTo )
{
	bool ex = (_access( pathTo.c_str(), 0 ) != -1);

#ifdef _DEBUG
	if( ! ex )
		Log(SCRIPT) << "Yказанный файл не существует" <<  pathTo << term;
#endif
	return ex;
}

bool OpFileSystem::IsFolder( const std::string& pathTo )
{
	std::string myPath = RemoveEndSlash( pathTo );
	assert( IsExist( myPath ) );
	
	_finddata_t fdata;	
	intptr_t hFile;

	hFile = _findfirst( myPath.c_str(), &fdata);
	if( hFile )
	{
		return ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ));// это папка
	}

	return false;
}

std::string OpFileSystem::GetExtension( const std::string& pathTo )
{
	assert( !IsFolder( pathTo ) );
	int index = pathTo.find_last_of( '.' );
	if( index >= 0 )
	    return pathTo.substr( index, 0xff );

	return "";
}

std::string OpFileSystem::UpDir( const std::string& pathTo )
{
	assert( IsExist( pathTo ) );
	if( pathTo.empty() )
		return "";

	std::string pathToAny = CheckEndSlash( pathTo );
	pathToAny.erase( pathToAny.end()-1 );
	int index = pathTo.find_last_of( '\\' );
	if( index < 0 )
		index = pathTo.find_last_of( '/' );

	if( index >=0 )
		return pathTo.substr( 0, index+1 );

	assert( IsExist( pathTo ) );
	return "";
}

}//end namespace nrp