#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

//! Аниматор элемента пользоватеского интерфейса
/*! 
	Изменяет прозрачность элемента при наведении 
	курсора
*/
class CNrpGuiHoveredAnimator : public IGUIAnimator
{
public:
	CNrpGuiHoveredAnimator( IGUIEnvironment* environment, 
							IGUIElement* node, 
							u32 min_blend, 
							u32 max_blend, 
							u32 time, 
							bool visOnStop, 
							bool remSelf,
							bool remParent );
	~CNrpGuiHoveredAnimator(void);

	virtual void draw();
private:
	u32 min_,											//! миниммальная прозначность для остановки
		max_,											//! максимальная прозрачность элемента
		_time;
    
    float _currentAlpha;//! шаг изменения прозрачности

	bool visOnStop_; 
	bool remSelf_;
	bool remParent_;
};

}//namspace gui

}//namespace irr