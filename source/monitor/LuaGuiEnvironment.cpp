#include "StdAfx.h"
#include <IVideoDriver.h>
#include <assert.h>

#include "LuaGuiEnvironment.h"
#include "nrpGUIEnvironment.h"
#include "NrpGuiTimeDestructor.h"
#include "NrpGuiLigthing.h"
#include "NrpMiniMap.h"
#include "Nrptext.h"
#include "nrpGlobalMap.h"
#include "nrpChartCtrl.h"
#include "nrpGuiLinkBox.h"
#include "NrpTimer.h"
#include "LuaWindow.h"
#include "LuaEdit.h"
#include "LuaButton.h"
#include "LuaLabel.h"
#include "LuaImage.h"
#include "LuaComboBox.h"
#include "LuaLinkBox.h"
#include "LuaComponentListBox.h"
#include "LuaProgressBar.h"
#include "LuaPictureFlow.h"
#include "LuaTable.h"
#include "LuaListBox.h"
#include "LuaTechMap.h"
#include "LuaSceneNode.h"
#include "LuaTab.h"
#include "NrpGuiFlick.h"
#include "LuaElement.h"
#include "LuaFlick.h"
#include "layout/irrlayout.h"
#include "LuaAnimator.h"
#include "LuaConsole.h"
#include "NrpGuiLink.h"
#include "LuaLink.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAGUI( "CLuaGuiEnvironment" );

BEGIN_LUNA_METHODS( CLuaGuiEnvironment )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddWindow )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddBlenderAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddHoveredAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTextTimeAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddMiniMap )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddButton )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddScrollBar )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLuaAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetElementByID )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetElementByName )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddCircleScrollBar )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddRectAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddSpringAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTable )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTechMap )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddGlobalMap )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddChart )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddComboBox )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLabel )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddMoveAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddToDeletionQueue )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddEdit )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddImage )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddProgressBar )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTabControl )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTab )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLinkBox )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddCursorPosAnimator )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, RemoveAnimators )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddComponentListBox )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, MessageBox )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddListBox )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddPictureFlow )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, FadeAction )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddDestructor )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, BringToFront )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, SendToBack )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTimer )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLoopTimer )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTextRunner )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLigthing )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddFlick )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLayout )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTopElement )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, SetDragObject )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, SetCursor )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLink )
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetTextSize )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGuiEnvironment)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaGuiEnvironment )
	LUNA_AUTONAME_PROPERTY( CLuaGuiEnvironment, "focus", GetFocusedElement, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGuiEnvironment, "root", GetRootGUIElement, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGuiEnvironment, "dragObject", GetDragObject, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGuiEnvironment, "console", GetConsole, PureFunction )
END_LUNA_PROPERTIES

CLuaGuiEnvironment::CLuaGuiEnvironment(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUAGUI, ex )
{}

int CLuaGuiEnvironment::RemoveAnimators( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":RemoveAnimators not need any parameter") ) );

	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );

	IF_OBJECT_NOT_NULL_THEN _object->RemoveAnimators( elm );

	return 1;    
}

