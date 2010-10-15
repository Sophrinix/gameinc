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
	std::string mStr = pathTo;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );
	_finddata_t fdata;	
	intptr_t hFile;

	assert( mStr.size() );
	if( mStr.size() )
	{
		hFile = _findfirst( (mStr+"\\*.*").c_str(), &fdata);
		while( hFile )
		{
			if ( !( strcmp( fdata.name, ".") == 0 || strcmp( fdata.name, ".." ) == 0 ) )// это удалять не надо
				if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
				{
					Remove( mStr + "/" + std::string( fdata.name ) );
				}
				else// иначе найден файл
				{
					DeleteFile( (mStr + "/" + std::string( fdata.name )).c_str() );
				}

				if( _findnext( hFile, &fdata) != 0 )
					break;
		}
	}

	_findclose( hFile );
	RemoveDirectory( mStr.c_str() );
}

void OpFileSystem::Move( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = CNrpApplication::Instance().GetValue<std::string>( WORKDIR ) + pathOld;
	std::string mStr2 = CNrpApplication::Instance().GetValue<std::string>( WORKDIR ) + pathNew;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	if( mStr2[ mStr2.length() -1 ] == '\\' || mStr2[ mStr2.length() -1 ] == '/' )
		mStr2 = mStr2.substr( 0, mStr2.length() - 1 );

	SHFILEOPSTRUCT sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCT ) );
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_MOVE;

	char from[ MAX_PATH ] = { 0 };
	strncpy( from, mStr.c_str(), mStr.length() );

	char to[ MAX_PATH ] = { 0 };
	strncpy( to, mStr2.c_str(), mStr2.length() );

	sh.pFrom  = from;
	sh.pTo    = to;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::Copy( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = pathOld, mStr2 = pathNew;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	if( mStr2[ mStr2.length() -1 ] == '\\' || mStr2[ mStr2.length() -1 ] == '/' )
		mStr2 = mStr2.substr( 0, mStr2.length() - 1 );

	SHFILEOPSTRUCT sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCT ) );
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_COPY;
	char from[ MAX_PATH ] = { 0 };
	strncpy( from, mStr.c_str(), mStr.length() );

	char to[ MAX_PATH ] = { 0 };
	strncpy( to, mStr2.c_str(), mStr2.length() );
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
						IniFile::Write( "options", templateName + IntToStr( number ), directory+"/"+std::string( fdata.name ), saveFile );
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

}//end namespace nrp