#include "StdAfx.h"
#include "nrpGlobalMap.h"
#include "NrpWorldConfig.h"

namespace irr
{

namespace gui
{

irr::gui::CNrpGloablMap::CNrpGloablMap( const core::rect<s32>& rectangle, IGUIEnvironment* env, s32 id )
: CNrpWindow( env, env->getRootGUIElement(), 0, id, rectangle )
{
}

void CNrpGloablMap::draw()
{
	CNrpWindow::draw();
}

bool CNrpGloablMap::OnEvent( const SEvent& event )
{
	return CNrpWindow::OnEvent( event );
}

void CNrpGloablMap::AddTerrain( scene::ITerrainSceneNode* terrain, video::ITexture* imageTex )
{
	gui::IGUIImage* img = Environment->addImage( core::recti( 0, 0, 12, 12 ), this );
	img->setScaleImage(true);
	img->setImage( imageTex );
	images_[ terrain ] = img;

	RecalculateImagePosition_();
}

void CNrpGloablMap::RecalculateImagePosition_()
{
	std::map< scene::ITerrainSceneNode*, gui::IGUIImage* >::iterator pIter = images_.begin();

	for(; pIter != images_.end(); pIter++ )
	{
		nrp::CNrpWorldConfig& conf = nrp::CNrpWorldConfig::Instance();
		core::aabbox3df box = pIter->first->getBoundingBox();
		gui::IGUIImage* img = pIter->second;

		core::recti imRect;
		core::vector3df maxpos = conf[ nrp::WORLD_MAXEDGE ].As<core::vector3df>();
		core::vector3df minpos = conf[ nrp::WORLD_MAXEDGE ].As<core::vector3df>();

		imRect.UpperLeftCorner.X = (s32)(box.MinEdge.X / ( maxpos.X - minpos.X ) * AbsoluteRect.getWidth());
		imRect.LowerRightCorner.X = (s32)(box.MaxEdge.X / ( maxpos.X - minpos.X ) * AbsoluteRect.getWidth());

		imRect.UpperLeftCorner.Y = (s32)(box.MinEdge.Z / ( maxpos.Z - minpos.Z ) * AbsoluteRect.getHeight());
		imRect.LowerRightCorner.Y = (s32)(box.MaxEdge.Z / ( maxpos.Z - minpos.Z ) * AbsoluteRect.getHeight());
		img->setRelativePosition( imRect );
	}
}

CNrpGloablMap::~CNrpGloablMap( void )
{

}
}//namespace gui

}//namesapce irr

