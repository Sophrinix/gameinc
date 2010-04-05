#ifndef NERPA_VERSION_H
#define NERPA_VERSION_H
#include <string>
namespace nrp
{

class nrpVersion
{
public:
	nrpVersion();
	~nrpVersion();
	std::wstring ProductName();
	std::wstring ProductVersion();
private:
	char* versionInfoBuf_;
	std::wstring productName_;
	std::wstring productVersion_;
};

} //namespace nrp
#endif