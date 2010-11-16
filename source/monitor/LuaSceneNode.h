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
	static const char* ClassName();

	CLuaSceneNode(lua_State *L);		
private:
	irr::scene::ISceneNode* GetTextSceneNode_();
	bool IsChildOfCurrentScene_();
};

}//namespace nrp