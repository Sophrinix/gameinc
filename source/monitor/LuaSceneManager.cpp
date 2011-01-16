#include "StdAfx.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <assert.h>

#include "LuaSceneManager.h"
#include "nrpEngine.h"
#include "nrpCameraAnimator.h"
#include "nrpLoadingScreen.h"
#include "NrpWorldConfig.h"
#include "nrpScene.h"
#include "LuaCamera.h"
#include "LuaSceneNode.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUASCENEMANAGER( "CLuaSceneManager" );

BEGIN_LUNA_METHODS(CLuaSceneManager)
	LUNA_ILUAOBJECT_HEADER( CLuaSceneManager )
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetActiveCamera )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddTerrainSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddSkyDomeSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddCameraSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddCubeSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetRootSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddTextSceneNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetSceneNodeByName )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetCurrentWorldPosition )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetSelectedNode )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, SetActiveCamera )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddNerpaCameraAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, CreateTerrainTriangleSelector )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, CreateTriangleSelectorFromBoundingBox )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, CreateCollisionResponseAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, DrawProgress )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, SetWorldSize )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, LinkTerrain )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddSceneFunction )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, RemoveSceneFunction )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetRayFromCursorCollisionWithTerrains )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetMarkText )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, SetMarkText )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, AddToDeletionQueue )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, RenderScene )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, LoadIrrlichtScene )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, GetSceneNodeByID )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, RemoveAllNodes )
	LUNA_AUTONAME_FUNCTION( CLuaSceneManager, SetSelectedNode )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaSceneManager)
END_LUNA_PROPERTIES

CLuaSceneManager::CLuaSceneManager(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUASCENEMANAGER, ex)
{}

int CLuaSceneManager::GetActiveCamera( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetActiveCamera not need any parameter");

	void* cam = NULL;
	IF_OBJECT_NOT_NULL_THEN cam = (void*)_object->getActiveCamera();

	lua_pop( vm, argc );
	lua_pushlightuserdata( vm, cam );
	Luna< CLuaCamera >::constructor( vm );

	return 1;
}

int CLuaSceneManager::AddTerrainSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:AddTerrainSceneNode need one parameter (char*) ");
	NrpText heightMapFile = lua_tostring(vm, 2);	
	NrpText name = lua_tostring( vm, 3 );

	scene::ITerrainSceneNode* terrain = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		terrain = _object->addTerrainSceneNode( heightMapFile );
		terrain->setMaterialFlag( video::EMF_LIGHTING, false );
		terrain->setName( name.ToWide() );
	}

	lua_pushlightuserdata(vm, (void*)terrain);  // And set the global name of this pointer

	return 1;
}

int CLuaSceneManager::AddSkyDomeSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaSceneManager:AddSkyDomeSceneNode need 6 parameter ");
	
	NrpText strSkyTxs = lua_tostring(vm, 2);	

	u32 horiRes = (u32)lua_tointeger( vm, 3 );
	u32 vertRes = (u32)lua_tointeger( vm, 4 );
	f32 txsPercentage = (f32)lua_tonumber( vm, 5 );
	f32 sphPercentage = (f32)lua_tonumber( vm, 6 );
	f32 radius = (f32)lua_tonumber( vm, 7 );

	scene::ISceneNode* sky = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* skyTexture = _object->getVideoDriver()->getTexture( strSkyTxs );

		sky = _object->addSkyDomeSceneNode( skyTexture, horiRes, vertRes, txsPercentage, sphPercentage, radius );
	}

	lua_pushlightuserdata(vm, (void*)sky);  // And set the global name of this pointer

	return 1;
}

int CLuaSceneManager::AddCameraSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:AddCameraSceneNode need one parameter CLuaSceneNode ");
	
	scene::ISceneNode* parent = (scene::ISceneNode*)lua_touserdata(vm, 2);	

	scene::ICameraSceneNode* cam = NULL;
	
	IF_OBJECT_NOT_NULL_THEN cam = _object->addCameraSceneNode( parent );

	lua_pushlightuserdata(vm, (void*)cam);  // And set the global name of this pointer

	return 1;
}

