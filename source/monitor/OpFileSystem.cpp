#include "StdAfx.h"
#include "OpFileSystem.h"
#include <ShellAPI.h>

namespace nrp
{
	
OpFileSystem::OpFileSystem(void)
{
}

OpFileSystem::~OpFileSystem(void)
{
}

void OpFileSystem::RemoveDirectory( HWND window, const std::string& pathTo )
{
	std::string mStr = pathTo;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	SHFILEOPSTRUCT sh;
	sh.hwnd   = window; //Äëÿ BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_DELETE;
	sh.pFrom  = mStr.c_str();
	sh.pTo    = NULL;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::MoveDirectory( HWND window, const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = pathOld, mStr2 = pathNew;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	if( mStr2[ mStr2.length() -1 ] == '\\' || mStr2[ mStr2.length() -1 ] == '/' )
		mStr2 = mStr2.substr( 0, mStr2.length() - 1 );

	SHFILEOPSTRUCT sh;
	sh.hwnd   = window; //Äëÿ BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_MOVE;
	sh.pFrom  = mStr.c_str();
	sh.pTo    = mStr2.c_str();
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}
}//end namespace nrp