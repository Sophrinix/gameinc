/********************************************************************
	created:	2009/11/27
	created:	27:11:2009   13:02
	classname: 	CNrpGuiBlendAnimator
	author:		Dalerank
*********************************************************************/
#pragma once
#include "iguielement.h"
#include "LuaFunctionality.h"

namespace irr
{

namespace gui
{

//! ��� ��������
const int EGUIET_ANIMATOR=EGUIET_COUNT+2;

//! �������� �������� �� ��������� ������������ ��������, � �������� �����������
class IGUIAnimator : public IGUIElement, public ILuaFunctionality
{
public:
	//! ����������
	/*! 
		\param environment ��������� �� ������� ���������
		\param parent ��������, ��� �������� ��������� ��������
	*/
	IGUIAnimator( IGUIEnvironment* environment, 
				  IGUIElement* parent )
				  : IGUIElement( EGUI_ELEMENT_TYPE( EGUIET_ANIMATOR ), environment, parent, -1, core::recti( 0, 0, 0, 0 ) )
	{

	}

	//! ����������
	virtual ~IGUIAnimator(void)
	{
		PCall( GUIELEMENT_ON_REMOVE, this );
	}

	//! ��� ��������
	const c8* getTypeName() const
	{
		return "GuiAnimator";
	}
};

}//namspace gui

}//namespace irr
