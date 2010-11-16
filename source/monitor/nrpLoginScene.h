#pragma once
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
	virtual void SetOption( const nrp::NrpText& name, bool amount ) {}
	virtual scene::ISceneNode* GetSelectedNode() { return NULL; }
	virtual core::vector3df GetCurrentWorldPosition() { return core::vector3df(0, 0, 0); }

private:
	typedef enum { BTN_OK=1, BTN_EXIT, BTN_COUNT } BTN_NAME;

	// stuff added by d.a.
	IAnimatedMesh *someMesh_; 
	ISceneNode *waterNode_, *lightNode_;
	ICameraSceneNode * camera_;
	gui::IGUIInOutFader* fader_;
	gui::IGUIImage* mainLogo_;
	gui::IGUIImage* companyBar_;
	video::ITexture* nerpaLogoTexture_;
	video::ITexture* companyBarTexture_;	
	// end of stuff
	const int SCENE_ID;

	void UpdateImages_();
};

}//namespace scene

}//namespace irr