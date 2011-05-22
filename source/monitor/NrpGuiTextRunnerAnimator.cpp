#include "StdAfx.h"
#include "NrpGuiTextRunnerAnimator.h"

namespace irr
{

namespace gui
{

CNrpGuiTextRunnerAnimator::~CNrpGuiTextRunnerAnimator(void)
{
}

CNrpGuiTextRunnerAnimator::CNrpGuiTextRunnerAnimator( IGUIEnvironment* environment, IGUIElement* node, 
													  const wchar_t* text )
	: IGUIAnimator( environment, node ) 
{
	time_ = GetTickCount();

	_text = wcslen( text ) == 0 ? node->getText() : text;

	_currentPos = 0;
}

void CNrpGuiTextRunnerAnimator::setText( const wchar_t* txt )
{
	_currentPos = 0;
	_text = txt;
}

void CNrpGuiTextRunnerAnimator::draw()
{
	int waitTime = Environment->getVideoDriver()->getFPS() * 2 / _text.size();

	if( GetTickCount() - time_ > waitTime )
	{
		if( _currentPos >= _text.size() )
			return;

		time_ = GetTickCount();
		NrpText tmpText = _text.subString( 0, _currentPos+1 );
		_currentPos++;
		Parent->setText( tmpText.ToWide() );
	}
}

}//namespace gui

}//namespace irr