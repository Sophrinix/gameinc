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
											f32 time,
											bool visOnStop, 
											bool remSelf, 
											bool remParent )
: IGUIAnimator( environment, node ), 
  _time( time ), _visOnStop( visOnStop ),
  _remSelf( remSelf ), _remParent( remParent )
{
	_start = min( start, stop );			//выявим границы работы аниматора
	_stop = max( start, stop );
	_currentAlpha = static_cast< f32 >( start );				//запомним начальное значение прозрачности
	_step = abs( static_cast<f32>( start-stop ) ) / Environment->getVideoDriver()->getFPS();
	_step /= (_time/1000.f);
	_step *= start > stop ? -1 : 1;
}

void CNrpGuiBlendAnimator::draw()
{
	if( _currentAlpha >= _start && _currentAlpha <= _stop )  //проверяем границы работы аниматора
		_currentAlpha += _step;
	else												   //вышли за границы рабоыт
	{
		Parent->setVisible( _visOnStop );				   //установим значение видимости элемента	

		if( _remSelf )										//удалим аниматор если надо
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );
	
		if( _remParent )									//удалим родителя, если установлен такой флаг
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );

		return;
	}

	if( Parent )
		Parent->setAlphaBlend( static_cast<u32>( _currentAlpha ) );			//установим новое значение прозрачности элемента
}

}//namespace gui

}//namespace irr