int CLuaGuiEnvironment::AddTable( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, _ErrStr(":AddTable need 7 parameter") );

	s32 id = lua_tointeger( vm, 6 );
	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 7, true );
	core::recti rectangle = _ReadRect( vm, 2, parentElem );

	gui::IGUITable* table = NULL;
	IF_OBJECT_NOT_NULL_THEN table = _object->addTable( rectangle, parentElem, id, false );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)table );
	Luna< CLuaTable >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::GetRootGUIElement( lua_State *vm )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIElement* elm = _object->getRootGUIElement();
		lua_pushlightuserdata( vm, (void*)elm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddWindow( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddWindow need 7 parameter");

	NrpText textureName = lua_tostring( vm, 2 );

	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parentElem = _GetLuaObject<gui::IGUIElement, ILuaObject>( vm, 8, true);
	core::recti rectangle = _ReadRect( vm, 3, parentElem );

	gui::IGUIWindow* window = NULL;
	IF_OBJECT_NOT_NULL_THEN 
	{
		video::ITexture* txs = NULL;

	    txs = _object->getVideoDriver()->getTexture( textureName );

		window = _object->addWindow( txs, rectangle, 0, parentElem, id );
		window->drop();
	}
	 
	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)window );
	Luna< CLuaWindow >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddBlenderAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddBlenderAnimator need 7 parameter");

	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	s32 minb = lua_tointeger( vm, 3 );
	s32 maxb = lua_tointeger( vm, 4 );
	f32 stepb = static_cast< f32 >( lua_tonumber( vm, 5 ));
	bool visibleOnEnd = lua_toboolean( vm, 6 ) > 0; 
	bool removeSelf = lua_toboolean( vm, 7 ) > 0;
	bool removeParent = lua_toboolean( vm, 8 ) > 0;

	IF_OBJECT_NOT_NULL_THEN _object->addBlendAnimator( parentElem, minb, maxb, stepb,
													   visibleOnEnd, removeSelf, removeParent );

	return 1;
}

int CLuaGuiEnvironment::AddHoveredAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddHoveredAnimator need 7 parameter");

	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	u32 minb = lua_tointeger( vm, 3 );
	u32 maxb = lua_tointeger( vm, 4 );
	u32 stepb = lua_tointeger( vm, 5 );
	bool visibleOnEnd = lua_toboolean( vm, 6 ) > 0; 
	bool removeSelf = lua_toboolean( vm, 7 ) > 0;
	bool removeParent = lua_toboolean( vm, 8 ) > 0;

	IF_OBJECT_NOT_NULL_THEN _object->addHoveredAnimator( parentElem, minb, maxb, stepb,
														 visibleOnEnd, removeSelf, removeParent );

	return 1;
}


