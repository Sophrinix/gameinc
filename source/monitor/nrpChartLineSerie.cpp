#include "stdafx.h"
#include "nrpChartLineSerie.h"
#include "nrpChartCtrl.h"

#include "Math.h"
namespace irr
{

namespace gui
{


CChartLineSerie::CChartLineSerie( IGUIEnvironment* env, CChartCtrl* pParent) : CChartSerie( env, pParent)
{
	lineWidth_ = 1;
	penStyle_ = PS_SOLID;
}

CChartLineSerie::~CChartLineSerie()
{

}

void CChartLineSerie::SetPenStyle(int NewStyle)  
{ 
	penStyle_ = NewStyle; 

}

void CChartLineSerie::SetWidth(int NewValue)  
{ 
	lineWidth_ = NewValue; 

}

/*void CChartLineSerie::DrawLegend(CDC *pDC, const CRect& rectBitmap) const
{
	if (m_strSerieName== _T(""))
		return;

	//Draw line:
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = m_ObjectColor;
	CPen NewPen(PS_GEOMETRIC | penStyle_,lineWidth_,&lb);
	CPen* pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(rectBitmap.left,rectBitmap.CenterPoint().y);
	pDC->LineTo(rectBitmap.right,rectBitmap.CenterPoint().y);
	pDC->SelectObject(pOldPen);
	DeleteObject(NewPen);
}*/
/*
if( left_point.Y >= right_point.Y )								//наклон положительный(учитывая размещение оси Y сверху вниз)
{
	if( tmp_line.intersectWith( bottom, inst_pnt ) == true )	//пересечение с нижней границей
	{
		left_point = position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );

		if( !plot_rect.isPointInside(right_point) )
		{   
			bool intersect = tmp_line.intersectWith( top, inst_pnt );
			if( !intersect )
				intersect = tmp_line.intersectWith( rigth, inst_pnt );			//пересечение c правой границей
			right_point = position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );
		}
		//иначе точка находится внутри области отрисовки
		driver->draw2DLine( left_point, right_point, m_ObjectColor );
		continue;
	}

	if( tmp_line.intersectWith( left, inst_pnt ) == true )		//есть пересечение с левой границей
	{
		left_point = position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );

		if( !plot_rect.isPointInside(right_point) )
		{
			bool intersect = tmp_line.intersectWith( top, inst_pnt );
			if( !intersect )
				intersect = tmp_line.intersectWith( rigth, inst_pnt);
			right_point = position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );
		}
		//иначе точка находится внутри области отрисовки
		driver->draw2DLine( left_point, right_point, m_ObjectColor );
		continue;
	}

}
else
{

}*/

void CChartLineSerie::Draw()
{
	video::IVideoDriver* driver = environment_->getVideoDriver();

	size_t dpNumber = drawingPoints_.size();

	core::position2di parentOffset = parent_->getAbsolutePosition().UpperLeftCorner;
	for( size_t cnt=0; cnt < dpNumber; cnt+=2 )
	{
		driver->draw2DLine( parentOffset + drawingPoints_[ cnt ],
							parentOffset + drawingPoints_[ cnt + 1 ], 
							color_ );
	}
}


void irr::gui::CChartLineSerie::Update()
{
	if (!visible_)
		return;

	drawingPoints_.clear();

	int iFirst=0, iLast=0;
	GetVisiblePoints(iFirst,iLast);

	if (iFirst>0)
		iFirst--;
	if (iLast<(int)GetPointsCount()-1)
		iLast++;

	core::recti plot_rect( 0, 0, parent_->getRelativePosition().getWidth(), parent_->getRelativePosition().getHeight() );

	core::line2df lines[ 4 ];
	lines[ 0 ] = core::line2df( (f32)plot_rect.UpperLeftCorner.X, (f32)plot_rect.LowerRightCorner.Y,			//bottom
		(f32)plot_rect.LowerRightCorner.X, (f32)plot_rect.LowerRightCorner.Y );
	lines[ 1 ] = core::line2df( (f32)plot_rect.UpperLeftCorner.X, (f32)plot_rect.UpperLeftCorner.Y,					//left
		(f32)plot_rect.UpperLeftCorner.X, (f32)plot_rect.LowerRightCorner.Y + 10 );
	lines[ 2 ] = core::line2df( (f32)plot_rect.UpperLeftCorner.X, (f32)plot_rect.UpperLeftCorner.Y,			//top
		(f32)plot_rect.LowerRightCorner.X, (f32)plot_rect.UpperLeftCorner.Y );
	lines[ 3 ] = core::line2df( (f32)plot_rect.LowerRightCorner.X, (f32)plot_rect.UpperLeftCorner.Y,			//rigth
		(f32)plot_rect.LowerRightCorner.X, (f32)plot_rect.LowerRightCorner.Y );

	for (m_iLastDrawnPoint=iFirst;m_iLastDrawnPoint<iLast;m_iLastDrawnPoint++)
	{
		core::position2di left_point, right_point;

		ValueToScreen(points_[m_iLastDrawnPoint].X,		points_[m_iLastDrawnPoint].Y,	left_point );
		ValueToScreen(points_[m_iLastDrawnPoint+1].X,	points_[m_iLastDrawnPoint+1].Y, right_point);

		core::line2df tmp_line( (f32)left_point.X, (f32)left_point.Y, 
			(f32)right_point.X, (f32)right_point.Y );
		core::vector2df inst_pnt;
		bool left_inside = plot_rect.isPointInside( left_point );
		bool right_inside = plot_rect.isPointInside( right_point );
		bool any_intersect = tmp_line.intersectWith( lines[ 0 ], inst_pnt ) ||
			tmp_line.intersectWith( lines[ 1 ], inst_pnt ) ||
			tmp_line.intersectWith( lines[ 2 ], inst_pnt ) ||
			tmp_line.intersectWith( lines[ 3 ], inst_pnt );


		if( !left_inside && !right_inside && !any_intersect )
			continue;

		if( left_inside && right_inside	 )
		{
			drawingPoints_.push_back( left_point );
			drawingPoints_.push_back( right_point );
			continue; 
		}
		else 
		{	
			int left_number=-1;
			bool perehod = left_inside && !right_inside;

			for( size_t limit_number=0; limit_number < 4; ++limit_number )
				if( tmp_line.intersectWith( lines[ limit_number ], inst_pnt ) == true )
				{
					left_number = limit_number;
					(perehod ? right_point : left_point) = core::position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );
					break;
				}

				for( size_t limit_number=0; limit_number < 4; ++limit_number )
					if( left_number != limit_number && 
						tmp_line.intersectWith( lines[ limit_number ], inst_pnt ) == true )
					{
						right_point = core::position2di( (s32)inst_pnt.X, (s32)inst_pnt.Y );
						break;
					}

				if( plot_rect.isPointInside( left_point ) && plot_rect.isPointInside( right_point ))
				{
					drawingPoints_.push_back( left_point );
					drawingPoints_.push_back( right_point );
				}
		}
	}
}

}//namespace gui

}//namespace irr