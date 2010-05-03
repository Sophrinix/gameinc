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

}//end namespace nrp