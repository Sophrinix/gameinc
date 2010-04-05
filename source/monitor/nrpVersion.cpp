#include "stdafx.h"
#include <windows.h>
#include <sstream>
//:
#include "nrpVersion.h"

namespace nrp
{

nrpVersion::nrpVersion() : versionInfoBuf_(0)
{
	DWORD someHandle = 0;
	if (size_t sz = GetFileVersionInfoSize(__argv[0], &someHandle))
	{
		versionInfoBuf_ = new char[ sz ];
		if (!GetFileVersionInfo(__argv[0], someHandle, sz, versionInfoBuf_))
		{
			//ErrLog(gfx) << "Не удалось загрузить информацию о версии ПО" << term;
			delete [] versionInfoBuf_;
			versionInfoBuf_ = 0;
		}
	}
}

nrpVersion::~nrpVersion()
{
	delete [] versionInfoBuf_;
}

std::wstring nrpVersion::ProductName()
{
	if (!versionInfoBuf_)
		return std::wstring(L"");

	VS_FIXEDFILEINFO *fileInfo = 0;
	size_t length = 0;
	if (VerQueryValue(versionInfoBuf_, "\\", reinterpret_cast<LPVOID*>(&fileInfo), &length))
	{
		std::wstringstream stream;
		stream << HIBYTE(fileInfo->dwFileVersionMS) << "." << LOBYTE(fileInfo->dwFileVersionMS) 
			<< "." << HIBYTE(fileInfo->dwProductVersionLS) << "." << LOBYTE(fileInfo->dwProductVersionLS);
		return stream.str();
	}
	else
		return std::wstring(L"");
}

std::wstring nrpVersion::ProductVersion()
{
	if (!versionInfoBuf_)
		return std::wstring(L"");

	VS_FIXEDFILEINFO *fileInfo = 0;
	size_t length = 0;
	if (VerQueryValue(versionInfoBuf_, "\\", reinterpret_cast<LPVOID*>(&fileInfo), &length))
	{
		std::wstringstream stream;
		stream << HIBYTE(fileInfo->dwFileVersionMS) << "." << LOBYTE(fileInfo->dwFileVersionMS) 
			<< "." << HIBYTE(fileInfo->dwProductVersionLS) << "." << LOBYTE(fileInfo->dwProductVersionLS);
		return stream.str();
	}
	else
		return std::wstring(L"");
}

} //namespace nrp