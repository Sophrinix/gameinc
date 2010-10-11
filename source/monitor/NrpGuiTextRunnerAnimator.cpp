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
	if( GetTickCount() - time_ > 100 )
	{
		if( _currentPos >= _text.size() )
			return;

		time_ = GetTickCount();
		core::stringw tmpText = _text.subString( 0, _currentPos+1 );
		_currentPos++;
		Parent->setText( tmpText.c_str() );
	}
}

}//namespace gui

}//namespace irr