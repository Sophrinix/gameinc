#pragma once
#include "NrpText.h"

namespace nrp
{

class IniFile;

class OpFileSystem
{
	OpFileSystem(void);
	~OpFileSystem(void);
public:
	static void Remove( const NrpText& pathTo );
	static void Copy( const NrpText& pathOld, const NrpText& pathNew );
    static void Rename( const NrpText& pathOld, const NrpText& pathNew );
	static void CreateDirectorySnapshot( const NrpText& directory,
										 const NrpText& templateName,
										 const NrpText& itemName,
										 IniFile* ini );
	static NrpText CheckEndSlash( NrpText pathTo );

	static void CreateDirectory( NrpText pathTo );
	static bool IsExist( const NrpText& pathTo );
	static bool IsFolder( const NrpText& pathTo );
	static NrpText GetExtension( const NrpText& pathTo );
	static NrpText UpDir( const NrpText& pathTo );
	static NrpText RemoveEndSlash( NrpText pathTo );
	static NrpText CheckFile( const NrpText& dir, const NrpText& fileName );
};

}//end namespace nrp