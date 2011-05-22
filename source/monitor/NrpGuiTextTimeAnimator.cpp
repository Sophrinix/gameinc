#include "StdAfx.h"
#include "NrpGuiTextTimeAnimator.h"
#include "NrpTime.h"

using namespace nrp;

namespace irr
{

namespace gui
{

CNrpGuiTextTimeAnimator::~CNrpGuiTextTimeAnimator(void)
{
}

CNrpGuiTextTimeAnimator::CNrpGuiTextTimeAnimator( IGUIEnvironment* environment, IGUIElement* node )
	: IGUIAnimator( environment, node )
{
	time_ = GetTickCount();
}

void CNrpGuiTextTimeAnimator::draw()
{
	if( GetTickCount() - time_ > 1000 )
	{
		time_ = GetTickCount();
		wchar_t text[ 32 ];

		NrpTime tmp = NrpTime::FromLocalTime();
		swprintf( text, 32, L"%02d:%02d:%02d", tmp.RHour(), tmp.RMinute(), tmp.RSecond() );

		Parent->setText( text );
	}
}

}//namespace gui

}//namespace irr