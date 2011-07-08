#pragma once
#include "NrpText.h"

namespace nrp
{

class IniFile;
class FolderEntry
{
public:
    bool isFolder;
    NrpText name;
    NrpText absPath;

    FolderEntry( bool f, const NrpText& n, const NrpText& p )
    {
        isFolder = f;
        name = n;
        absPath = p;
    }
};

typedef core::array< FolderEntry > FolderEntries;

class OpFileSystem
{
	OpFileSystem(void);
	~OpFileSystem(void);
public:


    static const NrpText anyFile;
    static const NrpText firstEntry;
    static const NrpText secondEntry;
    static const NrpText numTemplate;
    static int lastTimeUpdate;

	static void Remove( const NrpText& pathTo );
	static void Copy( const NrpText& pathOld, const NrpText& pathNew );
    static void Rename( const NrpText& pathOld, const NrpText& pathNew );
	static void CreateDirectorySnapshot( const NrpText& directory,
										 const NrpText& templateName,
										 const NrpText& itemName,
										 IniFile* ini, const NrpText& funcUpdate );
	static NrpText CheckEndSlash( NrpText pathTo );

	static void CreateDirectory( NrpText pathTo );
	static bool IsExist( const NrpText& pathTo );
	static bool IsFolder( const NrpText& pathTo );
	static NrpText GetExtension( const NrpText& pathTo );
	static NrpText UpDir( const NrpText& pathTo );
	static NrpText RemoveEndSlash( NrpText pathTo );
    static FolderEntries GetEntries( NrpText& pathToDir, bool fileOnly=false );
	static NrpText CheckFile( const NrpText& dir, const NrpText& fileName );
    static NrpText CheckAbsolutePath( const NrpText& relPath );
};

}//end namespace nrp