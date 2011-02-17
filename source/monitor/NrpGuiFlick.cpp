#include "stdafx.h"
#include "NrpGuiFlick.h"
#include "nrpGUIEnvironment.h"
#include "nrpButton.h"
#include <irrMath.h>

namespace irr
{

using namespace video;

namespace gui
{

// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//! constructor
CNrpGuiFlick::CNrpGuiFlick( IGUIEnvironment* guienv, IGUIElement* parent, core::recti rectangle, u32 column, s32 id )
								  : IGUIWindow( guienv, parent, id, rectangle ), _column( column ) 
{
	core::recti btnRect( 0, 0, 1, 1 );

	_btnUp = new CNrpButton( guienv, this, -1, btnRect );
	_btnUp->setText( L"up" );

	_btnDown = new CNrpButton( guienv, this, -1, btnRect );
	_btnDown->setText( L"down" );

	_starty = 0;

	updateAbsolutePosition();
}

//! destructor
CNrpGuiFlick::~CNrpGuiFlick()
{
}

IGUIElement* CNrpGuiFlick::GetFirst()
{
	core::list<IGUIElement*>::Iterator it = Children.begin();
	for (; it != Children.end(); ++it )
	{
		if( (*it) == _btnUp || (*it) == _btnDown )
			continue;

		break;
	}

	return it == Children.end() ? NULL : *it;
}

IGUIElement* CNrpGuiFlick::GetLast()
{
	core::list<IGUIElement*>::Iterator it = Children.getLast();
	for (; it != Children.end(); --it )
	{
		if( (*it) == _btnUp || (*it) == _btnDown )
			continue;

		break;
	}

	return it == Children.end() ? NULL : *it;
}

//! draws the element and its children
void CNrpGuiFlick::draw()
{
	if (!IsVisible)
		return;

	if( Children.size() > 0 )
	{
		if( IGUIElement* elm = GetFirst() )
			_btnUp->setVisible( elm->getRelativePosition().UpperLeftCorner.Y < 0 );

		if( IGUIElement* elm = GetLast() )
			_btnDown->setVisible( elm->getRelativePosition().LowerRightCorner.Y > AbsoluteRect.getHeight() );
	}

	IGUIElement::draw();
}

void CNrpGuiFlick::addChildToEnd( IGUIElement* elm )
{
	if( elm )
		IGUIElement::addChildToEnd( elm );

	if( Children.size() < 3 )
		return;

	core::dimension2di side;

	s32 sideNum = core::ceil32( core::squareroot( static_cast< float >(Children.size() - 2) ) );
	sideNum = (sideNum > _column ? _column : sideNum);
	side.Width = AbsoluteRect.getWidth() / sideNum;
	side.Height = AbsoluteRect.getHeight() / sideNum;

	core::list<IGUIElement*>::Iterator it = Children.begin();
	for ( s32 i=0; it != Children.end(); ++it, ++i )
	{
		if( (*it) == _btnUp || (*it) == _btnDown )
		{	i--; continue; }

		s32 row = i / sideNum;
		s32 col = i % sideNum;

		core::position2di pos( (col + 0.5f )* side.Width, (row + 0.5f) * side.Height );
		pos -= core::position2di( side.Width * 0.4f, side.Height * 0.4f - _starty );
		(*it)->setRelativePosition( pos );
	}

		
	core::recti btnRect = core::recti( AbsoluteRect.getWidth() / 2 - 40, 20, AbsoluteRect.getWidth() / 2 + 40, 60 );
	_btnUp->setRelativePosition( btnRect );
	bringToFront( _btnUp );

	btnRect.UpperLeftCorner.Y = AbsoluteRect.getHeight() - 60;
	btnRect.LowerRightCorner.Y = AbsoluteRect.getHeight() - 20;
	_btnDown->setRelativePosition( btnRect );
	bringToFront( _btnDown );
}

bool CNrpGuiFlick::OnEvent( const irr::SEvent& event )
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if( event.GUIEvent.Caller == _btnDown || event.GUIEvent.Caller == _btnUp )
			return true;

	case EET_KEY_INPUT_EVENT:
		if( event.KeyInput.Key == KEY_DOWN && event.KeyInput.PressedDown == false )
		{		
			_starty -= 50;
			addChildToEnd( NULL );
			return true;
		}

		if( event.KeyInput.Key == KEY_UP && event.KeyInput.PressedDown == false )
		{		
			_starty += 50;
			addChildToEnd( NULL );
			return true;
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}

void CNrpGuiFlick::Clear()
{
	core::list<IGUIElement*>::Iterator it = Children.begin();
	for ( ; it != Children.end(); ++it )
	{
		if( (*it) == _btnUp || (*it) == _btnDown ) continue;

		if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment ) )
			env->addToDeletionQueue( *it );
	}

	Children.clear();
	Children.push_back( _btnDown );
	Children.push_back( _btnUp );
}

}//namespace gui

}//namespace irr