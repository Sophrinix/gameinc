#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaSceneNode.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUASCENENODE( "CLuaSceneNode" );

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
	const core::list< scene::ISceneNode* >& childs = _nrpEngine.GetSceneManager()->
																	getRootSceneNode()->getChildren();

	core::list< scene::ISceneNode* >::ConstIterator pIter = childs.begin();
	for(; pIter != childs.end(); pIter++ )
		if( *pIter == _object )
			return true;

	return false;
}

const char* CLuaSceneNode::ClassName()
{
	return ( CLASS_LUASCENENODE );
}
}//namespace nrp