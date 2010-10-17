#pragma once

#include "ILuaSceneNode.h"

namespace irr
{
	namespace scene
	{
		class ISceneNode;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUASCENENODE( "CLuaSceneNode" );

class CLuaSceneNode : public ILuaSceneNode< irr::scene::ISceneNode >
{
public:
	static Luna<CLuaSceneNode>::RegType methods[];				//методы обертки

	CLuaSceneNode(lua_State *L);		
						
	static const char* ClassName() { return CLASS_LUASCENENODE.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUASCENENODE; }
private:
	irr::scene::ISceneNode* GetTextSceneNode_();
	bool IsChildOfCurrentScene_();
};

}//namespace nrp