#include "StdAfx.h"
#include <irrlicht.h>
#include "nrpVideoConfig.h"
#include "nrpLoadingScreen.h"

namespace irr
{

namespace scene
{

CLoadingScreen::~CLoadingScreen(void)
{
}
//////////////////////////////////////////////////////////////////////////

CLoadingScreen::CLoadingScreen( video::IVideoDriver* driver )
{
	driver_ = driver;
}
//////////////////////////////////////////////////////////////////////////

void CLoadingScreen::render( int progress )
{
	core::dimension2du sizer = driver_->getScreenSize();

	driver_->beginScene( true, true, video::SColor(150,50,50,50) );			//рисуем сцену 

	driver_->draw2DRectangle( video::SColor( 0xff000000 ), core::recti( 0, 0, sizer.Width, sizer.Height ) );//заполняем всю сцену черным цветом

	driver_->draw2DRectangle( video::SColor( 0xffffffff ), core::recti( sizer.Width / 4, sizer.Height * 4 / 5,
																		sizer.Width * 3 / 4, sizer.Height * 4 / 5 + 20 ) );

	driver_->draw2DRectangle( video::SColor( 0xff000000 ), core::recti( sizer.Width / 4 + 1, sizer.Height * 4 / 5 + 1,
																		sizer.Width * 3 / 4 - 1, sizer.Height * 4 / 5 + 20 - 1 ) );

	core::recti rect_fill( sizer.Width / 4 + 2, sizer.Height * 4 / 5 + 2,
							sizer.Width * 3 / 4 - 2, sizer.Height * 4 / 5 + 20 - 2 );

	core::dimension2di rf_size = rect_fill.getSize();

	rect_fill.LowerRightCorner.X = rect_fill.UpperLeftCorner.X + (s32)(progress / 100.f * rf_size.Width);

	driver_->draw2DRectangle( video::SColor( 0xffffffff ), rect_fill );

	driver_->endScene();
}
//////////////////////////////////////////////////////////////////////////

}//namespace scene

}//namespace irr