int CLuaSceneManager::GetRootSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetRootSceneNode not need any parameter");

	void* node = NULL;

	IF_OBJECT_NOT_NULL_THEN node = (void*)_object->getRootSceneNode();
	lua_pushlightuserdata( vm, node );

	return 1;
}

int CLuaSceneManager::SetActiveCamera( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:SetActiveCamera need one parameter CLuaSceneNode ");

	scene::ICameraSceneNode* cam = (scene::ICameraSceneNode*)lua_touserdata(vm, 2);	

	IF_OBJECT_NOT_NULL_THEN _object->setActiveCamera( cam );  // And set the global name of this pointer

	return 1;
}

int CLuaSceneManager::AddNerpaCameraAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 5, 5, "Function CLuaSceneManager:SetActiveCamera need one parameter CLuaSceneNode ");

	float angle = (float)lua_tointeger( vm, 2 );
	float rotateSpeed = (float)lua_tonumber( vm, 3 );
	float zoomSpeed = (float)lua_tonumber( vm, 4 );
	float translateSpeed = (float)lua_tonumber( vm, 5 );

	scene::CNrpCameraAnimator* anim = new scene::CNrpCameraAnimator(	_nrpEngine.GetDevice()->getCursorControl(),
																		angle,
																		rotateSpeed,		//скорость поворота
																		zoomSpeed,		//скорость зума
																		translateSpeed );	//скорость перемещения

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaSceneManager::CreateTerrainTriangleSelector( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:CreateTerrainTriangleSelector need 2 parameter ");

	scene::ITerrainSceneNode* node = (scene::ITerrainSceneNode*)lua_touserdata( vm, 2 );
	s32 amount = lua_tointeger( vm, 3 );

	scene::ITriangleSelector* selector = NULL;
	
	IF_OBJECT_NOT_NULL_THEN selector = _object->createTerrainTriangleSelector( node, amount );

	lua_pushlightuserdata( vm, (void*)selector );

	return 1;
}

int CLuaSceneManager::CreateCollisionResponseAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 12, 12, "Function CLuaSceneManager:CreateCollisionResponseAnimator need 12 parameter ");

	scene::ITriangleSelector* selector = (scene::ITriangleSelector*)lua_touserdata( vm, 2 );
	scene::ISceneNode* node = (scene::ISceneNode*)lua_touserdata( vm, 3 );
	
	core::vector3df elipsoidRadius;
	elipsoidRadius.X = (float)lua_tonumber( vm, 4 );
	elipsoidRadius.Y = (float)lua_tonumber( vm, 5 );
	elipsoidRadius.Z = (float)lua_tonumber( vm, 6 );

	core::vector3df gravity;
	gravity.X = (float)lua_tonumber( vm, 7 );
	gravity.Y = (float)lua_tonumber( vm, 8 );
	gravity.Z = (float)lua_tonumber( vm, 9 );

	core::vector3df elipsoidTranslation;
	elipsoidTranslation.X = (float)lua_tonumber( vm, 10 );
	elipsoidTranslation.Y = (float)lua_tonumber( vm, 11 );
	elipsoidTranslation.Z = (float)lua_tonumber( vm, 12 );
	
	scene::ISceneNodeAnimator* anim = NULL;
	
	IF_OBJECT_NOT_NULL_THEN anim = _object->createCollisionResponseAnimator(  selector, node, 
																			  elipsoidRadius, gravity, 
																			  elipsoidTranslation);

	lua_pushlightuserdata( vm, (void*)anim );
	
	return 1;
}

