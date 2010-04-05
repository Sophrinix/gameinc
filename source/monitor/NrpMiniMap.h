#pragma once
#include "nrprotatableimage.h"

namespace irr
{

namespace scene
{
	class ISceneNode;
}


namespace gui
{

class CNrpMiniMap : public CNrpRotatableImage
{
public:
	CNrpMiniMap( const core::rect<s32>& rect, IGUIEnvironment* env, s32 id, scene::ISceneNode* pr_node=NULL );
	~CNrpMiniMap(void);

	void draw();
	bool OnEvent( const SEvent& event );
	f32  GetAngleOffset() { return offsetAngle_; }
	void SetOnRotateAction( const core::stringc& funcName )
	{
		onRotateAction_ = funcName;
	}
private:
	scene::ISceneNode* animNode_;
	f32 scale_;
	bool mouse2rotate_;
	f32 offsetAngle_, startAngle_;
	core::dimension2di startSize_;
	void SetupQuadMesh_(scene::SMeshBuffer& mesh, const f32 width);
	f32 CNrpMiniMap::GetCurrentAngle_( const core::position2di& mPos );

	core::stringc onRotateAction_; 
};

}//namespace gui

}//namespace irr