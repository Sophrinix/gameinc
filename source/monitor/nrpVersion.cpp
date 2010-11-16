#include "stdafx.h"
#include <windows.h>
#include <sstream>
//:
#include "nrpVersion.h"

namespace nrp
{

nrpVersion::nrpVersion() : _versionInfoBuf(0)
{
	DWORD someHandle = 0;
	if (size_t sz = GetFileVersionInfoSize( __argv[0], &someHandle))
	{
		_versionInfoBuf = new char[ sz ];
		if (!GetFileVersionInfo(__argv[0], someHandle, sz, _versionInfoBuf))
		{
			//ErrLog(gfx) << "Не удалось загрузить информацию о версии ПО" << term;
			delete [] _versionInfoBuf;
			_versionInfoBuf = 0;
		}
	}
}

nrpVersion::~nrpVersion()
{
	delete [] _versionInfoBuf;
}

const NrpText& nrpVersion::ProductVersion()
{	
	_productVersion = L"";
	if (!_versionInfoBuf)
		return _productVersion;

	VS_FIXEDFILEINFO *fileInfo = 0;
	size_t length = 0;
	if (VerQueryValue(_versionInfoBuf, "\\", reinterpret_cast<LPVOID*>(&fileInfo), &length))
	{
		std::wstringstream stream;
		stream << HIBYTE(fileInfo->dwFileVersionMS) << "." << LOBYTE(fileInfo->dwFileVersionMS) 
			<< "." << HIBYTE(fileInfo->dwProductVersionLS) << "." << LOBYTE(fileInfo->dwProductVersionLS);

		_productVersion = stream.str().c_str();
	}
	
	return _productVersion;
}
} //namespace nrp