int CLuaGuiEnvironment::AddRectAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 11, 11, "Function CLuaGuiEnvironment:AddRectAnimator need 4 parameter");

	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	core::recti minr, maxr;
	minr = _ReadRect( vm, 3, parentElem );
	maxr = _ReadRect( vm, 7, parentElem );
	s32 step = lua_tointeger( vm, 11 );

	gui::IGUIAnimator* anim = NULL; 
	
	IF_OBJECT_NOT_NULL_THEN
	{
		anim = _object->addRectAnimator( parentElem, minr, maxr, step );
		lua_pushlightuserdata( vm, (void*)anim );
		Luna< CLuaAnimator >::constructor( vm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddLuaAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaGuiEnvironment:AddLuaAnimator need 2 parameter");

	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	int funcRef = _GetRef( vm, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIAnimator* anim = _object->addLuaAnimator( parentElem, funcRef );
		lua_pushlightuserdata( vm, anim );
		Luna< CLuaAnimator >::constructor( vm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddMiniMap( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddMiniMap need 6 parameter");

	s32 id = lua_tointeger( vm, 6 );
	scene::ISceneNode* parentNode = _GetLuaObject< scene::ISceneNode, CLuaSceneNode >( vm, 7, false );
	core::recti rectangle = _ReadRect( vm, 2, NULL );

	gui::CNrpMiniMap* mp = NULL;
	
	IF_OBJECT_NOT_NULL_THEN mp = new gui::CNrpMiniMap( rectangle, _object, id, parentNode );

	lua_pushlightuserdata( vm, (void*)mp );

	return 1;
}

int CLuaGuiEnvironment::AddButton( lua_State *vm )
{
	int argc = lua_gettop(vm);

	gui::IGUIElement* parent = NULL;
	core::recti rec;
	NrpText text;
	s32 id = -1;
	int action = -1;
	if( argc == 8 )
	{
		luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddButton need 7 parameter");
		parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 6, true );
		rec = _ReadRect( vm, 2, parent );
		id = lua_tointeger( vm, 7 );
		text = lua_tostring( vm, 8 );
	}
	else if( argc == 4 )
	{
		luaL_argcheck(vm, argc == 4, 4, "Function CLuaGuiEnvironment:AddButton need 2 parameter");
		parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 3, true );
		text = lua_tostring( vm, 2 );
		action = _GetRef( vm, 4 );
	}

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIButton* elm = _object->addButton( rec,  parent, id, text.ToWide() );
		if( action > 0 )
			dynamic_cast< gui::CNrpButton* >( elm )->setOnClickAction( action );
		//lua_pop( vm, argc );
		lua_pushlightuserdata( vm, (void*)elm );
		Luna< CLuaButton >::constructor( vm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddScrollBar( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddScrollBar need 7 parameter");

	bool gorizontal = lua_toboolean( vm, 2 ) > 0;

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 7, true );
	core::recti rectangle = _ReadRect( vm, 3, parent );
	s32 id = lua_tointeger( vm, 8 );

	gui::IGUIElement* elm = NULL;
	
	IF_OBJECT_NOT_NULL_THEN elm = _object->addScrollBar( gorizontal, rectangle,  parent, id );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::GetElementByID( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetElementByID need one parameter int ");

	int id = lua_tointeger(vm, 2);	

	gui::IGUIElement* sg = NULL;
	
	IF_OBJECT_NOT_NULL_THEN sg = _object->getRootGUIElement()->getElementFromId( id, true );
	lua_pushlightuserdata( vm, (void*)sg );

	return 1;
}

int CLuaGuiEnvironment::GetElementByName( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetElementByName need one parameter string ");

	NrpText name( lua_tostring(vm, 2) );	
	
	gui::IGUIElement* sg = NULL;	
	IF_OBJECT_NOT_NULL_THEN sg = _object->getRootGUIElement()->getChildenFromName( name.ToWide(), true );
	
	lua_pushlightuserdata( vm, (void*)sg );
	return 1;
}

int CLuaGuiEnvironment::AddCircleScrollBar( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaSceneManager:GetElementByID need 6 parameter ");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 6, true );
	core::recti rectangle = _ReadRect( vm, 2, parent );
	s32 id = lua_tointeger( vm, 7 );

	gui::IGUIScrollBar* scrb = NULL;

	IF_OBJECT_NOT_NULL_THEN scrb = _object->addCicrcleScrollBar( parent, id, rectangle );

	lua_pushlightuserdata( vm, (void*)scrb );

	return 1;
}

int CLuaGuiEnvironment::AddGlobalMap( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 6, 6, "Function CLuaSceneManager:GetElementByID need 5 parameter ");

	core::recti rectangle = _ReadRect( vm, 2, NULL );

	s32 id = lua_tointeger( vm, 6 );

	gui::CNrpGloablMap* gb = NULL;
	
	IF_OBJECT_NOT_NULL_THEN gb = new gui::CNrpGloablMap( rectangle, _object, id );

	lua_pushlightuserdata( vm, (void*)gb );

	return 1;
}

int CLuaGuiEnvironment::AddChart( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaSceneManager:AddChart need 6 parameter ");

	s32 id = lua_tointeger( vm, 6 );
	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 7, true );
	core::recti rectangle = _ReadRect( vm, 2, elm );

	gui::CChartCtrl* chart = NULL;

	IF_OBJECT_NOT_NULL_THEN chart = new gui::CChartCtrl( _object, elm, id, rectangle );

	lua_pushlightuserdata( vm, (void*)chart );

	return 1;
}

int CLuaGuiEnvironment::AddComboBox( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddComboBox need 7 parameter");

	NrpText texturepath = lua_tostring( vm, 2 );

	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 8, true );	
	core::recti rectangle = _ReadRect( vm, 3, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addComboBox( rectangle, parent, id );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaComboBox >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddEdit( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddEdit need 7 parameter");

	NrpText text = lua_tostring( vm, 2 );


	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = _GetLuaObject<gui::IGUIElement, ILuaObject>( vm, 8, true);	
	core::recti rectangle = _ReadRect( vm, 3, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addEditBox( text.ToWide(), rectangle, true, parent, id );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaEdit >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddLabel( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddLabel need 7 parameter");

	NrpText text( lua_tostring( vm, 2 ) );
	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = _GetLuaObject<gui::IGUIElement, ILuaObject>( vm, 8, true );	
	core::recti rectangle = _ReadRect( vm, 3, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addStaticText( text.ToWide(), rectangle, false, true, parent, id, false );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaLabel >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddMoveAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddMoveAnimator need 7 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );	
	assert( parent );
	if( parent )
	{
		core::position2di pos;
		core::dimension2di size = parent->getParent() ? parent->getParent()->getAbsolutePosition().getSize() : core::dimension2di( 0, 0 );
		pos.X = _ReadParam( vm, 3, size.Width, 0 );
		pos.Y = _ReadParam( vm, 4, size.Height, 0 );

		int time = lua_tointeger( vm, 5 );
		bool visibleOnStop = lua_toboolean( vm, 6 ) > 0;
		bool removeOnStop = lua_toboolean( vm, 7 ) > 0;
		bool removeParentOnStop =  lua_toboolean( vm, 8 ) > 0;

		gui::IGUIAnimator* anim = NULL;

		IF_OBJECT_NOT_NULL_THEN anim = _object->addMoveAnimator( parent, pos, time, 
																 visibleOnStop,
																 removeOnStop,
																 removeParentOnStop );

		lua_pushlightuserdata( vm, (void*)anim );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddToDeletionQueue( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaGuiEnvironment:AddToDeletionQueue need 2 parameter");

	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );	

	IF_OBJECT_NOT_NULL_THEN _object->addToDeletionQueue( elm );

	return 1;
}

int CLuaGuiEnvironment::AddImage( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddImage need 7 parameter");
	
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 6, true );	
	core::recti rectangle = _ReadRect( vm, 2, parent );
	s32 id = lua_tointeger( vm, 7 );
	NrpText text( lua_tostring( vm, 8 ) );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addImage( rectangle, parent, id, text.ToWide() );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaImage >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddTextTimeAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaGuiEnvironment:AddTextTimeAnimator need 1 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );	

	gui::IGUIAnimator* anim = NULL;

	IF_OBJECT_NOT_NULL_THEN anim = _object->addTextTimeAnimator( parent );

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::AddDestructor( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaGuiEnvironment:AddDestructor need 2 parameter");

	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );	
	int time = lua_tointeger( vm, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->AddDestructor( elm, time );

	return 1;
}

int CLuaGuiEnvironment::AddProgressBar( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddProgressBar need 6 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	core::recti rectangle = _ReadRect( vm, 3, parent );
	s32 iid = lua_tointeger( vm, 7 );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addProgressBar( parent, iid, rectangle );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaProgressBar >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddTabControl( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddTabControl need 6 parameter");

	s32 iid = lua_tointeger( vm, 6 );
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 7, true );	
	core::recti rectangle = _ReadRect( vm, 2, parent );
	
	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = _object->addTabControl( rectangle, parent, true, true, iid );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddTab( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 4, 4, "Function CLuaGuiEnvironment:AddTab need 3 parameter");

	gui::IGUITabControl* parent = _GetLuaObject< gui::IGUITabControl, ILuaObject >( vm, 2, true );	
	NrpText name( lua_tostring( vm, 3 ) ); 
	int id = lua_tointeger( vm, 4 );

	gui::IGUITab* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = parent->addTab( name.ToWide(), id );

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaTab >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddLinkBox( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddLinkModule need 7 parameter");

	NrpText name( lua_tostring( vm, 2 ) );
	s32 iid = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = NULL;
	parent = _GetLuaObject<gui::IGUIElement, ILuaObject>( vm, 8, true );
	core::recti rectangle = _ReadRect( vm, 3, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		elm = _object->addLinkBox( parent, iid, rectangle );
		if( elm )
			elm->setText( name.ToWide() );
	}

	//lua_pop( vm, argc );
	lua_pushlightuserdata( vm, (void*)elm );
	Luna< CLuaLinkBox >::constructor( vm );

	return 1;
}

int CLuaGuiEnvironment::AddCursorPosAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 4, 4, "Function CLuaGuiEnvironment:AddCursorPosAnimator need 3 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	core::position2di offset( lua_tointeger( vm, 3), lua_tointeger( vm, 4 ) );

	gui::IGUIAnimator* anim = NULL;

	IF_OBJECT_NOT_NULL_THEN anim = _object->addCursorPosAnimator( parent, offset );

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::GetFocusedElement( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIElement* elm = _object->getFocus();
		lua_pushlightuserdata( L, (void*)elm );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGuiEnvironment::SetDragObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaSceneManager:SetDragObject need 2 parameter");

	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );
		assert( lua_isstring( L, 3 ) );
		NrpText texture = lua_tostring( L, 3 );
		video::ITexture* rr = _object->getVideoDriver()->getTexture( texture );
		_object->setDragObject( elm, rr );
	}

	return 0;
}

int CLuaGuiEnvironment::GetDragObject( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIElement* elm = _object->getDragObject();
		lua_pushlightuserdata( L, (void*)elm );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGuiEnvironment::AddComponentListBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 7, 7, "Function CLuaSceneManager:AddComponentList need 7 parameter");

	s32 iid = lua_tointeger( L, 6 );
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 7, true );	
	core::recti rectangle = _ReadRect( L, 2, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN	elm = _object->addComponentListBox( rectangle, parent, iid );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, elm );
	Luna< CLuaComponentListBox >::constructor( L );

	return 1;
}

int CLuaGuiEnvironment::AddListBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 7, 7, "Function CLuaSceneManager:AddComponentList need 7 parameter");

	s32 iid = lua_tointeger( L, 6 );
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 7, true );	
	core::recti rectangle = _ReadRect( L, 2, parent );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN	elm = _object->addListBox( rectangle, parent, iid );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, elm );
	Luna< CLuaListBox >::constructor( L );

	return 1;
}

