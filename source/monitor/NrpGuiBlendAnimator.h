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

//! �������� ������������ �������� ����������������� ����������
class CNrpGuiBlendAnimator : public IGUIAnimator
{
public:
	//! �����������
	/*!
		\param environment	��������� �� ������� ���������
		\param node			������������ �������	
		\param start		��������� �������� ������������	
		\param stop			�������� �������� ������������	
		\param step			�������� ��������� ������������ �� ����� ����	
		\param visOnStop	�������� ������������ �������� � ����� ������	
		\param remSelf		������� ���� � ����� ������
		\param remParent	������� ������������ �������� � ����� ������
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

	//! ���� ������
	virtual void draw();
private:
	u32 _start,											/*!< ������������ ������������ ��� ��������� */
		_stop;											/*!< ������������ ������������ �������� */
	f32 _time;											/*!< ��� ��������� ������������ */
	f32 _step;
	f32 _currentAlpha;

    s32 _sign;

	bool _visOnStop,									/*!< ���������� ��� �������� ��������� �������� � �����*/
		 _remSelf,										/*!< ������� ���� � �����*/
		 _remParent;									/*!< ������� ������������ ������� � �����*/

};

}//namspace gui

}//namespace irr
