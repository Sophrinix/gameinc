#pragma once
#include <string>

namespace nrp
{

class OpFileSystem
{
	OpFileSystem(void);
	~OpFileSystem(void);
public:
	static void RemoveDirectory( HWND window, const std::string& pathTo );
};

}//end namespace nrp