#ifndef NERPA_VERSION_H
#define NERPA_VERSION_H
#include "NrpText.h"

namespace nrp
{

class nrpVersion
{
public:
	nrpVersion();
	~nrpVersion();

	const NrpText& ProductVersion();
private:
	char* _versionInfoBuf;
	NrpText _productVersion;
};

} //namespace nrp
#endif