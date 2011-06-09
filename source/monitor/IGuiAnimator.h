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

//! Тип элемента
const int EGUIET_ANIMATOR=EGUIET_COUNT+2;

//! Аниматор отвечает за изменение прозрачности элемента, к которому поодуключен
class IGUIAnimator : public IGUIElement, public ILuaFunctionality
{
public:
	//! Коструктор
	/*! 
		\param environment указатель на фабрику элементов
		\param parent элемента, для которого создается аниматор
	*/
	IGUIAnimator( IGUIEnvironment* environment, 
				  IGUIElement* parent )
				  : IGUIElement( EGUI_ELEMENT_TYPE( EGUIET_ANIMATOR ), environment, parent, -1, core::recti( 0, 0, 0, 0 ) )
	{

	}

	//! Деструктор
	virtual ~IGUIAnimator(void)
	{
		PCall( GUIELEMENT_ON_REMOVE, this );
	}

	//! Тип элемента
	const c8* getTypeName() const
	{
		return "GuiAnimator";
	}
};

}//namspace gui

}//namespace irr
