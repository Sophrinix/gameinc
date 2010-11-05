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
bool DownloadFiles ( HINTERNET hFtp, const std::string& rootDir ) 
{
	//printf( "Resolve directory %s\n", rootDir.c_str() );

	char buf[1024] = {0};
	std::vector< std::string > dirs;
	DWORD c = 1024;
	WIN32_FIND_DATAA fd = { 0 }, ft = { 0 };

	if ( !FtpSetCurrentDirectoryA( hFtp, rootDir.c_str() ) )
		printf( ("Directory " + rootDir + " are not avaible. Goodbay\n").c_str() );

	HINTERNET hSearch = FtpFindFirstFileA( hFtp, NULL, &fd, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD , 0 );
		
	if ( hSearch ) 
	{
		do 
		{
			if( strcmp( fd.cFileName, "." ) == 0 || strcmp( fd.cFileName, ".." ) == 0 )
				continue;

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				dirs.push_back( fd.cFileName );
			}
			
			else 
			{
				// Сравниваем дату найденного файла с тем что сейчас в списке
				if ( _access( fd.cFileName, 0 ) == -1 ) 
				{
					printf( "Loading file %s\n", fd.cFileName );
					if( !FtpGetFileA( hFtp, fd.cFileName, fd.cFileName, FALSE, 0, 0, 0 ) )
							  return false;
				}
				else
				{
					HANDLE ftFile = FindFirstFileA( fd.cFileName, &ft );
					if ( CompareFileTime ( &ft.ftLastWriteTime, &fd.ftLastWriteTime ) < 0 ) 
					{			
						printf( "Loading file %s\n", fd.cFileName );
						if ( !FtpGetFileA( hFtp, fd.cFileName, fd.cFileName, FALSE, 0, 0, 0 ) )
							return false;
					}
				}
			}
		} while ( InternetFindNextFile ( hSearch, &fd ) );
		 
		InternetCloseHandle ( hSearch );
	}

	for( size_t k=0; k < dirs.size(); k++ )
	{
		CreateDirectoryA( dirs[ k ].c_str(), NULL );
		SetCurrentDirectoryA( dirs[ k ].c_str() );

		DownloadFiles( hFtp, dirs[ k ] );
	}
	
	FtpSetCurrentDirectoryA( hFtp, ".." );
	SetCurrentDirectoryA( ".." );
	return true;
}  // DownloadFiles


// ===============================================================
// Функция загрузки обновления
void FtpDownload ( const std::string& serverName, const std::string& pathTo ) {

	// Соединение с сервером
	printf ( "Connecting to server %s...\n", ("ftp://" + serverName + "/" + pathTo).c_str() );
	HINTERNET hInet = InternetOpenA( "GameInc updater agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	HINTERNET hFtp = InternetConnectA( hInet, serverName.c_str(), INTERNET_DEFAULT_FTP_PORT, "anonymous", "anonymous@mail.com", INTERNET_SERVICE_FTP, 0, 0 );
	if ( !hInet || !hFtp )
		printf( "Server are not availble. Goodbay\n" );
	else
		if ( !DownloadFiles ( hFtp, pathTo ) )
			printf( "Some errors while update\n" );

	// Освобождение ресурсов
	InternetCloseHandle( hFtp );
	InternetCloseHandle( hInet );
} // ThreadProc


int _tmain(int argc, _TCHAR* argv[])
{
	FtpDownload( "10.10.72.57", "game" );

	printf( "Update finished.Press Enter");
	_getch();
	return 0;
}

