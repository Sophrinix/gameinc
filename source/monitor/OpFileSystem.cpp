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

void OpFileSystem::Remove( const NrpText& pathTo )
{
	if( !IsFolder( pathTo ) ) //файл можно удалить сразу
		DeleteFileW( pathTo.ToWide() );
	else
	{
		NrpText mStr = CheckEndSlash( pathTo );
		_wfinddata_t fdata;	
		intptr_t hFile;

		assert( IsExist( mStr ) );
		if( IsExist( mStr ) )
		{
			hFile = _wfindfirst( ( mStr+ NrpText("\\*.*") ).ToWide(), &fdata);
			while( hFile > 0 )
			{
				if ( !( wcscmp( fdata.name, L".") == 0 || wcscmp( fdata.name, L".." ) == 0 ) )// это удалять не надо
					if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
					{
						Remove( CheckEndSlash( mStr ) + NrpText( fdata.name ) );
					}
					else// иначе найден файл
					{
						DeleteFileW( ( CheckEndSlash( mStr ) + fdata.name ).ToWide() );
					}

					if( _wfindnext( hFile, &fdata) != 0 )
						break;
			}
		}

		_findclose( hFile );
		RemoveDirectoryW( mStr.ToWide() );
	}
}

void OpFileSystem::Move( const NrpText& pathOld, const NrpText& pathNew )
{
	NrpText workDir = _nrpApp[ WORKDIR ];
	NrpText mStr = RemoveEndSlash( workDir + pathOld );
	NrpText mStr2 = RemoveEndSlash( workDir + pathNew );

	SHFILEOPSTRUCTW sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCTW ) );
	sh.hwnd   = _nrpEngine.GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_MOVE;

	wchar_t from[ MAX_PATH ] = { 0 };
	int iSize = mStr.size();
	wcsncpy_s( from, MAX_PATH-1, mStr.ToWide(), iSize );
	from[ iSize ] = 0;
	from[ iSize+1 ] = 0;

	iSize = mStr2.size();
	wchar_t to[ MAX_PATH ] = { 0 };
	wcsncpy_s( to, MAX_PATH-1, mStr2.ToWide(), iSize );
	to[ iSize ] = 0;
	to[ iSize+1 ] = 0;

	sh.pFrom  = from;
	sh.pTo    = to;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;//FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	int j = SHFileOperationW(&sh);
	j = GetLastError();
}

void OpFileSystem::Copy( const NrpText& pathOld, const NrpText& pathNew )
{
	NrpText mStr = RemoveEndSlash( pathOld ), 
			mStr2 = RemoveEndSlash( pathNew );

	SHFILEOPSTRUCTW sh;
	memset( &sh, 0, sizeof( SHFILEOPSTRUCTW ) );
	sh.hwnd   = _nrpEngine.GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_COPY;

	wchar_t from[ MAX_PATH ] = { 0 };
	int iSize = mStr.size();
	wcsncpy_s( from, MAX_PATH-1, mStr.ToWide(), iSize );
	from[ iSize ] = 0;
	from[ iSize+1 ] = 0;

	iSize = mStr2.size();
	wchar_t to[ MAX_PATH ] = { 0 };
	wcsncpy_s( to, MAX_PATH-1, mStr2.ToWide(), iSize );
	to[ iSize ] = 0;
	to[ iSize+1 ] = 0;

	sh.pFrom  = from;
	sh.pTo    = to;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperationW(&sh);
}

