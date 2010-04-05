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
	start_ = min( start, stop );			//������ ������� ������ ���������
	stop_ = max( start, stop );
	currentAlpha_ = (f32)start;				//�������� ��������� �������� ������������
}

void CNrpGuiBlendAnimator::draw()
{
	if( currentAlpha_ > start_ && currentAlpha_ < stop_ )  //��������� ������� ������ ���������
		currentAlpha_ += step_;
	else												   //����� �� ������� ������
	{
		Parent->setVisible( visOnStop_ );				   //��������� �������� ��������� ��������	

		if( remSelf_ )										//������ �������� ���� ����
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( this );
	
		if( remParent_ )									//������ ��������, ���� ���������� ����� ����
			dynamic_cast< CNrpGUIEnvironment* >( Environment )->addToDeletionQueue( Parent );

		return;
	}

	Parent->setAlphaBlend( (u32)currentAlpha_ );			//��������� ����� �������� ������������ ��������
}

}//namespace gui

}//namespace irr