int CLuaGuiEnvironment::MessageBox( lua_State* L )
{
	int argc = lua_gettop(L);
	core::array< int > funcs; 
	luaL_argcheck(L, argc == 6, 6, "Function CLuaSceneManager:MessageBox need 6 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	bool btnYesVisible = lua_toboolean( L, 3 )>0;
	bool btnNoVisible = lua_toboolean( L, 4 )>0;

	funcs.push_back( _GetRef( L, 5 ) );
	funcs.push_back( _GetRef( L, 6 ) );

	int flags = (btnYesVisible ? gui::EMBF_YES : 0) | (btnNoVisible ? gui::EMBF_NO : 0 );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIWindow* wnd = _object->addMessageBox( text.ToWide(), flags, funcs );
		if( gui::CNrpWindow* nrpw = dynamic_cast< gui::CNrpWindow* >( wnd ) )
		{
			nrpw->setModal();
			nrpw->getCloseButton()->setVisible( false );

			lua_pushlightuserdata( L, wnd );
			Luna< CLuaWindow >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaGuiEnvironment::AddPictureFlow( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 7, 7, "Function CLuaSceneManager:AddPictureFlow need 6 parameter");

	s32 iid = lua_tointeger( L, 6 );
	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 7, true);	
	core::recti rectangle = _ReadRect( L, 2, parent );
	u32 minside = min( rectangle.getWidth(), rectangle.getHeight() );
	core::recti pictureRect( core::position2di( 0, 0 ), core::dimension2du( minside, minside ) );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIElement* elm = (gui::IGUIElement*)_object->addPictureFlow( rectangle, pictureRect, iid, parent );
		lua_pushlightuserdata( L, elm );
		Luna< CLuaPictureFlow >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaGuiEnvironment::FadeAction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaGuiEnvironment:FadeAction need int, bool, bool parameter" );

	int time = lua_tointeger( L, 2 );
	assert( time > 0 && time < 10000 );
	bool inaction = lua_toboolean( L, 3 ) > 0;
	bool needDestruct = lua_toboolean( L, 4 ) > 0;

	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIInOutFader* fader = (gui::IGUIInOutFader*)_object->getRootGUIElement()->getElementFromId( 2002002 );
		if( fader == NULL )
			fader = _object->addInOutFader(0, _object->getRootGUIElement(), 2002002 );

		if( inaction )
			fader->fadeIn( time );
		else
			fader->fadeOut( time );

		if( fader->getParent() )
			fader->getParent()->bringToFront( fader );
	}

	return 0;
}

