#include "stdafx.h"
#include "nrpChartGrid.h"
#include "nrpChartAxis.h"

using namespace std;

namespace irr
{

namespace gui
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartGrid::CChartGrid( IGUIEnvironment* env, CChartCtrl* pParent, bool bHoriz) 
  : CChartObject(env, pParent), horizontal_(bHoriz), tickPos_()
{
	color_ = video::SColor( 25,0,0,0 );
}

CChartGrid::~CChartGrid()
{

}

void CChartGrid::AddTick(int Position)
{
	tickPos_.push_back(Position);
}

void CChartGrid::ClearTicks()
{
	tickPos_.clear();
}

void CChartGrid::Draw()
{
	if (!visible_)
		return;

	const int sz = tickPos_.size();
	list<int>::const_iterator iter = tickPos_.begin();
	int ActuPosition = 0;
	core::position2di startPos, endPos;

	for (iter; iter!=tickPos_.end(); iter++)
	{
		ActuPosition = *iter;
		
		if (!horizontal_)
		{
			// вертикальная ось - горизонтальные линии
			startPos = core::position2di( rectangle_.UpperLeftCorner.X, ActuPosition + rectangle_.UpperLeftCorner.Y);
			endPos = core::position2di( rectangle_.LowerRightCorner.X, ActuPosition + rectangle_.UpperLeftCorner.Y );		
		}
		else
		{
			// горизонтальная ось - вертикальные линии
			startPos = core::position2di( ActuPosition + rectangle_.UpperLeftCorner.X, rectangle_.UpperLeftCorner.Y );
			endPos = core::position2di( ActuPosition + rectangle_.UpperLeftCorner.X, rectangle_.LowerRightCorner.Y );
		}

		environment_->getVideoDriver()->draw2DLine(startPos, endPos, color_);
	}
}

}//namespace gui

}//namespace irr