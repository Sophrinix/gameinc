#include "stdafx.h"
#include "Nrp2DPictureFlow.h"

namespace irr
{

namespace gui
{

	video::ITexture* CNrp2DPictureFlow::CNrpImageDescription::CreateDownTexture_( video::IVideoDriver* driver, 
																			   video::ITexture* pTxr )
{
	video::ITexture* resultt = NULL;

	if( pTxr != NULL )
	{
		std::string name = core::stringc( pTxr->getName() ).c_str();
		name += "toDown";
		resultt = driver->addTexture( pTxr->getSize(), name.c_str(), pTxr->getColorFormat() );

		u32* pTxrData = (u32*)pTxr->lock();
		u32* resultTxrData = (u32*)resultt->lock();
		for( size_t k=0; k < pTxr->getSize().Height; k++ )
		{
			int offsetPtxr = k * pTxr->getSize().Width;
			int offsetResult = ( resultt->getSize().Height - k - 1 ) * resultt->getSize().Width;
			int pith = pTxr->getPitch();
			memcpy( resultTxrData+offsetResult, pTxrData+offsetPtxr, pTxr->getPitch() );
		}

		pTxr->unlock();
		resultt->unlock();
	}

	return resultt;
}

CNrp2DPictureFlow::CNrp2DPictureFlow( IGUIEnvironment* env, 
								  IGUIElement* parent, 
								  core::recti rectangle,
								  core::recti pictureRect,
								  s32 id )
: IGUIListBox( env, parent, id, rectangle )
{
	_activeIndex = 0;
	_pictureRect = pictureRect;
	_drawBackground = true;
}

u32 CNrp2DPictureFlow::addItem( video::ITexture* texture, const wchar_t* text )
{
	CNrpImageDescription* descr = new CNrpImageDescription();
	_images.push_back( descr );
	descr->SetTexture( Environment->getVideoDriver(), texture ); 
	
	descr->rectangle = core::recti( 0, 0, 0, 0 );

	_UpdateImages();

	return _images.size() - 1;
}

irr::u32 CNrp2DPictureFlow::addItem( const wchar_t* text )
{
	return addItem( NULL, text );
}

irr::u32 CNrp2DPictureFlow::addItem( video::ITexture* texture, const wchar_t* text, void* object )
{
	u32 resultt = addItem( texture, text );
	_images[ resultt ]->object = object;
	return resultt;
}

void CNrp2DPictureFlow::_UpdateImages()
{
	core::recti tmpRect( RelativeRect.getCenter().X - _pictureRect.getWidth()/2, 
						 RelativeRect.getCenter().Y - _pictureRect.getHeight()/2,
						 RelativeRect.getCenter().X + _pictureRect.getWidth()/2, 
						 RelativeRect.getCenter().Y + _pictureRect.getHeight()/2 );

	if( _activeIndex < (int)_images.size() )
		_images[ _activeIndex ]->rectangle = tmpRect;

	s32 offsetx = 0;
	core::recti lRect = tmpRect;
	if( _activeIndex - 1 >= 0 )
		for( int k=max( 0, _activeIndex-1); k >= 0; k-- )
		{
			offsetx += lRect.getWidth() * (0.7f - (_activeIndex-k)*0.1f);
			core::dimension2di sides( 0.7f * lRect.getWidth(), 0.7f * lRect.getHeight() ); 
			lRect = core::recti( RelativeRect.getCenter().X - sides.Width/2, RelativeRect.getCenter().Y - sides.Height/2,
				                 RelativeRect.getCenter().X + sides.Width/2, RelativeRect.getCenter().Y + sides.Height/2 );
			_images[ k ]->rectangle = lRect - core::position2di( offsetx, 0 );
		}

	offsetx = 0;
	core::recti rRect = tmpRect;
	for( int k=min(_activeIndex+1, _images.size()); k < _images.size(); k++ )
	{
		offsetx += rRect.getWidth() * (0.7f - (k-_activeIndex)*0.1f);
		core::dimension2di sides( 0.7f * rRect.getWidth(), 0.7f * rRect.getHeight() ); 
		rRect = core::recti( RelativeRect.getCenter().X - sides.Width/2, RelativeRect.getCenter().Y - sides.Height/2,
							 RelativeRect.getCenter().X + sides.Width/2, RelativeRect.getCenter().Y + sides.Height/2 );
		_images[ k ]->rectangle = rRect + core::position2di( offsetx, 0 );
	}
}

void CNrp2DPictureFlow::_DrawPairImage( CNrpImageDescription* pDesk )
{
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::recti rectangle( pDesk->currentRect.UpperLeftCorner.X, pDesk->currentRect.UpperLeftCorner.Y,
						   pDesk->currentRect.LowerRightCorner.X, pDesk->currentRect.LowerRightCorner.Y );

	if( pDesk->GetTexture() )
		driver->draw2DImage( pDesk->GetTexture(), rectangle + AbsoluteRect.UpperLeftCorner, 
			 			 	 core::recti( core::position2di( 0, 0), pDesk->GetTexture()->getSize() ),
							 &AbsoluteClippingRect, NULL, true );
	else
		driver->draw2DRectangle(rectangle + AbsoluteRect.UpperLeftCorner, 
								0xC0C0C0C0, 0xC0C0C0C0, 0xC0C0C0C0, 0xC0C0C0C0,
								&AbsoluteClippingRect );

	video::SColor colors[] = { 0xC0C0C0C0, 0, 0, 0xC0C0C0C0 };
	if( pDesk->GetDownTexture() )
		driver->draw2DImage( pDesk->GetDownTexture(), 
							rectangle + core::position2di( 0, rectangle.getHeight() ) + AbsoluteRect.UpperLeftCorner, 
							core::recti( core::position2di( 0, 0), pDesk->GetDownTexture()->getSize() ),
							&AbsoluteClippingRect, 
							colors,	true );
	else
		driver->draw2DRectangle(rectangle+core::position2di( 0, rectangle.getHeight() ) + AbsoluteRect.UpperLeftCorner, 
								colors[0], colors[3], colors[1], colors[2],
								&AbsoluteClippingRect );

}

void CNrp2DPictureFlow::draw()
{
	if( !IsVisible )
		return;

	_UpdatePositions();

	if( _drawBackground )
		Environment->getVideoDriver()->draw2DRectangle( video::SColor( 0xff000000 ), AbsoluteRect, &AbsoluteClippingRect );

	if( _images.size() > 0 )
	{
		for( size_t pos=max( 0, _activeIndex-4 ); pos < _activeIndex; pos++ )
			_DrawPairImage( _images[ pos ] );

		for( size_t pos=min( _activeIndex + 4, _images.size()-1); pos > _activeIndex; pos-- )
			 _DrawPairImage( _images[ pos ] );

		if( _activeIndex < static_cast< int >( _images.size() ) )
			_DrawPairImage( _images[ _activeIndex ] );
	}
	IGUIListBox::draw();
}

void CNrp2DPictureFlow::_UpdatePositions()
{
	for( size_t k=0; k < _images.size(); k++ )
	{
		if( _images[ k ]->rectangle.LowerRightCorner.X != _images[ k ]->currentRect.LowerRightCorner.X )
		{
			f32 offset = (_images[ k ]->rectangle.LowerRightCorner.X - _images[ k ]->currentRect.LowerRightCorner.X)
														/(float)Environment->getVideoDriver()->getFPS();

			_images[ k ]->currentRect.LowerRightCorner.X += offset;
		}

		if( _images[ k ]->rectangle.LowerRightCorner.Y != _images[ k ]->currentRect.LowerRightCorner.Y )
		{
			f32 offset = (_images[ k ]->rectangle.LowerRightCorner.Y - _images[ k ]->currentRect.LowerRightCorner.Y)
				/(float)Environment->getVideoDriver()->getFPS();

			_images[ k ]->currentRect.LowerRightCorner.Y += offset;
		}

		if( _images[ k ]->rectangle.UpperLeftCorner.X != _images[ k ]->currentRect.UpperLeftCorner.X )
		{
			f32 offset = (_images[ k ]->rectangle.UpperLeftCorner.X - _images[ k ]->currentRect.UpperLeftCorner.X)
				/(float)Environment->getVideoDriver()->getFPS();

			_images[ k ]->currentRect.UpperLeftCorner.X += offset;
		}

		if( _images[ k ]->rectangle.UpperLeftCorner.Y != _images[ k ]->currentRect.UpperLeftCorner.Y )
		{
			f32 offset = (_images[ k ]->rectangle.UpperLeftCorner.Y - _images[ k ]->currentRect.UpperLeftCorner.Y)
				/(float)Environment->getVideoDriver()->getFPS();

			_images[ k ]->currentRect.UpperLeftCorner.Y += offset;
		}
	}
}

void CNrp2DPictureFlow::Prev( int offset )
{
	_activeIndex -= offset;
	_activeIndex = max( _activeIndex, 0 );

	_UpdateImages();
}

void CNrp2DPictureFlow::Next( int offset )
{
	_activeIndex += offset;
	_activeIndex = min( _activeIndex, _images.size()-1 );

	_UpdateImages();
}

void CNrp2DPictureFlow::_SendEventSelected( const SEvent& event )
{
	SEvent e;
	e.EventType = EET_GUI_EVENT;
	e.GUIEvent.Caller = this;
	e.GUIEvent.Element = 0;
	e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
	Parent->OnEvent(e);
}

bool CNrp2DPictureFlow::OnEvent( const SEvent& event )
{
	s32 curTime = GetTickCount();
	switch( event.EventType )
	{
	case EET_KEY_INPUT_EVENT:
		//uncheck for double event of keyInput
		if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_RIGHT && (curTime - _lastTimeKey > 20))
		{
			Next( +1 );
			_lastTimeKey = GetTickCount();

			_SendEventSelected( event );
			return true;
		}

		if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_LEFT && (curTime - _lastTimeKey > 20))
		{
			Prev( 1 );
			_lastTimeKey = GetTickCount();

			_SendEventSelected( event );
			return true;
		}
	break;

