#pragma once

#include <map>
#include <irrlicht.h>

#include "NrpWindow.h"

namespace irr
{

namespace scene
{
	class ITerrainSceneNode;
}


namespace gui
{

class CNrpGloablMap : public CNrpWindow
{
public:
	CNrpGloablMap( const core::rect<s32>& rectangle, IGUIEnvironment* env, s32 id );
	~CNrpGloablMap( void );

	void draw();
	bool OnEvent( const SEvent& event );
	void AddTerrain( scene::ITerrainSceneNode* terrain, video::ITexture* imageTex );
	
private:

	void RecalculateImagePosition_();

	std::map< scene::ITerrainSceneNode*, gui::IGUIImage* > images_;
};

}//namespace gui

}//namespace irr