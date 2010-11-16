#include "stdafx.h"
#include "nrpConfig.h"
#include "NrpTranslate.h"
#include "NrpConfigLooder.h"

namespace nrp
{

void CheckClassesType( const NrpText& type1, const NrpText& type2 )
{
	if( type1 != type2 ) 
		Log(HW) << L"warning: request type " << type2 << L" but native typename is " << type1 << term;
}

void INrpConfig::Load( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Load( fileName );
}

NrpText INrpConfig::Save( const NrpText& fileName )
{
	CNrpConfigLooder p( this );
	p.Save( fileName );
	return fileName;
}

void INrpConfig::EraseValue( NrpText name )
{
	PropertyArray::Iterator pIter = _options.find( name.ToLower() );

	if( pIter.atEnd() )
	{
#ifdef _DEBUG
		Log(HW) << "erase: bad config param " << name << term;
#endif
		throw "error"; 
	}
	else 
	{
		delete pIter->getValue();
		_options.remove( name );
	}
}

}//end namespace nrp