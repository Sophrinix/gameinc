#include "StdAfx.h"
#include "irrlayout.h"
#include <assert.h>

namespace irr
{

namespace gui
{

void CNrpLayout::addChildToEnd( IGUIElement* elm )
{
	if( elm )
		IGUIElement::addChildToEnd( elm );

	core::list< IGUIElement* > pChildrens( Children );
	int numRow = ceil( Children.size() / (float)_btnInLine );
	
	if( numRow == 0 )
		numRow = 1;
	
	int lastY = 0;
	core::dimension2du side( 0, ( AbsoluteRect.getHeight() - _margin / 2 * numRow ) / numRow );
	while( pChildrens.size() > 0 )
	{
		core::array< IGUIElement* > tmp;
		u32 maxAdd = min( _btnInLine, pChildrens.size() );
		for( u32 i=0; i < maxAdd; i++ )
		{
			tmp.push_back( *pChildrens.begin() );
			pChildrens.erase( pChildrens.begin() );
		}

		side.Width = (AbsoluteRect.getWidth() - _margin / 2 * tmp.size() ) / tmp.size(); 

		core::dimension2du finalSize = side;

		if( _size.Width > 0 )
			finalSize.Width = min( side.Width, _size.Width );
		if( _size.Height > 0 )
			finalSize.Height = min( side.Height, _size.Height );

		for( u32 i=0; i < tmp.size(); i++ )
		{
			core::recti rectangle( core::position2di( side.Width * i, 0 ), finalSize );
			rectangle += core::position2di( _margin / 2, lastY );
			tmp[ i ]->setRelativePosition( rectangle );
		}

		lastY += side.Height + _margin / 2;
	}
}

CNrpLayout::CNrpLayout( IGUIEnvironment* guienv, IGUIElement* parent, core::recti rectangle, u32 buttonInLine/*=4*/, s32 id/*=-1 */ )
: IGUIWindow( guienv, parent, -1, rectangle )
{
	assert( buttonInLine > 0 );
	_margin = 4;
	_btnInLine = buttonInLine;

	AlignRight = EGUIA_LOWERRIGHT;
	AlignBottom = EGUIA_LOWERRIGHT;
	_size = core::dimension2du( 0, 0 );
	//updateAbsolutePosition();
}

/*void CNrpLayout::setVisible( bool visible )
{
	core::list< IGUIElement* >::Iterator pIter = Children.begin();
	for( ; pIter != Children.end(); pIter++ )
		(*pIter)->setVisible( visible );

	IGUIWindow::setVisible( visible );
}*/

void CNrpLayout::draw()
{
	if( !IsVisible )
		return;

	IGUIWindow::draw();
}

bool CNrpLayout::OnEvent( const SEvent& event )
{
	return Parent ? Parent->OnEvent(event) : false;
}

CNrpLayout::~CNrpLayout()
{

}

bool CNrpLayout::isPointInside( const core::position2d<s32>& point ) const
{
	return AbsoluteRect.isPointInside( point );
}

void CNrpLayout::updateAbsolutePosition()
{
	IGUIWindow::updateAbsolutePosition();

	addChildToEnd( NULL );
}

void CNrpLayout::setRelativePosition(const core::rect<s32>& r)
{
	IGUIWindow::setRelativePosition( r );

	addChildToEnd( NULL );
}

}//end namesoace gui

}//end namespace irr