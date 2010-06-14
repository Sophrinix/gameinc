#pragma once

#include <irrlicht.h>
#include <string>
#include <map>

#include "nrpScene.h"

namespace irr
{

namespace scene
{

class CNrpMainScene : public INrpScene
{
public:
	typedef enum {  CURSOR_MOVE=0, CURSOR_ROTATE, CURSOR_NORMAL, 
					CURSOR_LMB_ACTION, CURSOR_RMB_ACTION } CURSOR_STATUS;

	typedef enum { WATER_AND_POINT=0, 
				   ONLY_WATER, ONLY_POINT, 
				   NO_WATER_NO_POINT, 
				   WATER_VIEW_COUNT } WATER_VIEW_STATUS;

private:
	friend class CLoginWindow; 
	friend class CMainSceneEventer;

	//RealisticWaterSceneNode* water_;

	gui::IGUIInOutFader* fader_;

	WATER_VIEW_STATUS waterView_;
	scene::ISceneNode* selectedNode_;
	core::vector3df intersection_;
	int lastTimeNodeSelect_;

	core::position2di oldPos_;
	core::triangle3df selectedTriangle_;

	bool mouseSceneBLeftEvent_;
	bool twinLeftMouseClick_;

	bool InitWater_();					//добавление сцены воды
	void GetNodeAndIntersectionFromCursor_( scene::ISceneNode*& node, core::vector3df& point, bool &doubleClick );
	void RenderScene_();
	bool IsObjectChildOfScene_( scene::ISceneNode* node );

	std::map< core::stringc, bool > options_;
public:
	CNrpMainScene();								
	~CNrpMainScene(void);
	
	bool OnEvent( const SEvent& event );
	void OnUpdate();							//функция отрисовки мира
	void OnEnter();							//создание сцены
	void OnLeave();
	void SetOption( const core::stringc& name, bool amount );

	scene::ISceneNode* GetSelectedNode() { return selectedNode_; }
	void SetSelectedNode( scene::ISceneNode* node ) { selectedNode_ = node; }
	core::vector3df GetCurrentWorldPosition();
};

}//namespace scene

}//namespace irr