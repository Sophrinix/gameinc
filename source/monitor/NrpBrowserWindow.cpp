#include "StdAfx.h"
#include <irrlicht.h>
#include "NrpBrowserWindow.h"
#include "HTMLEngine.h"

using namespace nrp;

namespace irr
{

namespace gui
{

CNrpBrowserWindow::CNrpBrowserWindow(void) : CNrpWindow( NULL, NULL, NULL, -1, core::recti( 0, 0, 0, 0 ) )
{
}

CNrpBrowserWindow::CNrpBrowserWindow( gui::IGUIEnvironment* env, 
									  video::ITexture* texture, 
									  s32 id, 
									  core::position2di pos,
									  core::dimension2du size ) : CNrpWindow( env, env->getRootGUIElement(), texture, id, 
																			  core::recti( pos, size+core::dimension2du( 10, 25 ) ) )
{
	setRelativePosition( core::recti( 0, 0, size.Width + 14, size.Height + 15 ) );
	image_ = Environment->addImage( core::recti( 7, 7, size.Width+7, size.Height+7 ), this, -1, 0 );
	image_->setScaleImage( true );
	bringToFront( getCloseButton() );
	
	setDraggable( true );
	drop();
}

CNrpBrowserWindow::~CNrpBrowserWindow(void)
{
	HTMLEngine::Instance().SetBrowserWindow( NULL );
	image_->remove();
}

bool CNrpBrowserWindow::OnEvent(const SEvent& event)
{
	if( !IsVisible )
		return false;

	switch(event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
	{
		core::position2di absPos(event.MouseInput.X, event.MouseInput.Y);
		core::position2di mousePos = absPos - image_->getAbsolutePosition().UpperLeftCorner;

		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			HTMLEngine::Instance().MouseDown(mousePos.X, mousePos.Y);
		break;

		case EMIE_LMOUSE_LEFT_UP:
			HTMLEngine::Instance().MouseUp(mousePos.X, mousePos.Y);
		break;
		
		case EMIE_MOUSE_WHEEL:
			HTMLEngine::Instance().ScrollByLines( event.MouseInput.Wheel > 0 ? -1 : 1 );
		break;

		case EMIE_MOUSE_MOVED:
			HTMLEngine::Instance().MouseMoved( mousePos.X, mousePos.Y );
		break;
		}

		if( image_->getRelativePosition().isPointInside( mousePos) )
			return true;
	}
	break;

	case EET_GUI_EVENT:
	{
		if( event.GUIEvent.EventType == EGET_BUTTON_CLICKED && event.GUIEvent.Caller == buttons_[ BTNE_CLOSE ] )
		{
			setVisible( false );
			return true;
		}

		if( event.GUIEvent.EventType == EGET_ELEMENT_LEFT && event.GUIEvent.Caller == image_ )
		{
			HTMLEngine::Instance().SetFocus( false );
			return true;
		}
	}
	break;

	case EET_KEY_INPUT_EVENT:
	{
		if (event.KeyInput.PressedDown)
			HTMLEngine::Instance().KeyPress(event.KeyInput.Key);
	}
	break;

	default:
		{
			int k=0;
		}
	break;
	}

	return CNrpWindow::OnEvent( event );
}

void CNrpBrowserWindow::SetTexture( video::ITexture* texture )
{
	image_->setImage( texture );
}

void CNrpBrowserWindow::draw()
{
	if( IsVisible )
	{
		HTMLEngine::Instance().Update();

		CNrpWindow::draw();
	}
}
}//namespace gui

}//namespace irr