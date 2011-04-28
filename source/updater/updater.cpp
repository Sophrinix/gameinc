// updater.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "afxinet.h"

#include <string>
#include <vector>
#include <io.h>
#include <conio.h>

// ===============================================================
// Скачивает файлы с FTP
bool DownloadFiles ( HINTERNET hFtp, const std::wstring& rootDir ) 
{
	//printf( "Resolve directory %s\n", rootDir.c_str() );

	char buf[1024] = {0};
	std::vector< std::wstring > dirs;
	DWORD c = 1024;
	WIN32_FIND_DATAW fd = { 0 }, ft = { 0 };

	if ( !FtpSetCurrentDirectoryW( hFtp, rootDir.c_str() ) )
		wprintf( ( L"Directory " + rootDir + L" are not avaible. Goodbay\n").c_str() );

	HINTERNET hSearch = FtpFindFirstFileW( hFtp, NULL, &fd, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD , 0 );
		
	if ( hSearch ) 
	{
		do 
		{
			if( wcscmp( fd.cFileName, L"." ) == 0 || wcscmp( fd.cFileName, L".." ) == 0 )
				continue;

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				dirs.push_back( fd.cFileName );
			}
			
			else 
			{
				// Сравниваем дату найденного файла с тем что сейчас в списке
				if ( _waccess( fd.cFileName, 0 ) == -1 ) 
				{
					wprintf( L"Loading file %s\n", fd.cFileName );
					if( !FtpGetFileW( hFtp, fd.cFileName, fd.cFileName, FALSE, 0, 0, 0 ) )
							  return false;
				}
				else
				{
					HANDLE ftFile = FindFirstFileW( fd.cFileName, &ft );
					if ( CompareFileTime ( &ft.ftLastWriteTime, &fd.ftLastWriteTime ) < 0 ) 
					{			
						wprintf( L"Loading file %s\n", fd.cFileName );
						if ( !FtpGetFileW( hFtp, fd.cFileName, fd.cFileName, FALSE, 0, 0, 0 ) )
							return false;
					}
				}
			}
		} while ( InternetFindNextFile ( hSearch, &fd ) );
		 
		InternetCloseHandle ( hSearch );
	}

	for( size_t k=0; k < dirs.size(); k++ )
	{
		CreateDirectoryW( dirs[ k ].c_str(), NULL );
		SetCurrentDirectoryW( dirs[ k ].c_str() );

		DownloadFiles( hFtp, dirs[ k ] );
	}
	
	FtpSetCurrentDirectoryW( hFtp, L".." );
	SetCurrentDirectoryW( L".." );
	return true;
}  // DownloadFiles


// ===============================================================
// Функция загрузки обновления
void FtpDownload ( const std::wstring& serverName, const std::wstring& pathTo ) {

	// Соединение с сервером
	wprintf ( L"Connecting to server %s...\n", (L"ftp://" + serverName + L"/" + pathTo).c_str() );
	HINTERNET hInet = InternetOpenW( L"GameInc updater agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	HINTERNET hFtp = InternetConnectW( hInet, serverName.c_str(), INTERNET_DEFAULT_FTP_PORT, L"anonymous", L"anonymous@mail.com", INTERNET_SERVICE_FTP, 0, 0 );
	if ( !hInet || !hFtp )
		wprintf( L"Server are not availble. Goodbay\n" );
	else
		if ( !DownloadFiles ( hFtp, pathTo ) )
			wprintf( L"Some errors while update\n" );

	// Освобождение ресурсов
	InternetCloseHandle( hFtp );
	InternetCloseHandle( hInet );
} // ThreadProc


int _tmain(int argc, _TCHAR* argv[])
{
	FtpDownload( L"10.10.72.57", L"game" );

	printf( "Update finished.Press Enter");
	_getch();
	return 0;
}

