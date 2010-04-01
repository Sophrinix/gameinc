#include "stdafx.h"
#include "nrpChartSerie.h"
#include "nrpChartAxis.h"
#include "nrpChartCtrl.h"

#include "Math.h"
#include <algorithm>

namespace irr
{

namespace gui
{

CChartSerie::CChartSerie( IGUIEnvironment* env, CChartCtrl* ptrParent) : CChartObject( env, ptrParent)
{
	hasAnyPoints_ = false;

	m_iLastDrawnPoint = 0;

	horizontalAxis_ = parent_->GetAxis( CChartCtrl::AN_BOTTOM );
	horizontalAxis_->RegisterSeries( this );

	verticalAxis_ = parent_->GetAxis( CChartCtrl::AN_LEFT );
	verticalAxis_->RegisterSeries(this);

	posOffset_ = maxPosition_ = minPosition_ = core::position2df( 0, 0 );
	ordering_ = soXOrdering;
}

CChartSerie::~CChartSerie()
{
	horizontalAxis_->UnregisterSeries(this);
	verticalAxis_->UnregisterSeries(this);
}

void CChartSerie::SetSeriesOrdering(SERIES_ORDERING newOrdering)
{
	ordering_ = newOrdering;
	ReorderPoints();
}

void CChartSerie::SetName(const core::stringw& NewName) 
{ 
	name_ = NewName; 
}

void CChartSerie::AddPoint( float X, float Y )
{
	bool bInsertPoint = true;
	if (points_.size() == 0)
	{
		minPosition_ = maxPosition_ = core::position2df( X, Y );
		bInsertPoint = false;
	}
	else
	{
		// Check if it can be inserted at the end of the series
		switch (ordering_)
		{
		case soNoOrdering:
			bInsertPoint = false;
			break;
		case soXOrdering:
			if (X > points_[points_.size()-1].X)
				bInsertPoint = false;
			break;
		case soYOrdering:
			if (Y > points_[points_.size()-1].Y)
				bInsertPoint = false;
			break;
		}

		if (X > maxPosition_.X )  maxPosition_.X = X;
		if (X < minPosition_.X )  minPosition_.X = X;
		if (Y > maxPosition_.Y )  maxPosition_.Y = Y;
		if (Y < minPosition_.Y )  minPosition_.Y = Y;
	}

	SChartPoint NewPoint( X, Y );
	if (!bInsertPoint)
		points_.push_back( NewPoint );
	else
		InsertNewPoint( NewPoint );

	parent_->EnableRefresh(false);
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();
	parent_->EnableRefresh(true);

	hasAnyPoints_ = true;
} 

void CChartSerie::AddPoints(float *X, float *Y, int Count)
{
	if (points_.size() == 0)
		minPosition_ = maxPosition_ = core::position2df( X[ 0 ], Y[ 0 ]);

	parent_->EnableRefresh(false);
	points_.reserve(points_.size() + Count);
	for (int i=0;i<Count;i++)
	{
		AddPoint(X[i], Y[i]);
	}
	parent_->EnableRefresh(true);

	hasAnyPoints_ = true;
}

void CChartSerie::SetPoints( SChartPoint* points, int Count)
{
	points_.clear();
	for (int i=0;i<Count;i++)
		points_.push_back( points[i] );

	parent_->EnableRefresh(false);
	ReorderPoints();
	RefreshMinMax();
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
	hasAnyPoints_ = true;
	
}

void CChartSerie::SetPoints(float *X, float *Y, int Count)
{
	points_.clear();
	for (int i=0;i<Count;i++)
	{
		points_.push_back( SChartPoint( X[ i ], Y[ i ] ) );
	}

	parent_->EnableRefresh(false);
	ReorderPoints();
	RefreshMinMax();
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
	hasAnyPoints_ = true;
}

void CChartSerie::ModifyPointsInPlace( SChartPoint* points, size_t count)
{
	if (!hasAnyPoints_)
		SetPoints(points, count);
	else 
	{
		points_.resize( count );
		memcpy( &points_[0], points, count * sizeof(core::position2df));
	}
}

void CChartSerie::ModifyPointsInPlace( float *X, float *Y, int Count )
{
	if (!hasAnyPoints_)
		SetPoints(X, Y, Count);
	else
		for (int i=0;i<Count;i++)
			points_[ i ] = SChartPoint( X[ i ], Y[ i ] );
}

void CChartSerie::RemovePointsFromBegin(unsigned Count)
{
	//ASSERT (Count < m_vPoints.size());
	points_.erase(points_.begin(),points_.begin()+Count);

	parent_->EnableRefresh(false);
	RefreshMinMax();
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
}

void CChartSerie::RemovePointsFromEnd(unsigned Count)
{
	//ASSERT (Count < m_vPoints.size());
	points_.erase(points_.end()-Count,points_.end());

	parent_->EnableRefresh(false);
	RefreshMinMax();
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
}

void CChartSerie::Clear()
{
	hasAnyPoints_ = false;
	// We don't care about the return of RefreshAutoAxis: 
	// we will always refresh the control afterwards.
	parent_->EnableRefresh(false);
	horizontalAxis_->RefreshAutoAxis();
	verticalAxis_->RefreshAutoAxis();

	minPosition_ = maxPosition_ = core::position2df( 0, 0 );
	points_.clear();

	parent_->EnableRefresh(true);
}

void CChartSerie::InsertNewPoint(const SChartPoint& NewPoint)
{
	if (points_.size() == 0) 
	{
		points_.push_back(NewPoint);
		return;
	}

	bool bInserted = false;
	std::vector<SChartPoint>::iterator iter = points_.begin();
	if (ordering_ == soXOrdering)
	{
		for (iter; iter!=points_.end(); iter++)
		{
			if (NewPoint.X > iter->X)
			{
				points_.insert(iter,NewPoint);
				bInserted = true;
				break;
			}
		}
	}
	else if (ordering_ == soYOrdering)
	{
		for (iter; iter!=points_.end(); iter++)
		{
			if (NewPoint.Y > iter->Y)
			{
				points_.insert(iter,NewPoint);
				bInserted = true;
				break;
			}
		}
	}

	if (!bInserted)
		points_.push_back(NewPoint);
}

void CChartSerie::ReorderPoints()
{
	SPointSort NewSort;
	switch(ordering_)
	{
	case soNoOrdering:
		//Do nothing
		break;
	case soXOrdering:
		NewSort.m_bXSort = true;
		std::sort(points_.begin(),points_.end(),NewSort);
		break;
	case soYOrdering:
		NewSort.m_bXSort = false;
		std::sort(points_.begin(),points_.end(),NewSort);
		break;
	}
}


double CChartSerie::GetXPointValue(int PointIndex) const
{
	if (PointIndex >= (int)points_.size() )
		return 0;

	return points_[PointIndex].X;
}

double CChartSerie::GetYPointValue(int PointIndex) const
{
	if (PointIndex >= (int)points_.size())
		return 0;

	return points_[PointIndex].Y;
}

void CChartSerie::SetYPointValue(int PointIndex, float NewVal)
{
	if (PointIndex >= (int)points_.size())
		return;

	points_[PointIndex].Y = NewVal;

	minPosition_.Y = maxPosition_.Y = points_[0].Y;
	std::vector<SChartPoint>::iterator iter = points_.begin();
	for (iter; iter!=points_.end(); iter++)
	{
		if (iter->Y > maxPosition_.Y )  maxPosition_.Y = iter->Y;
		if (iter->Y < minPosition_.Y )  minPosition_.Y = iter->Y;
	}

	if (ordering_ == soYOrdering)
		ReorderPoints();

	// We don't care about the return of RefreshAutoAxis: 
	// we will always refresh the control afterwards.
	parent_->EnableRefresh(false);
	verticalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
}
	
void CChartSerie::SetXPointValue(int PointIndex, float NewVal)
{
	if (PointIndex >= (int)points_.size())
		return;

	points_[PointIndex].X = NewVal;

	minPosition_.X = maxPosition_.X = points_[0].X;
	std::vector<SChartPoint>::iterator iter = points_.begin();
	for (iter; iter!=points_.end(); iter++)
	{
		if (iter->X > maxPosition_.X )  maxPosition_.X = iter->X;
		if (iter->X < minPosition_.X )  minPosition_.X = iter->X;
	}

	if (ordering_ == soYOrdering)
		ReorderPoints();

	// We don't care about the return of RefreshAutoAxis: 
	// we will always refresh the control afterwards.
	parent_->EnableRefresh(false);
	horizontalAxis_->RefreshAutoAxis();

	parent_->EnableRefresh(true);
}

void CChartSerie::RefreshMinMax()
{
	if (points_.size() == 0)
	{
		minPosition_ = maxPosition_ = core::position2df( 0, 0 );
		return;
	}

	minPosition_ = maxPosition_ = core::position2df( points_[0].X, points_[ 0 ].Y ) ;

	std::vector<SChartPoint>::iterator iter = points_.begin();
	for (iter; iter!=points_.end(); iter++)
	{
		if (iter->X > maxPosition_.X )  maxPosition_.X = iter->X;
		if (iter->X < minPosition_.X )  minPosition_.X = iter->X;
		if (iter->Y > maxPosition_.Y )  maxPosition_.Y = iter->Y;
		if (iter->Y < minPosition_.Y )  minPosition_.Y = iter->Y;
	}
}

bool CChartSerie::GetSerieXMinMax( float &Min, float &Max) const
{
	if ( (points_.size()==0) || !IsVisible() ) 
		return false;

	Min = minPosition_.X;
	Max = maxPosition_.X;
	return true;
}

bool CChartSerie::GetSerieYMinMax( float &Min, float &Max) const
{
	if (points_.size() == 0)
		return false;

	Min = minPosition_.Y;
	Max = maxPosition_.Y;
	return true;
}

bool CChartSerie::SetVerticalAxis(bool bSecond)
{
	if (bSecond)
	{		
		CChartAxis* pAxis = parent_->GetAxis( CChartCtrl::AN_RIGTH );
		if (pAxis)
		{
			parent_->EnableRefresh(false);
			verticalAxis_->UnregisterSeries(this);
			verticalAxis_->RefreshAutoAxis();

			verticalAxis_ = pAxis;
			verticalAxis_->RegisterSeries(this);
			verticalAxis_->RefreshAutoAxis();

			// The series will need to be redrawn so we need to refresh the control

			parent_->EnableRefresh(true);
			return true;
		}
		else
			return false;
	}
	else
	{
		CChartAxis* pAxis = parent_->GetAxis( CChartCtrl::AN_LEFT );
		if (pAxis)
		{
			parent_->EnableRefresh(false);
			verticalAxis_->UnregisterSeries(this);
			verticalAxis_->RefreshAutoAxis();

			verticalAxis_ = pAxis;
			verticalAxis_->RegisterSeries(this);
			verticalAxis_->RefreshAutoAxis();

			// The series will need to be redrawn so we need to refresh the control

			parent_->EnableRefresh(true);
			return true;
		}
		else
			return false;
	}
}

bool CChartSerie::SetHorizontalAxis(bool bSecond)
{
	if (bSecond)
	{
		CChartAxis* pAxis = parent_->GetAxis( CChartCtrl::AN_TOP );
		if (pAxis)
		{
			parent_->EnableRefresh(false);
			horizontalAxis_->UnregisterSeries(this);
			horizontalAxis_->RefreshAutoAxis();

			horizontalAxis_ = pAxis;
			horizontalAxis_->RegisterSeries(this);
			horizontalAxis_->RefreshAutoAxis();

			// The series will need to be redrawn so we need to refresh the control

			parent_->EnableRefresh(true);
			return true;
		}
		else
			return false;
	}
	else
	{
		CChartAxis* pAxis = parent_->GetAxis( CChartCtrl::AN_BOTTOM );
		if (pAxis)
		{
			parent_->EnableRefresh(false);
			horizontalAxis_->UnregisterSeries(this);
			horizontalAxis_->RefreshAutoAxis();

			horizontalAxis_ = pAxis;
			horizontalAxis_->RegisterSeries(this);
			horizontalAxis_->RefreshAutoAxis();

			// The series will need to be redrawn so we need to refresh the control

			parent_->EnableRefresh(true);
			return true;
		}
		else
			return false;
	}
}


double CChartSerie::XScreenToValue(long XScreenCoord) const
{
	return horizontalAxis_->ScreenToValue(XScreenCoord);
}

double CChartSerie::YScreenToValue(long YScreenCoord) const
{
	return verticalAxis_->ScreenToValue(YScreenCoord);
}

void CChartSerie::ValueToScreen( float XValue, float YValue, core::position2di& ScreenPoint) const
{
	ScreenPoint.X = horizontalAxis_->ValueToScreen(XValue);
	ScreenPoint.Y = verticalAxis_->ValueToScreen(YValue);
}

CChartLineSerie* CChartSerie::GetAsLine()
{
	//ASSERT(m_iSerieType==stLineSerie);
	return reinterpret_cast<CChartLineSerie*>(this);
}

CChartPointsSerie* CChartSerie::GetAsPoints()
{
	//ASSERT(m_iSerieType==stPointsSerie);
	return reinterpret_cast<CChartPointsSerie*>(this);
}

void CChartSerie::GetVisiblePoints(int& iFirst, int& iLast)
{
	// Init the values at largest range
	// TODO: optimize the function.
	iFirst = 0;
	iLast = points_.size() - 1;
	if (ordering_ == soNoOrdering)
		return;

	double XMin=0, XMax=0;
	double YMin=0, YMax=0;
	horizontalAxis_->GetMinMax(XMin, XMax);
	verticalAxis_->GetMinMax(YMin, YMax);

	int Count = 0;
	std::vector<SChartPoint>::iterator iter = points_.begin();
	for (iter; iter!=points_.end(); iter++)
	{
		if ( (ordering_==soXOrdering) && (iter->X >= XMin) )
		{
			iFirst = Count;
			break;
		}
		else if ( (ordering_==soYOrdering) && (iter->Y >= YMin) )
		{
			iFirst = Count;
			break;
		}
		Count++;
	}

	for (iter; iter!=points_.end(); iter++)
	{
		if ( (ordering_==soXOrdering) && (iter->X > XMax) )
		{
			iLast = Count;
			break;
		}
		else if ( (ordering_==soYOrdering) && (iter->Y > YMax) )
		{
			iLast = Count;
			break;
		}
		Count++;
	}

/*	Count = m_vPoints.size() - 1;
	std::vector<SChartPoint>::reverse_iterator revIter = m_vPoints.rbegin();
	for (revIter; revIter!=m_vPoints.rend(); revIter++)
	{
		if ( (m_Ordering==soXOrdering) && (revIter->X <= XMax) )
		{
			iLast = Count;
			break;
		}
		else if ( (m_Ordering==soYOrdering) && (revIter->Y <= YMax) )
		{
			iLast = Count;
			break;
		}
		Count--;
	}*/
}

void CChartSerie::SetOffset( float X, float Y )
{	
	std::vector<SChartPoint>::iterator pntIter = points_.begin();
	for( ; pntIter != points_.end(); pntIter++ )
	{
		 (*pntIter).X += (-posOffset_.X + X);
		 (*pntIter).Y += (-posOffset_.Y + Y);
	}

	posOffset_ = core::position2df( X, Y );
}

core::vector2df CChartSerie::GetOffset()
{
	return posOffset_;
}
}//namespace gui

}//namespace irr