int CLuaSceneManager::DrawProgress( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:DrawProgress need one parameter ");

	int progress = lua_tointeger(vm, 2);	
	NrpText text( lua_tostring( vm, 3 ) );

	IF_OBJECT_NOT_NULL_THEN
	{
		scene::CLoadingScreen ld( _object->getVideoDriver(), _nrpEngine.GetGuiEnvironment()->getFont( "font_14" ) );
		ld.render( progress, text.ToWide() );
	}

	return 1;
}

int CLuaSceneManager::SetWorldSize( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:SetWorldSize need 2 parameter ");

	core::dimension2df wsize;
	wsize.Width = (f32)lua_tointeger(vm, 2);	
	wsize.Height = (f32)lua_tointeger(vm, 3);	

	CNrpWorldConfig::Instance()[WORLD_REAL_SIZE] = wsize;

	return 1;
}

int CLuaSceneManager::LinkTerrain( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:LinkTerrain need 1 parameter ");

	scene::ITerrainSceneNode* terrain = (scene::ITerrainSceneNode*)lua_touserdata(vm, 2);	

	RecalculateWorldParams_( terrain );

	return 1;
}

void CLuaSceneManager::RecalculateWorldParams_( scene::ITerrainSceneNode* newTerrain )
{
	CNrpWorldConfig& conf = CNrpWorldConfig::Instance();
	core::vector3df minpos = conf[WORLD_MINEDGE];
	minpos.X = min( minpos.X, newTerrain->getBoundingBox().MinEdge.X );
	minpos.Z = min( minpos.Z, newTerrain->getBoundingBox().MinEdge.Z );
	conf[ WORLD_MINEDGE ] = minpos;

	core::vector3df maxpos = conf[ WORLD_MAXEDGE ];
	maxpos.X = max( maxpos.X, newTerrain->getBoundingBox().MaxEdge.X );
	maxpos.Z = max( maxpos.Z, newTerrain->getBoundingBox().MaxEdge.Z );
	conf[ WORLD_MAXEDGE ] = maxpos;

	core::dimension2df wsize = conf[ WORLD_REAL_SIZE ];
	float koeff = (maxpos.X - minpos.X) / wsize.Width;
	conf[ WORLD_WIDTH_COEFF ] = koeff;
}

int CLuaSceneManager::AddCubeSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:AddCubeSceneNode need 1 parameter ");

	NrpText name = lua_tostring( vm, 2 );

	scene::ISceneNode* cube = NULL;
	
	IF_OBJECT_NOT_NULL_THEN
	{
		cube = _object->addCubeSceneNode();
		cube->setName( name.ToStr() );
	}

	lua_pushlightuserdata( vm, cube );

	return 1;
}

int CLuaSceneManager::GetSceneNodeByName( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetSceneNodeByName need 1 parameter ");

	NrpText name = lua_tostring( vm, 2 );

	scene::ISceneNode* node = NULL;
	
	IF_OBJECT_NOT_NULL_THEN node = _object->getSceneNodeFromName( name );

	lua_pop( vm, argc );
	lua_pushlightuserdata( vm, node );
	Luna< CLuaSceneNode >::constructor( vm );

	return 1;
}

int CLuaSceneManager::AddSceneFunction( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:AddSceneFunction need 2 parameter ");

	int typef = lua_tointeger( vm, 2 );
	NrpText name = lua_tostring( vm, 3 );

	_nrpEngine.GetCurrentScene()->AddLuaFunction( typef, name );

	return 1;
}

int CLuaSceneManager::RemoveSceneFunction( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaSceneManager:RemoveSceneFunction need 2 parameter ");

	int typef = lua_tointeger( vm, 2 );
	NrpText name = lua_tostring( vm, 3 );

	_nrpEngine.GetCurrentScene()->RemoveLuaFunction( typef, name );

	return 1;
}

int CLuaSceneManager::GetCurrentWorldPosition( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetCurrentWorldPosition need any parameter ");

	core::vector3df position = _nrpEngine.GetCurrentScene()->GetCurrentWorldPosition();

	lua_pushnumber( vm, position.X );
	lua_pushnumber( vm, position.Y );
	lua_pushnumber( vm, position.Z );

	return 3;
}