int CLuaGuiEnvironment::AddTechMap( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function  CLuaGuiEnvironment:AddTechMap need 7 parameter");

	s32 id = lua_tointeger( vm, 6 );
	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 7, true );
	core::recti rectangle = _ReadRect( vm, 2, parentElem );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::CNrpTechMap* techMap = _object->AddTechMap( rectangle, parentElem, id, false );
		lua_pushlightuserdata( vm, techMap );
		Luna< CLuaTechMap >::constructor( vm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::SendToBack( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGuiEnvironment:SendToBack need element parameter" );

	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );
	assert( elm != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIElement* parent = elm->getParent();
		if( parent)
			parent->sendToBack( elm );
	}

	return 0;
}

int CLuaGuiEnvironment::BringToFront( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGuiEnvironment:BringToFront need element parameter" );

	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );
	assert( elm != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIElement* parent = elm->getParent();
		if( parent)
			parent->bringToFront( elm );
	}

	return 0;
}

int CLuaGuiEnvironment::AddTimer( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaGuiEnvironment:AddTimer need int, bool parameter" );

	int time = lua_tointeger( L, 2 );
	assert( lua_isfunction( L, 3 ) );
	int action = _GetRef( L, 3 );
	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 4 );

	gui::CNrpTimer* timer = NULL;
	IF_OBJECT_NOT_NULL_THEN 
	{
		timer = new gui::CNrpTimer( _object, elm ? elm : _object->getRootGUIElement(), time, action );
	}

	lua_pushlightuserdata( L, timer );
	return 1;
}

