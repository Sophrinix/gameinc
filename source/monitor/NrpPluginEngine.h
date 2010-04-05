#pragma once

#include <vector>
#include <string>
#include <map>

namespace nrp
{

namespace plugin
{

class CNrpPluginEngine
{
public:
	static CNrpPluginEngine& Instance();

	std::vector< std::string > FindLibraries( std::string pluginFolder );
	void RegisterLibraries( std::vector< std::string >& paths );
	HMODULE GetLibrary( std::string name );	

private:
	CNrpPluginEngine(void);
	~CNrpPluginEngine(void);

	void RegisterLibrary_( std::string pathToDLL );

	std::map< std::string, HMODULE > plugins_;
};

}//namespace plugin

}//namespace nrp
