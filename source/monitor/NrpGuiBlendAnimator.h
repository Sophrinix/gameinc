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
						  f32 step,
						  bool visOnStop, 
						  bool remSelf, 
						  bool remParent );

	~CNrpGuiBlendAnimator(void);

	//! ���� ������
	virtual void draw();
private:
	u32 start_,											/*!< ������������ ������������ ��� ��������� */
		stop_;											/*!< ������������ ������������ �������� */
	f32 step_;											/*!< ��� ��������� ������������ */
	f32 currentAlpha_;

	bool visOnStop_,									/*!< ���������� ��� �������� ��������� �������� � �����*/
		 remSelf_,										/*!< ������� ���� � �����*/
		 remParent_;									/*!< ������� ������������ ������� � �����*/

};

}//namspace gui

}//namespace irr
