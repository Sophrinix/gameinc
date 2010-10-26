#pragma once
#include <string>

namespace nrp
{

class OpFileSystem
{
	OpFileSystem(void);
	~OpFileSystem(void);
public:
	static void Remove( const std::string& pathTo );
	static void Copy( const std::string& pathOld, const std::string& pathNew );
	static void Move( const std::string& pathOld, const std::string& pathNew );
	static void CreateDirectorySnapshot( const std::string& directory,
										 const std::string& saveFile, 
										 const std::string& templateName,
										 const std::string& itemName );
	static std::string CheckEndSlash( std::string pathTo );

	static void CreateDirectory( std::string pathTo );
	static bool IsExist( const std::string& pathTo );
	static bool IsFolder( const std::string& pathTo );
	static std::string GetExtension( const std::string& pathTo );
	static std::string UpDir( const std::string& pathTo );
	static std::string RemoveEndSlash( std::string pathTo );
};

}//end namespace nrp