void OpFileSystem::CreateDirectorySnapshot( const NrpText& directory,
											const NrpText& saveFile, 
											const NrpText& templateName,
											const NrpText& itemName )
{
	std::auto_ptr<IniFile> ini( new IniFile( saveFile ) );

	int number= ini->Get( "options", templateName + L"Number", (int)0 );
	_wfinddata_t fdata;	
	intptr_t hFile;

	assert( directory.size() );
	if( directory.size() )
	{
		hFile = _wfindfirst( directory + L"\\*.*", &fdata);
		while( hFile )
		{
			if ( !( wcscmp( fdata.name, L".") == 0 || wcscmp( fdata.name, L".." ) == 0 ) )// это удалять не надо
				if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
				{
					CreateDirectorySnapshot( CheckEndSlash( directory ) + NrpText( fdata.name ), saveFile, templateName, itemName );
				}
				else// иначе найден файл
				{
					if( _wcsicmp( itemName.ToWide(), fdata.name ) == 0 )
					{
						NrpText fileName = CheckEndSlash( directory )+ fdata.name;
						IniFile rv( fileName );

						ini->Set( L"options", NrpText( "name" ) + NrpText( (int)number ), rv.Get( "properties", "internalname:string", NrpText( "" ) ) );
						ini->Set( L"options", templateName + NrpText( (int)number ), fileName );
						number++;
						ini->Set( L"options", templateName + L"Number", number );
					}
				}
			
			if( _wfindnext( hFile, &fdata) != 0 )
				break;
		}
	}

	_findclose( hFile );
}

NrpText OpFileSystem::CheckEndSlash( NrpText pathTo )
{
	wchar_t endsym = pathTo[ pathTo.size() - 1 ];
	if( endsym != L'/' && endsym != L'\\' )
		pathTo.append( L"/" );

	return pathTo;
}

NrpText OpFileSystem::RemoveEndSlash( NrpText pathTo )
{
	wchar_t endsym = pathTo[ pathTo.size() - 1 ];
	if( endsym == L'/' || endsym == L'\\' )
		pathTo.erase( pathTo.size()-1 );

	return pathTo;
}

void OpFileSystem::CreateDirectory( NrpText pathTo )
{
	pathTo = CheckEndSlash( pathTo );
	if( !IsExist( pathTo ) )
		::CreateDirectoryW( pathTo, NULL );

	assert( IsExist( pathTo ) );
}

bool OpFileSystem::IsExist( const NrpText& pathTo )
{
	bool ex = ( _waccess( pathTo.ToWide(), 0 ) != -1);

#ifdef _DEBUG
	if( ! ex )
		Log(SCRIPT) << "Yказанный файл не существует" <<  pathTo << term;
#endif
	return ex;
}

bool OpFileSystem::IsFolder( const NrpText& pathTo )
{
	NrpText myPath = RemoveEndSlash( pathTo );
	assert( IsExist( myPath ) );
	
	_wfinddata_t fdata;	
	intptr_t hFile;

	hFile = _wfindfirst( myPath, &fdata);
	if( hFile != -1 )
	{
		return ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ));// это папка
	}

	return false;
}

NrpText OpFileSystem::GetExtension( const NrpText& pathTo )
{
	assert( !IsFolder( pathTo ) );
	int index = pathTo.findLast( L'.' );
	if( index >= 0 )
	    return pathTo.subString( index, 0xff );

	return "";
}

NrpText OpFileSystem::UpDir( const NrpText& pathTo )
{
	assert( IsExist( pathTo ) );
	if( !pathTo.size() )
		return L"";

	NrpText pathToAny = RemoveEndSlash( pathTo );
	int index = pathToAny.findLast( L'\\' );
	if( index < 0 )
		index = pathToAny.findLast( L'/' );

	if( index >=0 )
		return pathToAny.subString( 0, index+1 );

	assert( IsExist( pathTo ) );
	return L"";
}

NrpText OpFileSystem::CheckFile( const NrpText& dir, const NrpText& fileName )
{
	assert( IsExist( dir ) );
	if( !fileName.size() )
		return L"";

	if( IsExist( fileName ) )
		return fileName;

	NrpText finalName = CheckEndSlash( dir ) + fileName; 
	if( IsExist( finalName ) )
		return finalName;

	return fileName;
}

}//end namespace nrp