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
	_start = min( start, stop );			//������ ������� ������ ���������
	_stop = max( start, stop );
	_currentAlpha = static_cast< f32 >( start );				//�������� ��������� �������� ������������
	_step = abs( static_cast<f32>( start-stop ) ) / Environment->getVideoDriver()->getFPS();
	_step /= (_time/1000.f);
	_step *= start > stop ? -1 : 1;
}

void CNrpGuiBlendAnimator::draw()
{
	if( _currentAlpha >= _start && _currentAlpha <= _stop )  //��������� ������� ������ ���������
        _currentAlpha += _step;

    bool mayDelete = false;
    if( _currentAlpha < _start )
    {
        _currentAlpha = _start;
        mayDelete = true;
    }

    if( _currentAlpha > _stop )
    {
        _currentAlpha = _stop;
        mayDelete = true;
    }
	
 	if( Parent )
        Parent->setAlphaBlend( static_cast<u32>( _currentAlpha ) );			//��������� ����� �������� ������������ ��������

    if( mayDelete )//����� �� ������� ������
    {
        Parent->setVisible( _visOnStop );				   //��������� �������� ��������� ��������	

        if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment ) )
        {
            if( _remSelf )										//������ �������� ���� ����
                env->addToDeletionQueue( this );

            if( _remParent )									//������ ��������, ���� ���������� ����� ����
                env->addToDeletionQueue( Parent );
        }

        return;
    }
}

}//namespace gui

}//namespace irr