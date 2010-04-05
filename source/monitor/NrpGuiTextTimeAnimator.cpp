#include "StdAfx.h"
#include "NrpGuiTextTimeAnimator.h"

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
	SYSTEMTIME locTime;

	GetLocalTime( &locTime );
	if( GetTickCount() - time_ > 1000 )
	{
		time_ = GetTickCount();
		wchar_t text[ 32 ];

		swprintf( text, 32, L"%02d:%02d:%02d", locTime.wHour, locTime.wMinute, locTime.wSecond );

		Parent->setText( text );
	}
}

}//namespace gui

}//namespace irr