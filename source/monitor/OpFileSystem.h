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
	static void MoveDirectory( HWND window, const std::string& pathOld, const std::string& pathNew );
};

}//end namespace nrp