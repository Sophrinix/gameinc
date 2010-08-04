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
	image_ = Environment->addImage( core::recti( 5, 20, size.Width + 5, size.Height + 20 ), this, -1, L"" );
	image_->setScaleImage( true );
	drop();
}

CNrpBrowserWindow::~CNrpBrowserWindow(void)
{
	HTMLEngine::Instance().SetBrowserWindow( NULL );
	image_->remove();
}

bool CNrpBrowserWindow::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
	{
		core::position2di mousePos = core::position2di(event.MouseInput.X, event.MouseInput.Y) - RelativeRect.UpperLeftCorner - image_->getAbsolutePosition().UpperLeftCorner;

		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			HTMLEngine::Instance().MouseDown(mousePos.X, mousePos.Y);
		return true;

		case EMIE_LMOUSE_LEFT_UP:
			HTMLEngine::Instance().MouseUp(mousePos.X, mousePos.Y);
		return true;
		
		case EMIE_MOUSE_WHEEL:
			HTMLEngine::Instance().ScrollByLines( event.MouseInput.Wheel > 0 ? -1 : 1 );
		return true;

		case EMIE_MOUSE_MOVED:
			HTMLEngine::Instance().MouseMoved( mousePos.X, mousePos.Y );
		return true;
		}
	}
	break;
	
	case EET_KEY_INPUT_EVENT:
	{
		if (event.KeyInput.PressedDown)
		{
			HTMLEngine::Instance().KeyPress(event.KeyInput.Key);
			return true;
		}
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
	HTMLEngine::Instance().Update();

	CNrpWindow::draw();
}
}//namespace gui

}//namespace irr