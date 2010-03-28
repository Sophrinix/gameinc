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

class CLuaSceneNode : public ILuaSceneNode< irr::scene::ISceneNode >
{
public:
	static Luna<CLuaSceneNode>::RegType methods[];				//методы обертки

	CLuaSceneNode(lua_State *L);		
						
	static const char* StaticGetLuaName() { return "CLuaSceneNode"; }
private:
	irr::scene::ISceneNode* GetTextSceneNode_();
};

}//namespace nrp