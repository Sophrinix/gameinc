#pragma once

#include "ILuaObject.h"
namespace irr
{
	namespace gui
	{
		class CNrpGUIEnvironment;
		class CNrpTechMap;
	}
}

namespace nrp
{
//! �����-������� ������� ����������������� ����������
/*!
	����� ���-������� ��������� � ������� ������� ������
*/
class CLuaGuiEnvironment : public ILuaObject<irr::gui::CNrpGUIEnvironment>
{
public:
	static Luna<CLuaGuiEnvironment>::RegType methods[];

	//! ����������� 
	/*!
		\param ptr ��������� �� ���������������� ������, ������� ������������� � ��������
	*/
	CLuaGuiEnvironment(lua_State *L);
	//! ������� ���������� � ���-������ ������� �������
	/*! � ������� ���������� ���*/
	int GetRootGUIElement( lua_State *vm );

	//! ������� ����� ���� �� �������
	/*! \param texturePath ���� � �������� ��������
		\param xb, yb, xe, ye ��������� � �������� ����� �������������� ����
		\param id ������������� ����
		\param parent ������������ �������
	*/
	int AddWindow( lua_State *vm );

	//! ������� �������� ������������ ��������
	/*! \param parentElem ������������ ������� 
		\param minb		  ��������� �������� ������������
		\param maxb		  �������� �������� ������������
		\param stepb	  �������� ��������� ������������	
		\param visibleOnEnd �������� ��������� �������� ��� ���������� ������
		\param removeSelf	���� ������������ ��������� ��� ���������� ������
		\param removeParent	���� �������� ������������� �������� ��� ���������� ������
	*/
	int AddBlenderAnimator( lua_State *vm );
	int AddHoveredAnimator( lua_State *vm );
	int AddTextTimeAnimator( lua_State *vm );
	int AddMiniMap( lua_State *vm );
	int AddButton( lua_State *vm );
	int AddScrollBar( lua_State *vm );
	int AddCircleScrollBar( lua_State *vm );
	int AddProgressBar( lua_State* vm );
	int AddLuaAnimator( lua_State *vm );
	int GetElementByID( lua_State* vm );
	int GetElementByName( lua_State* vm );
	int AddRectAnimator( lua_State *vm );
	int AddTable( lua_State *vm );
	int AddTechMap( lua_State *vm );
	int AddGlobalMap( lua_State* vm );
	int AddChart( lua_State* vm );
	int AddComboBox( lua_State* vm );
	int AddLabel( lua_State* vm );
	int AddMoveAnimator( lua_State* vm );
	int AddToDeletionQueue( lua_State* vm );
	int AddEdit( lua_State* vm );
	int AddImage(  lua_State* vm ); 
	int AddTabControl( lua_State* vm );
	int AddTab( lua_State* vm );
	int AddTimer( lua_State* vm );
	int AddLinkBox( lua_State* vm );
	int AddCursorPosAnimator( lua_State* vm );
	int RemoveAnimators( lua_State* L );
	int GetFocusedElement( lua_State* L );
	int SetDragObject( lua_State* L );
	int GetDragObject( lua_State* L );
	int AddComponentListBox( lua_State* L );
	int MessageBox( lua_State* L );
	int AddListBox( lua_State* L );
	int AddPictureFlow( lua_State* L );
	int FadeAction( lua_State* L );
	int AddDestructor( lua_State* vm );
	int BringToFront( lua_State* L );
	
	static const char* StaticGetLuaName() { return "CLuaGuiEnvironment"; }	
};

}//namespace nrp