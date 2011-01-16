#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaSceneNode.h"
#include "nrpEngine.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUASCENENODE( "CLuaSceneNode" );

BEGIN_LUNA_METHODS(CLuaSceneNode)
	LUNA_ILUASCENENODE_HEADER( CLuaSceneNode )
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaSceneNode)
END_LUNA_PROPERTIES

CLuaSceneNode::CLuaSceneNode(lua_State *L, bool ex)	: ILuaSceneNode(L, CLASS_LUASCENENODE, ex)							//конструктор
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