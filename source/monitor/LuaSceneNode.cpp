#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaSceneNode.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{

Luna< CLuaSceneNode >::RegType CLuaSceneNode::methods[] =			//���������� ������
{
	LUNA_ILUASCENENODE_HEADER( CLuaSceneNode ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	{0,0}
};

CLuaSceneNode::CLuaSceneNode(lua_State *L)	: ILuaSceneNode(L, "CLuaSceneNode")							//�����������
{}

}//namespace nrp