#include "StdAfx.h"
#include "NrpGameBox.h"
#include "NrpGame.h"

namespace nrp
{

CNrpGameBox::~CNrpGameBox(void)
{
}

nrp::CNrpGameBox::CNrpGameBox( CNrpGame* ptrGame ) : INrpConfig( "CNrpGameBox", "" )
{
	CreateValue<std::string>( NAME, ptrGame->GetValue<std::string>( NAME ) );
	CreateValue<PNrpGame>( BOXGAME, ptrGame );
}

}//end of namespace nrp