int CLuaSceneManager::CreateTriangleSelectorFromBoundingBox(lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:CreateTriangleSelectorFromBoundingBox need ISceneNode parameter ");

	scene::ISceneNode* node = (scene::ISceneNode*)lua_touserdata( vm, 2 );

	scene::ITriangleSelector* selector = NULL;
	
	IF_OBJECT_NOT_NULL_THEN selector = _object->createTriangleSelectorFromBoundingBox( node );

	lua_pushlightuserdata( vm, selector );

	return 1;
}

int CLuaSceneManager::GetSelectedNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetCurrentWorldPosition need any parameter ");

	scene::ISceneNode* node = _nrpEngine.GetCurrentScene()->GetSelectedNode();

	lua_pushlightuserdata( vm, node );

	return 1;
}

int CLuaSceneManager::AddTextSceneNode( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 12, 12, "Function CLuaSceneManager:AddTextSceneNode need 12 parameter ");

	int fontSize = lua_tointeger( vm, 2 );
	NrpText text( lua_tostring( vm, 3 ) ); 
	video::SColor color( lua_tointeger( vm, 4 ), lua_tointeger( vm, 5 ), lua_tointeger( vm, 6 ), lua_tointeger( vm, 7 ) );
	scene::ISceneNode* node = (scene::ISceneNode*)lua_touserdata( vm, 8 );
	core::vector3df position( (f32)lua_tonumber( vm, 9 ), (f32)lua_tonumber( vm, 10 ), (f32)lua_tonumber( vm, 11 ) );
	int id = lua_tointeger( vm, 12 );
	gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( NrpText( "font_" ) + NrpText( fontSize ) );

	scene::ITextSceneNode* textNode = NULL;
	
	IF_OBJECT_NOT_NULL_THEN textNode = _object->addTextSceneNode( font, text.ToWide(), color, node, position, id );

	lua_pushlightuserdata( vm, textNode );

	return 1;
}

int CLuaSceneManager::GetRayFromCursorCollisionWithTerrains( lua_State* vm ) 
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetRayFromCursorCollisionWithTerrains need any parameter ");

	//Сюда поместится треугольник пересечения
	core::triangle3df tri;
	//Здесь лежат террайны для обработки
	core::array< scene::ISceneNode* > terrains; 
	//Это точка пересчения c лучом
	core::vector3df collisionPos( 0, 0, 0 );

	//это объект сцены с которым было пересечение
	const scene::ISceneNode* outNode=NULL;
	//это флаг пересечения
	bool isCollision = false;

	scene::ISceneManager* smgr = _nrpEngine.GetSceneManager();
	core::position2di pos = _nrpEngine.GetDevice()->getCursorControl()->getPosition();
	core::line3df line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates( pos, smgr->getActiveCamera() );		

	//получим все террайны от сцены
	smgr->getSceneNodesFromType( scene::ESNT_TERRAIN, terrains, smgr->getRootSceneNode() );

	for( u32 cnt=0; cnt < terrains.size(); ++cnt )
	{
		//если у террайна есть селектор пересечений и было пересечение
		if( terrains[ cnt ]->getTriangleSelector() && smgr->getSceneCollisionManager()->getCollisionPoint( line, 
														terrains[ cnt ]->getTriangleSelector(), 
														collisionPos, 
														tri, outNode ) )
		{
			//то выходим из цикла обработки
			isCollision = true;
			break;
		}
		
	}

	lua_pushboolean( vm, isCollision ? 1 : 0 );
	lua_pushnumber( vm, collisionPos.X );
	lua_pushnumber( vm, collisionPos.Y );
	lua_pushnumber( vm, collisionPos.Z );
	
	return 4;
}

