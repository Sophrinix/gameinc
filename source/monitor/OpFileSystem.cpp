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
			hFile = _wfindfirst( ( mStr+ NrpText("*.*") ).ToWide(), &fdata);
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

			_findclose( hFile );
			if( !RemoveDirectoryW( RemoveEndSlash( mStr ).ToWide() ) )
			{
				int rr = GetLastError();
				rr = rr;
			}
		}
	}
}

void OpFileSystem::Copy( const NrpText& pathOld, const NrpText& pathNew )
{
	NrpText newfile;
	intptr_t hFile;

	if( !IsExist( pathNew ) )
		CreateDirectory( pathNew );

	_wfinddata_t fdata;
	assert( IsExist( pathOld ) );
	if( IsExist( pathOld ) )
	{
		hFile = _wfindfirst( ( CheckEndSlash( pathOld )+ NrpText("*.*") ).ToWide(), &fdata);
		while( hFile > 0 )
		{
			if ( fdata.attrib & _A_SUBDIR )  // если нашли папку
			{
				if( !(wcscmp( fdata.name, L"." ) == 0 || wcscmp( fdata.name, L".." ) == 0) )
				{
					Copy( CheckEndSlash( pathOld ) + NrpText( fdata.name ),
						  CheckEndSlash( pathNew ) + NrpText( fdata.name ) );// Рекурсивный вызов
				}
			}
			else // если нашли файл
			{
				newfile = CheckEndSlash( pathNew ) + fdata.name;
			    CopyFileW( ( CheckEndSlash( pathOld ) + fdata.name ).ToWide(), newfile.ToWide(), true);
			}

			if( _wfindnext( hFile, &fdata) != 0 )
				break;
		}

		_findclose( hFile );
	}
	return;
}

void OpFileSystem::CreateDirectorySnapshot( const NrpText& directory, const NrpText& templateName,
											const NrpText& itemName, IniFile* ini )
{
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
					CreateDirectorySnapshot( CheckEndSlash( directory ) + NrpText( fdata.name ), templateName, itemName, ini );
				}
				else// иначе найден файл
				{
					if( _wcsicmp( itemName.ToWide(), fdata.name ) == 0 )
					{
						NrpText fileName = CheckEndSlash( directory )+ fdata.name;
						IniFile rv( fileName );

						int number= ini->Get( "options", templateName + L"Number", (int)0 );

						NrpText intName = rv.Get( "properties", "internalname:string", NrpText( "" ) );
						ini->Set( L"options", NrpText( "name" ) + NrpText( (int)number ), intName );
						ini->Set( L"options", templateName + NrpText( (int)number ), fileName );
						ini->Set( L"options", templateName + L"Number", number+1 );
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
		::CreateDirectoryW( RemoveEndSlash( pathTo ), NULL );

	assert( IsExist( pathTo ) );
}

bool OpFileSystem::IsExist( const NrpText& pathTo )
{
	bool ex = ( _waccess( pathTo.ToWide(), 0 ) != -1 );

/*
#ifdef _DEBUG
	if( !ex )
		Log(SCRIPT) << "Указанный файл не существует " <<  pathTo << term;
#endif
*/
	return ex;
}

bool OpFileSystem::IsFolder( const NrpText& pathTo )
{
	NrpText myPath = RemoveEndSlash( pathTo );
	if( !IsExist( myPath ) )
        Log( HW ) << "Try delete unexisting file " << myPath << term;
	
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

void OpFileSystem::Rename( const NrpText& pathOld, const NrpText& pathNew )
{
    assert( IsExist( pathOld ) );

    _wrename( pathOld.ToWide(), pathNew.ToWide() );
}
}//end namespace nrp