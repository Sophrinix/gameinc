/********************************************************************
	created:	2009/11/27
	created:	27:11:2009   13:02
	classname: 	CNrpGuiBlendAnimator
	author:		Dalerank
*********************************************************************/
#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

//! Аниматор прозрачности элемента пользовательского интерфейса
class CNrpGuiBlendAnimator : public IGUIAnimator
{
public:
	//! Конструктор
	/*!
		\param environment	указатель на фабрику элементов
		\param node			родительский элемент	
		\param start		начальное значение прозрачности	
		\param stop			конечное значение прозрачности	
		\param step			величина изменения прозрачности на одном шаге	
		\param visOnStop	значение прозрачности элемента в конце работы	
		\param remSelf		удалить себя в конце работы
		\param remParent	удалить рожительский элемента в конце работы
	*/
	CNrpGuiBlendAnimator( IGUIEnvironment* environment, 
						  IGUIElement* node, 
						  u32 start, 
						  u32 stop, 
						  f32 time,
						  bool visOnStop, 
						  bool remSelf, 
						  bool remParent );

	~CNrpGuiBlendAnimator(void);

	//! цикл работы
	virtual void draw();
private:
	u32 _start,											/*!< миниммальная прозначность для остановки */
		_stop;											/*!< максимальная прозрачность элемента */
	f32 _time;											/*!< шаг изменения прозрачности */
	f32 _step;
	f32 _currentAlpha;

    s32 _sign;

	bool _visOnStop,									/*!< установить это значение видимости элемента в конце*/
		 _remSelf,										/*!< удалить себя в конце*/
		 _remParent;									/*!< удалить родительский элемент в конце*/

};

}//namspace gui

}//namespace irr
