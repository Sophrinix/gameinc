#include "StdAfx.h"
#include "NrpGuiBlendAnimator.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

CNrpGuiBlendAnimator::~CNrpGuiBlendAnimator(void)
{
}

CNrpGuiBlendAnimator::CNrpGuiBlendAnimator( IGUIEnvironment* environment, 
											IGUIElement* node, 
											u32 start, 
											u32 stop, 
											f32 step,
											bool visOnStop, 
											bool remSelf, 
											bool remParent )
: IGUIAnimator( environment, node ), 
  step_( step ), visOnStop_( visOnStop ),
  remSelf_( remSelf ), remParent_( remParent )
{
	start_ = min( start, stop );			//выявим границы работы аниматора
	stop_ = max( start, stop );
	currentAlpha_ = (f32)start;				//запомним начальное значение прозрачности
}

void CNrpGuiBlendAnimator::draw()
{
	if( currentAlpha_ > start_ && currentAlpha_ < stop_ )  //проверяем границы работы аниматора
		currentAlpha_ += step_;
	else												   //вышли за границы рабоыт
	{
		Parent->setVisible( visOnStop_ );				   //установим значение видимости элемента	

		if( remSelf_ )										//удалим аниматор если надо
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );
	
		if( remParent_ )									//удалим родителя, если установлен такой флаг
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );

		return;
	}

	Parent->setAlphaBlend( (u32)currentAlpha_ );			//установим новое значение прозрачности элемента
}

}//namespace gui

}//namespace irr