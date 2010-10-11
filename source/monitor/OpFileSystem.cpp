#include "StdAfx.h"
#include "OpFileSystem.h"
#include <ShellAPI.h>
#include "nrpEngine.h"

namespace nrp
{
	
OpFileSystem::OpFileSystem(void)
{
}

OpFileSystem::~OpFileSystem(void)
{
}

void OpFileSystem::RemoveDirectory( const std::string& pathTo )
{
	std::string mStr = pathTo;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	SHFILEOPSTRUCT sh;
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_DELETE;
	sh.pFrom  = mStr.c_str();
	sh.pTo    = NULL;
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::MoveDirectory( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = pathOld, mStr2 = pathNew;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	if( mStr2[ mStr2.length() -1 ] == '\\' || mStr2[ mStr2.length() -1 ] == '/' )
		mStr2 = mStr2.substr( 0, mStr2.length() - 1 );

	SHFILEOPSTRUCT sh;
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_MOVE;
	sh.pFrom  = mStr.c_str();
	sh.pTo    = mStr2.c_str();
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;

	SHFileOperation (&sh);
}

void OpFileSystem::CopyFile( const std::string& pathOld, const std::string& pathNew )
{
	std::string mStr = pathOld, mStr2 = pathNew;
	if( mStr[ mStr.length() -1 ] == '\\' || mStr[ mStr.length() -1 ] == '/' )
		mStr = mStr.substr( 0, mStr.length() - 1 );

	if( mStr2[ mStr2.length() -1 ] == '\\' || mStr2[ mStr2.length() -1 ] == '/' )
		mStr2 = mStr2.substr( 0, mStr2.length() - 1 );

	SHFILEOPSTRUCT sh;
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //Для BCB sh.hwnd=FormX->Handle;
	sh.wFunc  = FO_COPY;
	sh.pFrom  = mStr.c_str();
	sh.pTo    = mStr2.c_str();
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	sh.hNameMappings = 0;
	sh.lpszProgressTitle = NULL;
}

void OpFileSystem::CreateDirectorySnapshot( const std::string& directory,
											const std::string& saveFile, 
											const std::string& templateName,
											const std::string& itemName )
{
	assert( directory.size() );
	if( directory.size() )
	{
		
		if( ! FindFirst( directory+"\\*.*",faAnyFile,sr) )
			do
			{
				if (!(sr.Name=="." || sr.Name==".."))// это удалять не надо
					if (((sr.Attr & faDirectory) == faDirectory ) ||
						(sr.Attr == faDirectory))// найдена папка
					{
						FileSetAttr(DirName+"\\"+sr.Name, faDirectory );// сброс всяких read-only
						DeleteDir(DirName+"\\"+sr.Name);//рекурсивно удаляем содержимое
						RemoveDir(DirName + "\\"+sr.Name);// удаляем теперь уже пустую папку
					}
					else// иначе найден файл
					{
						FileSetAttr(DirName+"\\"+sr.Name, 0);// сброс всяких read-only
						DeleteFile(DirName+"\\"+sr.Name);// удаляем файл
					}
			}
			while (!FindNext(sr));// ищем опять, пока не найдем все
			FindClose(sr);
	}
	RemoveDir(DirName);
	return true;
}

}//end namespace nrp