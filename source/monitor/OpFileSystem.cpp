#include "StdAfx.h"
#include "OpFileSystem.h"
#include <ShellAPI.h>
#include "nrpEngine.h"
#include "IniFile.h"
#include "NrpApplication.h"
#include "nrpScript.h"

#include <io.h>
#include <sys/stat.h>
#include <assert.h>

namespace nrp
{

const NrpText OpFileSystem::anyFile = L"*.*";
const NrpText OpFileSystem::firstEntry = L".";
const NrpText OpFileSystem::secondEntry = L"..";
const NrpText OpFileSystem::numTemplate = L"Number";
int OpFileSystem::lastTimeUpdate = 0;
	
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
			hFile = _wfindfirst( ( mStr + anyFile ).ToWide(), &fdata);
			while( hFile > 0 )
			{
				if ( !( firstEntry == fdata.name || secondEntry == fdata.name ) )// это удалять не надо
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
		}

        if( !RemoveDirectoryW( RemoveEndSlash( mStr ).ToWide() ) )
        {
            int rr = GetLastError();
            rr = rr;
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
		hFile = _wfindfirst( ( CheckEndSlash( pathOld )+ anyFile ).ToWide(), &fdata);
		while( hFile > 0 )
		{
			if ( fdata.attrib & _A_SUBDIR )  // если нашли папку
			{
				if( !( firstEntry == fdata.name || secondEntry == fdata.name ) )
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
											const NrpText& itemName, IniFile* ini, const NrpText& funcUpdate )
{
	_wfinddata_t fdata;	
	intptr_t hFile;

	assert( directory.size() );
	if( directory.size() )
	{
		hFile = _wfindfirst( ( CheckEndSlash( directory )+ anyFile ).ToWide(), &fdata);
		while( hFile )
		{
			if ( !( firstEntry == fdata.name || secondEntry == fdata.name ) )// это удалять не надо
				if ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ))// найдена папка
				{
					CreateDirectorySnapshot( CheckEndSlash( directory ) + NrpText( fdata.name ), templateName, itemName, ini, funcUpdate );
				}
				else// иначе найден файл
				{
					if( _wcsicmp( itemName.ToWide(), fdata.name ) == 0 )
					{
						NrpText fileName = CheckEndSlash( directory )+ fdata.name;
						IniFile rv( fileName );

						int number= ini->Get( SECTION_OPTIONS, templateName + numTemplate, (int)0 );

                        NrpText intName = rv.Get( SECTION_PROPERTIES, INrpConfig::uniqTemplate, NrpText( "" ) );
						ini->Set( SECTION_OPTIONS, CreateKeyName( number ), intName );
						ini->Set( SECTION_OPTIONS, templateName + NrpText( (int)number ), fileName );
						ini->Set( SECTION_OPTIONS, templateName + numTemplate, number+1 );

                        if( funcUpdate.size() && GetTickCount() - lastTimeUpdate > 500 )
                        {
                            lastTimeUpdate = GetTickCount();
                            CNrpScript::Instance().DoString( funcUpdate + "(" + NrpText( number ) + ")" );
                        }
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
    if( IsExist( pathTo ) )
        return;

    for( int i=0; i < 5; i++ )
    {
	    BOOL created = CreateDirectoryW( RemoveEndSlash( pathTo ).ToWide(), NULL );

        assert( created );

        if( !created )
        {
            Log( HW ) << "Can't create directory " << pathTo << term;
            Sleep( 500 );
        }
        else
            return;
    }
}

bool OpFileSystem::IsExist( const NrpText& pathTo )
{
    struct stat stFileInfo; 
    bool blnReturn; 
    int intStat; 

    // Attempt to get the file attributes 
    intStat = stat( const_cast< NrpText& >( RemoveEndSlash( pathTo ) ).ToStr(), &stFileInfo); 
    if(intStat == 0) 
    { 
        // We were able to get the file attributes 
        // so the file obviously exists. 
        blnReturn = true; 
    } 
    else 
    { 
        // We were not able to get the file attributes. 
        // This may mean that we don't have permission to 
        // access the folder which contains this file. If you 
        // need to do that level of checking, lookup the 
        // return values of stat which will give you 
        // more details on why stat failed. 
        blnReturn = false; 
    } 

    return(blnReturn);
}

bool OpFileSystem::IsFolder( const NrpText& pathTo )
{
    bool ret = false;
	NrpText myPath = RemoveEndSlash( pathTo );
	if( !IsExist( myPath ) )
        Log( HW ) << "Try delete unexisting file " << myPath << term;
	
	_wfinddata_t fdata;	
	intptr_t hFile;

	hFile = _wfindfirst( myPath, &fdata);
	if( hFile != -1 )
	{
		ret = ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR ));// это папка
	}

    _findclose( hFile );

	return ret;
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

NrpText OpFileSystem::CheckAbsolutePath( const NrpText& relPath )
{
    NrpText outPath = relPath;
    if( outPath.find( L':' ) > 0 )
        return outPath;

    return CheckEndSlash( _nrpApp[ WORKDIR ] ) + outPath;
}

FolderEntries OpFileSystem::GetEntries( NrpText& pathToDir, bool fileOnly )
{
    _wfinddata_t fdata;	
    intptr_t hFile;

    FolderEntries ret;

    assert( pathToDir.size() );
    if( pathToDir.size() )
    {
        hFile = _wfindfirst( ( CheckEndSlash( pathToDir )+ anyFile ).ToWide(), &fdata);
        while( hFile )
        {
            NrpText absPath = CheckEndSlash( pathToDir )+ fdata.name;
            if ( !( firstEntry == fdata.name || secondEntry == fdata.name ) )// это удалять не надо
                if( !fileOnly && ((( fdata.attrib & _A_SUBDIR ) == _A_SUBDIR ) || ( fdata.attrib == _A_SUBDIR )) )// найдена папка
                    ret.push_back( FolderEntry( true, fdata.name, absPath ) );
                else// иначе найден файл
                    ret.push_back( FolderEntry( false, fdata.name, absPath ) );
 
                if( _wfindnext( hFile, &fdata) != 0 )
                    break;
        }
    }

    _findclose( hFile );

    return ret;
}
}//end namespace nrp