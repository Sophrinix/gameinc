#pragma once
#include <vector>
#include "nrpScene.h"
#include <irrlicht.h>

namespace irr
{

namespace scene
{

class CNrpLoginScene : public INrpScene
{
public:
	CNrpLoginScene();
	virtual ~CNrpLoginScene() {};

	virtual bool OnEvent( const irr::SEvent& event );
	virtual void OnUpdate();
	virtual void OnEnter();
	virtual void OnLeave();
	virtual void SetOption( const core::stringc& name, bool amount ) {}
	virtual scene::ISceneNode* GetSelectedNode() { return NULL; }
	virtual core::vector3df GetCurrentWorldPosition() { return core::vector3df(0, 0, 0); }

private:
	typedef enum { BTN_OK=1, BTN_EXIT, BTN_COUNT } BTN_NAME;

	// stuff added by d.a.
	irr::scene::IAnimatedMesh *someMesh_; 
	irr::scene::ISceneNode *waterNode_, *lightNode_;
	irr::scene::ICameraSceneNode * camera_;
	irr::gui::IGUIInOutFader* fader_;
	irr::gui::IGUIImage* mainLogo_;
	irr::gui::IGUIImage* companyBar_;
	irr::video::ITexture* nerpaLogoTexture_;
	irr::video::ITexture* companyBarTexture_;	
	// end of stuff
	const int SCENE_ID;

	void UpdateImages_();
};

}//namespace scene

}//namespace irr