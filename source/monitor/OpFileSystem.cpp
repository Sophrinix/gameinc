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
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //��� BCB sh.hwnd=FormX->Handle;
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
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //��� BCB sh.hwnd=FormX->Handle;
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
	sh.hwnd   = CNrpEngine::Instance().GetWindowHandle(); //��� BCB sh.hwnd=FormX->Handle;
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
				if (!(sr.Name=="." || sr.Name==".."))// ��� ������� �� ����
					if (((sr.Attr & faDirectory) == faDirectory ) ||
						(sr.Attr == faDirectory))// ������� �����
					{
						FileSetAttr(DirName+"\\"+sr.Name, faDirectory );// ����� ������ read-only
						DeleteDir(DirName+"\\"+sr.Name);//���������� ������� ����������
						RemoveDir(DirName + "\\"+sr.Name);// ������� ������ ��� ������ �����
					}
					else// ����� ������ ����
					{
						FileSetAttr(DirName+"\\"+sr.Name, 0);// ����� ������ read-only
						DeleteFile(DirName+"\\"+sr.Name);// ������� ����
					}
			}
			while (!FindNext(sr));// ���� �����, ���� �� ������ ���
			FindClose(sr);
	}
	RemoveDir(DirName);
	return true;
}

}//end namespace nrp