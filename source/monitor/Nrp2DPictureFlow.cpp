#include "stdafx.h"
#include "Nrp2DPictureFlow.h"
#include <assert.h>

#define DEFAULT_SMOOTH 0.6f

namespace irr
{

namespace gui
{

video::ITexture* CNrp2DPictureFlow::CNrpImageDescription::CreateTextures_( video::IVideoDriver* driver )
{
	video::ITexture* resultt = NULL;

	if( texture_ == NULL && _pathToTexture.size() )
		texture_ = driver->getTexture( _pathToTexture.ToStr() );

	if( texture_ != NULL )
	{
		NrpText name = NrpText( texture_->getName().getPath().c_str() ) + NrpText( "toDown" );
		resultt = driver->addTexture( core::dimension2du( texture_->getSize().Width, texture_->getSize().Height * DEFAULT_SMOOTH ), 
									  name, texture_->getColorFormat() );

		u32* pTxrData = (u32*)texture_->lock();
		u32* resultTxrData = (u32*)resultt->lock();
		for( size_t k=0; k < resultt->getSize().Height; k++ )
		{
			int offsetPtxr = ( texture_->getSize().Height - k - 1 ) * texture_->getSize().Width;
			int offsetResult = k * resultt->getSize().Width;
			int pith = texture_->getPitch();
			memcpy( resultTxrData+offsetResult, pTxrData+offsetPtxr, texture_->getPitch() );
		}

		texture_->unlock();
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
	//_pictureRect = pictureRect;
	int minSize = min( rectangle.getWidth(), rectangle.getHeight() );
	_pictureRect = core::recti( 0, 0, minSize * 0.5f, minSize * 0.5f );
	_drawBackground = true;
	_lastTimeTexturesUpdate = 0;
}

u32 CNrp2DPictureFlow::addItem( const wchar_t* pathToTexture, const wchar_t* text, void* object )
{
	u32 resultt = addItem( NULL, text );
	_images[ resultt ]->object = object;
	_images[ resultt ]->SetTexture( pathToTexture );
	return resultt;
}

u32 CNrp2DPictureFlow::addItem( video::ITexture* texture, const wchar_t* text )
{
	CNrpImageDescription* descr = new CNrpImageDescription();
	_images.push_back( descr );
	descr->SetTexture( texture ); 
	descr->SetText( text );
	
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

core::recti CNrp2DPictureFlow::_CorrectRect( video::ITexture* texture, const core::recti& rectangle )
{
	if( texture != NULL )
	{
		core::dimension2du txsSize = texture->getSize();
		f32 koeff = rectangle.getWidth() / static_cast< f32 >( min( txsSize.Width, txsSize.Height ) );
		core::dimension2du imSize( static_cast< u32 >( txsSize.Width * koeff ),
								   static_cast< u32 >( txsSize.Height * koeff ) );

		core::recti realRect( rectangle.getCenter().X - imSize.Width / 2, rectangle.getCenter().Y - imSize.Height / 2,
					          rectangle.getCenter().X + imSize.Width / 2, rectangle.getCenter().Y + imSize.Height / 2 );

		return realRect - core::position2di( 0, AbsoluteRect.getHeight() * 0.1f );
	}
	else
		return rectangle - core::position2di( 0, AbsoluteRect.getHeight() * 0.1f );
}

core::recti CNrp2DPictureFlow::_GetDownRect( const core::rectf& rectangle )
{
	core::recti ret( rectangle.UpperLeftCorner.X, rectangle.LowerRightCorner.Y,
					  rectangle.LowerRightCorner.X, 0  );
	ret.LowerRightCorner.Y = rectangle.LowerRightCorner.Y + rectangle.getHeight() * DEFAULT_SMOOTH;

	ret += core::position2di( 0, 10 );

	return ret;
}

void CNrp2DPictureFlow::_UpdateImages()
{
	core::recti tmpRect( AbsoluteRect.getCenter().X - _pictureRect.getWidth()/2, 
						 AbsoluteRect.getCenter().Y - _pictureRect.getHeight()/2,
						 AbsoluteRect.getCenter().X + _pictureRect.getWidth()/2, 
						 AbsoluteRect.getCenter().Y + _pictureRect.getHeight()/2 );

	if( _activeIndex < (int)_images.size() )
	{
		_images[ _activeIndex ]->rectangle = _CorrectRect( _images[ _activeIndex ]->GetTexture(), tmpRect );
	}

	s32 offsetx = 0;
	s32 offsetWidth = AbsoluteRect.getWidth() / 4;
	core::recti lRect = tmpRect;
	if( _activeIndex - 1 >= 0 )
		for( int k=max( 0, _activeIndex-1); k >= 0; k-- )
		{
			offsetx += static_cast< s32 >( offsetWidth * (0.7f - (_activeIndex-k)*0.1f) );
			core::dimension2di sides( 0.7f * lRect.getWidth(), 0.7f * lRect.getHeight() ); 
			
			lRect = core::recti( AbsoluteRect.getCenter().X - sides.Width/2, AbsoluteRect.getCenter().Y - sides.Height/2,
				                 AbsoluteRect.getCenter().X + sides.Width/2, AbsoluteRect.getCenter().Y + sides.Height/2 );

			
			_images[ k ]->rectangle = _CorrectRect( _images[ k ]->GetTexture(), lRect ) - core::position2di( offsetx, 0 );
		}

	offsetx = 0;
	core::recti rRect = tmpRect;
	for( size_t k=min(_activeIndex+1, _images.size()); k < _images.size(); k++ )
	{
		offsetx += static_cast< s32 >( offsetWidth * (0.7f - (k-_activeIndex)*0.1f) );
		core::dimension2di sides( 0.7f * rRect.getWidth(), 0.7f * rRect.getHeight() ); 
		rRect = core::recti( AbsoluteRect.getCenter().X - sides.Width/2, AbsoluteRect.getCenter().Y - sides.Height/2,
							 AbsoluteRect.getCenter().X + sides.Width/2, AbsoluteRect.getCenter().Y + sides.Height/2 );
	
		_images[ k ]->rectangle = _CorrectRect( _images[ k ]->GetTexture(), rRect ) + core::position2di( offsetx, 0 );
	}
}

void CNrp2DPictureFlow::_DrawAny( video::ITexture* txs, const core::recti& rectabgle, video::SColor* colors )
{
	video::IVideoDriver* driver = Environment->getVideoDriver();
	if( txs )
		driver->draw2DImage( txs, rectabgle, 
							 core::recti( core::position2di( 0, 0), txs->getSize() ),
							 &AbsoluteClippingRect, colors, true );
	else
		driver->draw2DRectangle( rectabgle, 
								 *colors, *(colors+3), *(colors+2), *(colors+1),
								 &AbsoluteClippingRect );
}

void CNrp2DPictureFlow::_DrawPairImage( CNrpImageDescription* pDesk )
{
	core::recti rectangle( pDesk->currentRect.UpperLeftCorner.X, pDesk->currentRect.UpperLeftCorner.Y,
						   pDesk->currentRect.LowerRightCorner.X, pDesk->currentRect.LowerRightCorner.Y );

	video::SColor clr( pDesk->blend, pDesk->blend, pDesk->blend, pDesk->blend );
	video::SColor colorsA[] = { clr, clr, clr, clr};
	_DrawAny( pDesk->GetTexture(), rectangle, colorsA );

	colorsA[ 1 ] = colorsA[ 2 ] = 0;
	colorsA[ 3 ] = colorsA[ 0 ] = 0xC0C0C0C0C0;
	_DrawAny( pDesk->GetDownTexture(), pDesk->downRect, colorsA );

	if( gui::IGUIFont* font = Environment->getBuiltInFont() )
	{
		core::recti clipRect = AbsoluteRect;
		font->draw( pDesk->GetText().ToWide(), rectangle, video::SColor( 0xff000000 ), true, true, &clipRect );
	}
	
}

void CNrp2DPictureFlow::draw()
{
	if( !IsVisible )
		return;

	if( GetTickCount() - _lastTimeTexturesUpdate > 250 )
	{
		_UpdateTextures();
		_lastTimeTexturesUpdate = GetTickCount();
	}

	_UpdatePositions();

	if( _drawBackground )
		Environment->getVideoDriver()->draw2DRectangle( video::SColor( 0xff000000 ), AbsoluteRect, &AbsoluteClippingRect );

	if( _images.size() > 0 )
	{
		for( int pos=max( 0, _activeIndex-6 ); pos < _activeIndex; pos++ )
			_DrawPairImage( _images[ pos ] );

		for( size_t pos=min( _activeIndex + 6, _images.size()-1); pos > _activeIndex; pos-- )
			 _DrawPairImage( _images[ pos ] );

		if( _activeIndex < static_cast< int >( _images.size() ) )
			_DrawPairImage( _images[ _activeIndex ] );
	}

	IGUIListBox::draw();
}

void CNrp2DPictureFlow::_UpdateTextures()
{
	for( size_t k=0; k < _images.size(); k++ )
		if( !_images[ k ]->IsLoaded() )
		{
			_images[ k ]->UpdateTextures( Environment->getVideoDriver() );
			_UpdateImages();
			break;
		}
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

		_images[ k ]->downRect = _GetDownRect( _images[ k ]->currentRect );
	}
}

void CNrp2DPictureFlow::Prev( int offset )
{
	_activeIndex -= offset;
	_activeIndex = max( _activeIndex, 0 );

	_UpdateImages();

	PCall( GUIELEMENT_LBXITEM_SELECTED, this, NULL );
}

void CNrp2DPictureFlow::Next( int offset )
{
	_activeIndex += offset;
	_activeIndex = min( _activeIndex, _images.size()-1 );

	_UpdateImages();

	PCall( GUIELEMENT_LBXITEM_SELECTED, this, NULL );
}

void CNrp2DPictureFlow::_SendEventSelected( const SEvent& event )
{
	SEvent e;
	e.EventType = EET_GUI_EVENT;
	e.GUIEvent.Caller = this;
	e.GUIEvent.Element = 0;
	e.GUIEvent.EventType = EGET_LISTBOX_SELECTED_AGAIN;
	Parent->OnEvent(e);

	PCall( GUIELEMENT_LBXITEM_SELECTED, this, NULL );
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

			return true;
		}

		if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_LEFT && (curTime - _lastTimeKey > 20))
		{
			Prev( 1 );
			_lastTimeKey = GetTickCount();
			return true;
		}