int CLuaSceneManager::GetMarkText( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSceneNode::GetMarkText not need any parameter");

	scene::ISceneNode* ptrNode = (scene::ISceneNode*)lua_touserdata( L, 2 );
	NrpText text("");

	scene::ISceneNode* node = GetTextSceneNode_( ptrNode );
	if( node != NULL )
		text = ((scene::ITextSceneNode*)node)->getText();

	lua_pushstring( L, text );

	return 1;
}

int CLuaSceneManager::SetMarkText( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaSceneManager::SetMarkText need string parameter");

	scene::ISceneNode* ptrNode = (scene::ISceneNode*)lua_touserdata( L, 2 );
	NrpText text( lua_tostring( L, 3 ) );
	assert( ptrNode != NULL );
	
	if( ptrNode == NULL )
		return 1;
	
	scene::ISceneNode* textNode = GetTextSceneNode_( ptrNode );
	IF_OBJECT_NOT_NULL_THEN
	{
		//при существующем ноде прислали пустую строку ( надо удалить текстнод из отрисовки )
		if( text.size() && textNode != NULL )
		{
			_object->addToDeletionQueue( textNode );
			return 1;
		}

		//есть какойто текст передан
		if( text != NULL )		
		{
			//и существует привязанный нод, то надо поменять текст у этого нода
			if( textNode != NULL )
				reinterpret_cast< scene::ITextSceneNode* >( textNode )->setText( text.ToWide() );
			else
			{
				//иначе надо создать новый текстнод и присвоить ему этот текст
				gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( "font_12" );
				_object->addTextSceneNode( font, text.ToWide(), 0xff000000, ptrNode, core::vector3df( 0, 0, 0 ) );
			}
		}


	}

	return 1;
}

scene::ISceneNode* CLuaSceneManager::GetTextSceneNode_( scene::ISceneNode* ptrNode )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		core::list< scene::ISceneNode* > childrens = ptrNode->getChildren();
		core::list< scene::ISceneNode* >::Iterator pIter = childrens.begin();

		for( ; pIter != childrens.end(); pIter++ )
			if( (*pIter)->getType() == scene::ESNT_TEXT )
				return (*pIter);
	}

	return NULL;
}

int CLuaSceneManager::AddToDeletionQueue( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneNode::AddToDeletionQueue need ISceneNode* parameter");

	scene::ISceneNode* ptrNode = (scene::ISceneNode*)lua_touserdata( vm, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->addToDeletionQueue( ptrNode );

	return 1;
}

int CLuaSceneManager::RenderScene( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaSceneManager:RenderScene not need any parameter");

	IF_OBJECT_NOT_NULL_THEN _object->drawAll();

	return 2;	
}

int CLuaSceneManager::LoadIrrlichtScene( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSceneNode::LoadIrrlichtScene need string parameter");

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		_object->loadScene( fileName );
	}

	return 1;	
}

int CLuaSceneManager::GetSceneNodeByID( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetSceneNodeByID need int parameter ");

	int id = lua_tointeger( vm, 2 );
	scene::ISceneNode* node = NULL;

	IF_OBJECT_NOT_NULL_THEN node = _object->getSceneNodeFromId( id );

	lua_pop( vm, argc );
	lua_pushlightuserdata( vm, node );
	Luna< CLuaSceneNode >::constructor( vm );

	return 1;
}

int CLuaSceneManager::SetSelectedNode( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSceneManager:SetSelectedNode need int parameter ");

	int id = lua_tointeger( L, 2 );
	scene::ISceneNode* node = NULL;

	_nrpEngine.GetCurrentScene()->SetSelectedNode( node );

	return 1;
}

int CLuaSceneManager::RemoveAllNodes( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaSceneManager:RemoveAllNodes not need any parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		_object->getRootSceneNode()->removeAll();
	}

	return 2;		
}

const char* CLuaSceneManager::ClassName()
{
	return ( CLASS_LUASCENEMANAGER );
}
}//namespace nrp