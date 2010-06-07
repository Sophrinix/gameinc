#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaSceneNode.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{

Luna< CLuaSceneNode >::RegType CLuaSceneNode::methods[] =			//реализуемы методы
{
	LUNA_ILUASCENENODE_HEADER( CLuaSceneNode ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	{0,0}
};

CLuaSceneNode::CLuaSceneNode(lua_State *L)	: ILuaSceneNode(L, "CLuaSceneNode")							//конструктор
{}

bool CLuaSceneNode::IsChildOfCurrentScene_()
{
	const core::list< scene::ISceneNode* >& childs = CNrpEngine::Instance().GetSceneManager()->
																	getRootSceneNode()->getChildren();

	core::list< scene::ISceneNode* >::ConstIterator pIter = childs.begin();
	for(; pIter != childs.end(); pIter++ )
		if( *pIter == object_ )
			return true;

	return false;}
}//namespace nrp