#pragma once

#include <irrArray.h>
#include <irrMap.h>

namespace nrp
{

namespace plugin
{

class CNrpPluginEngine
{
public:
	static CNrpPluginEngine& Instance();

	core::array< NrpText > FindLibraries( const NrpText& pluginFolder );
	void RegisterLibraries( core::array< NrpText >& paths );
	HMODULE GetLibrary( const NrpText& name );	

private:
	CNrpPluginEngine(void);
	~CNrpPluginEngine(void);

	void RegisterLibrary_( const NrpText& pathToDLL );

	core::map< NrpText, HMODULE > plugins_;
};

}//namespace plugin

}//namespace nrp
