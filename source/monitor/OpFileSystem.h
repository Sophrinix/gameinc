#pragma once
#include <string>

namespace nrp
{

class OpFileSystem
{
	OpFileSystem(void);
	~OpFileSystem(void);
public:
	static void RemoveDirectory( const std::string& pathTo );
	static void CopyFile( const std::string& pathOld, const std::string& pathNew );
	static void MoveDirectory( const std::string& pathOld, const std::string& pathNew );
};

}//end namespace nrp