		if( event.KeyInput.PressedDown && event.KeyInput.Key == KEY_SPACE && (curTime - _lastTimeKey > 20))
		{
			_lastTimeKey = GetTickCount();
			_SendEventSelected( event );
			return true;
		}
	break;

	case EET_MOUSE_INPUT_EVENT:
		if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			int side = AbsoluteRect.getWidth() / 5;
			if( event.MouseInput.X > AbsoluteRect.getCenter().X + _pictureRect.getWidth() / 2 )
			{
				int offset = (event.MouseInput.X-AbsoluteRect.getCenter().X ) / side;
				Next( offset );
				return true;
			}
			
			if( event.MouseInput.X < AbsoluteRect.getCenter().X - _pictureRect.getWidth() / 2 )
			{
				int offset = (AbsoluteRect.getCenter().X - event.MouseInput.X) / side;
				Prev( offset );
				return true;
			}

			core::recti centralRect = _pictureRect;
			centralRect += core::position2di( AbsoluteRect.getCenter().X - _pictureRect.getWidth() / 2, 
											  AbsoluteRect.getCenter().Y - _pictureRect.getHeight() / 2 );
				
			if( centralRect.isPointInside( core::position2di( event.MouseInput.X, event.MouseInput.Y ) ) )
			{
				_SendEventSelected( event );
				return true;
			}

			return true;
		}
	break;
	}

	return IGUIListBox::OnEvent(event);
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

void CNrp2DPictureFlow::setItemTexture( u32 index, video::ITexture* texture )
{
	assert( index < _images.size() );
	if( index < _images.size() )
		_images[ index ]->SetTexture( texture );
}

void CNrp2DPictureFlow::setItemBlend( u32 index, int blend )
{
	assert( index < _images.size() );
	if( index < _images.size() )
		_images[ index ]->blend = blend;
}
}//end namespace gui

}//end namespace irr