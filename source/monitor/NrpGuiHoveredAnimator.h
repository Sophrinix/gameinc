#pragma once
#include "IGuiAnimator.h"

namespace irr
{

namespace gui
{

//! �������� �������� ��������������� ����������
/*! 
	�������� ������������ �������� ��� ��������� 
	�������
*/
class CNrpGuiHoveredAnimator : public IGUIAnimator
{
public:
	CNrpGuiHoveredAnimator( IGUIEnvironment* environment, 
							IGUIElement* node, 
							u32 min_blend, 
							u32 max_blend, 
							u32 step, 
							bool visOnStop, 
							bool remSelf,
							bool remParent );
	~CNrpGuiHoveredAnimator(void);

	virtual void draw();
private:
	u32 min_,											//! ������������ ������������ ��� ���������
		max_,											//! ������������ ������������ ��������
		step_;											//! ��� ��������� ������������

	bool visOnStop_; 
	bool remSelf_;
	bool remParent_;
};

}//namspace gui

}//namespace irr