int CLuaGuiEnvironment::AddTextRunner( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaGuiEnvironment:AddTextRunner need 2 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	NrpText text( lua_tostring( vm, 3 ) );

	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIAnimator* anim = _object->addTextRunnerAnimator( parent, text.ToWide() );
		lua_pushlightuserdata( vm, (void*)anim );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::AddLigthing( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaGuiEnvironment:AddLigthing need 2 parameter");

	gui::IGUIElement* p1 = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );
	gui::IGUIElement* p2 = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 3, true );
	assert( p1 );
	NrpText textureName( lua_tostring( L, 4 ) );
	int timeToDeath = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		video::ITexture* txs = NULL;

		txs = _object->getVideoDriver()->getTexture( textureName );
		gui::CNrpGuiLigthing* lg = new gui::CNrpGuiLigthing( _object, p1, p2, -1, txs, timeToDeath );
		lua_pushlightuserdata( L, lg );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

const char* CLuaGuiEnvironment::ClassName()
{
	return ( CLASS_LUAGUI );
}

int CLuaGuiEnvironment::AddFlick( lua_State* L )
{
	int argc = lua_gettop( L );
	luaL_argcheck( L, argc == 8, 8, "Function  CLuaGuiEnvironment:AddTechMap need 7 parameter");

	s32 id = lua_tointeger( L, 7 );
	u32 column = static_cast< u32 >( lua_tointeger( L, 6 ) );
	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 8, true );
	core::recti rectangle = _ReadRect( L, 2, parentElem );

	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::CNrpGuiFlick* elm = new gui::CNrpGuiFlick( _object, parentElem, rectangle, column, id );
		lua_pushlightuserdata( L, elm );
		Luna< CLuaFlick >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGuiEnvironment::AddLayout( lua_State* L )
{
	int argc = lua_gettop( L );
	luaL_argcheck( L, argc == 8, 8, "Function  CLuaGuiEnvironment:AddLayout need 7 parameter");

	s32 id = lua_tointeger( L, 7 );
	u32 column = static_cast< u32 >( lua_tointeger( L, 6 ) );
	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 8, true );
	core::recti rectangle = _ReadRect( L, 2, parentElem );

	gui::CNrpLayout* elm = NULL;
	IF_OBJECT_NOT_NULL_THEN elm = new gui::CNrpLayout( _object, parentElem, rectangle, column, id );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, elm );
	Luna< CLuaElement >::constructor( L );

	return 1;
}

