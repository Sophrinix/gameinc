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
	DEFINE_PROPERTIES_AND_METHODS(CLuaSceneNode)
	static const char* ClassName();

	CLuaSceneNode(lua_State *L, bool);		
private:
	irr::scene::ISceneNode* GetTextSceneNode_();
	bool IsChildOfCurrentScene_();
};

}//namespace nrp