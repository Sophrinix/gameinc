#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpTranslate.h"
#include "IUser.h"
//#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "NrpConfigLooder.h"

namespace nrp
{

void CheckClassesType( const std::string type1, const std::string type2 )
{
	if( type1 != type2 ) 
		Log(HW) << "warning: request type " << type2 << " but native typename is " << type1 << term;
}

void INrpConfig::Load( const std::string& fileName )
{
	CNrpConfigLooder p = CNrpConfigLooder( this );
	p.Load( fileName );
}

std::string INrpConfig::Save( const std::string& fileName )
{
	CNrpConfigLooder p = CNrpConfigLooder( this );
	p.Save( fileName );
	return fileName;
}

void INrpConfig::EraseValue( std::string name )
{
	std::transform( name.begin(), name.end(), name.begin(), tolower );
	PropertyArray::iterator pIter = options_.find( name );

	if( pIter == options_.end() )
	{
#ifdef _DEBUG
		Log(HW) << "erase: bad config param " << name << term;
#endif
		throw "error"; 
	}
	else 
	{
		delete pIter->second;
		options_.erase( pIter );
	}
}

}//end namespace nrp