int CLuaGuiEnvironment::AddTopElement( lua_State* L )
{
	int argc = lua_gettop( L );
	luaL_argcheck( L, argc == 2, 2, "Function  CLuaGuiEnvironment:AddTopElement need IGuiElement parameter");

	IF_OBJECT_NOT_NULL_THEN 
	{
		gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 2, true );
		assert( elm );
		if( gui::CNrpGUIEnvironment* mg = dynamic_cast< gui::CNrpGUIEnvironment* >( _object ) )
		{
			mg->AddTopElement( elm );	
		}
	}

	return 0;
}

int CLuaGuiEnvironment::AddLoopTimer( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaGuiEnvironment:AddTimer need int, bool parameter" );

	int time = lua_tointeger( L, 2 );
	assert( lua_isfunction( L, 3 ) );

	int action = _GetRef( L, 3 );
	gui::IGUIElement* elm = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 4, true );

	gui::CNrpTimer* timer = NULL;
	IF_OBJECT_NOT_NULL_THEN 
	{
		timer = new gui::CNrpTimer( _object, elm, time, action );
		timer->SetLoop( true );

		lua_pushlightuserdata( L, timer );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGuiEnvironment::AddSpringAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 11, 11, "Function CLuaGuiEnvironment:AddSpringAnimator need 10 parameter");

	gui::IGUIElement* parentElem = _GetLuaObject< gui::IGUIElement, ILuaObject >( vm, 2, true );
	core::recti minr, maxr;
	minr = _ReadRect( vm, 3, parentElem );
	maxr = _ReadRect( vm, 7, parentElem );
	s32 step = lua_tointeger( vm, 11 );

	gui::IGUIAnimator* anim = NULL; 

	IF_OBJECT_NOT_NULL_THEN
	{
		anim = _object->addSpringAnimator( parentElem, minr, maxr, step );
		lua_pushlightuserdata( vm, (void*)anim );
		Luna< CLuaAnimator >::constructor( vm );
		return 1;
	}

	lua_pushnil( vm );
	return 1;
}

int CLuaGuiEnvironment::SetCursor( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaGuiEnvironment:SetCursor need path to texture");

	NrpText text( lua_tostring( vm, 2 ) );

	IF_OBJECT_NOT_NULL_THEN 
	{
		//
	}

	return 0;
}

int CLuaGuiEnvironment::GetConsole( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		gui::CNrpConsole* console = _nrpEngine.GetConsole();
		if( console )
		{
			lua_pushlightuserdata( L, console );
			Luna< CLuaConsole >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGuiEnvironment::AddLink( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 9, 9, "Function CLuaGuiEnvironment:AddButton need 7 parameter");

	gui::IGUIElement* parent = _GetLuaObject< gui::IGUIElement, ILuaObject >( L, 6, true );
	core::recti rec = _ReadRect( L, 2, parent );
	s32 id = lua_tointeger( L, 7 );
	NrpText text = lua_tostring( L, 8 );
	int action = _GetRef( L, 9 );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::CNrpGuiLink* elm = _object->addLink( rec,  parent, id, text.ToWide() );
		elm->setOnClickAction( action );
		//lua_pop( vm, argc );
		lua_pushlightuserdata( L, (void*)elm );
		Luna< CLuaGuiLink >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaGuiEnvironment::GetTextSize( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaGuiEnvironment:GetTextWidth need fontName, string as parameter");
	
	NrpText fontName = lua_tostring( L, 2 );
	NrpText text = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIFont* font = _object->getFont( fontName );
		
		if( font )
		{
			core::dimension2du size = font->getDimension( text );

			lua_pushinteger( L, size.Width );
			lua_pushinteger( L, size.Height );
			return 2;
		}
	}

	lua_pushnil( L );
	lua_pushnil( L );
	return 2;
}

}//namespace nrp
