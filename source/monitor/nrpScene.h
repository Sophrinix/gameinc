#pragma once

#include <irrlicht.h>
#include "LuaFunctionality.h"

namespace irr
{
class IrrlichtDevice;
struct SEvent;

namespace scene
{

class ISceneNode;

class INrpScene : public ILuaFunctionality
{
public:
	INrpScene()
	{};
	
	virtual ~INrpScene() {};

	virtual bool OnEvent( const SEvent& event ) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnEnter() = 0;
	virtual void OnLeave() = 0;
	virtual void SetOption( const core::stringc& name, bool amount ) = 0; 
	virtual scene::ISceneNode* GetSelectedNode() = 0;
	virtual void SetSelectedNode( scene::ISceneNode* node ) {}
	virtual core::vector3df GetCurrentWorldPosition() = 0;

	virtual ISceneNode* GetNodeByName( char* name ) { return NULL; }
};

}
}