	case EET_MOUSE_INPUT_EVENT:
		if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			int side = AbsoluteRect.getWidth() / 5;
			if( event.MouseInput.X > AbsoluteRect.getCenter().X / 2 )
			{
				int offset = (event.MouseInput.X-AbsoluteRect.getCenter().X / 2) / side - 1;
				Next( offset );

				_SendEventSelected( event );
			}
			else
			{
				int offset = (AbsoluteRect.getCenter().X / 2 - event.MouseInput.X) / side + 1;
				Prev( offset );

				_SendEventSelected( event );
			}
			return true;
		}
	break;
	}

	return IGUIElement::OnEvent(event);
}

CNrp2DPictureFlow::~CNrp2DPictureFlow()
{
	for( size_t pos=0; pos < _images.size(); pos++ )
		delete _images[ pos ];
}

const wchar_t* CNrp2DPictureFlow::getListItem( u32 id ) const
{
	return _images[ id ]->name.c_str();
}

void CNrp2DPictureFlow::setSelected( const wchar_t *item )
{

}

void CNrp2DPictureFlow::setSelected( s32 index )
{
	if( index < (int)_images.size() && index >= 0 )
		_activeIndex = index;
}

void CNrp2DPictureFlow::removeItem( u32 index )
{
	if( index < _images.size() )
	{
		delete _images[ index ];
		_images.erase( index );
	}
}

void CNrp2DPictureFlow::clear()
{
	for( u32 k=0; k < _images.size(); k++ )
		delete _images[ k ];

	_images.clear();
	_activeIndex = 0;
}

void* CNrp2DPictureFlow::getObject( int index )
{
	return index < _images.size() ? _images[ index ]->object : NULL;
}
}//end namespace gui

}//end namespace irr