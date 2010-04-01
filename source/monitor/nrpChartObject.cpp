#include "stdafx.h"
#include "nrpChartObject.h"

namespace irr
{

namespace gui
{

CChartObject::CChartObject(IGUIEnvironment* env, CChartCtrl* ptrOwner)
{
	rectangle_ = core::recti( 0, 0, 0, 0 );
	color_ = video::SColor( 0xff, 0, 0, 0);
	parent_ = ptrOwner;
	visible_ = true;
	environment_ = env;
}

CChartObject::~CChartObject()
{
}

void CChartObject::SetVisible(bool visible)
{ 
	visible_ = visible; 

}

void CChartObject::SetColor( video::SColor newColor)	   
{ 
	color_ = newColor; 
}

bool CChartObject::Clip( int& dX, int& dY ) const
{
	bool result = true;

	if (dX>rectangle_.UpperLeftCorner.X) 
	{
		dX = rectangle_.UpperLeftCorner.X;
		result = false;
	}
	if (dX<rectangle_.UpperLeftCorner.X)
	{
		dX = rectangle_.UpperLeftCorner.X;
		result = false;
	}
	if (dY>rectangle_.LowerRightCorner.Y)
	{
		dY = rectangle_.LowerRightCorner.Y;
		result = false;
	}
	if (dY<rectangle_.UpperLeftCorner.Y)
	{
		dY = rectangle_.UpperLeftCorner.Y;
		result = false;
	}

	return result;
}

void CChartObject::SetRect( const core::recti newTect )
{
	rectangle_ = newTect;
}
}//namespace gui

}//namespace irr