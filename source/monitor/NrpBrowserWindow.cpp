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
	imageRect_ = core::recti( 5, 20, size.Width + 5, size.Height + 20 );
	//image_->setScaleImage( true );
	setDraggable( true );
	drop();
}

CNrpBrowserWindow::~CNrpBrowserWindow(void)
{
	HTMLEngine::Instance().SetBrowserWindow( NULL );
	//image_->remove();
}

bool CNrpBrowserWindow::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
	{
		core::position2di mousePos = core::position2di(event.MouseInput.X, event.MouseInput.Y) - RelativeRect.UpperLeftCorner - imageRect_.UpperLeftCorner;

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
	texture_ = texture;
}

void CNrpBrowserWindow::draw()
{
	if( IsVisible )
	{
		HTMLEngine::Instance().Update();

		CNrpWindow::draw();

		if( texture_ )
		{
			core::recti txsResct( 0, 0, texture_->getSize().Width, texture_->getSize().Height );
			Environment->getVideoDriver()->draw2DImage( texture_, imageRect_ + AbsoluteRect.UpperLeftCorner, txsResct );
		}	
	}
}
}//namespace gui

}//namespace irr