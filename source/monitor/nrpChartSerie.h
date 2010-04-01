#pragma once
#include "nrpChartObject.h"
#include <vector>
#include <string>

namespace irr
{

namespace gui
{

class CChartLineSerie;
class CChartPointsSerie;
class CChartAxis;
class CChartLegend;

class CChartSerie : public CChartObject  
{
	friend CChartCtrl;
	friend CChartLegend;

public:
	typedef struct SChartPoint 
	{
		float X;
		float Y;

		SChartPoint( float tX, float tY ) : X(tX), Y(tY) {}
		SChartPoint() : X(0), Y(0) { }
	} *PChartPoint;

	typedef enum 
	{
		ST_LINE_SERIE=0,
		ST_POINTS_SERIE=1,
		ST_SURFACE_SERIE=2,
		ST_BAR_SERIE=3
	} SERIES_TYPE;

	enum SERIES_ORDERING
	{
		soNoOrdering,
		soXOrdering,
		soYOrdering
	};

	CChartPointsSerie* GetAsPoints();
	CChartLineSerie*   GetAsLine();

	void AddPoint( float X, float Y);
	void AddPoints( float *X, float *Y, int Count);
	void AddPoint( float *Y, int Count);

	void SetOffset( float X, float Y );					//установка смещения графика
	core::vector2df GetOffset();							//получение смещения графика

	void SetPoints(float *X, float *Y, int Count);
	void SetPoints(SChartPoint* points, int Count);

	void ModifyPointsInPlace(SChartPoint* points, size_t count);
	void ModifyPointsInPlace(float *X, float *Y, int Count);

	virtual SERIES_TYPE GetType() = 0;

	void RemovePointsFromBegin(unsigned Count);
	void RemovePointsFromEnd(unsigned Count);
	void Clear();

	size_t GetPointsCount() const  { return points_.size(); }

	double GetYPointValue(int PointIndex) const;
	double GetXPointValue(int PointIndex) const;

	void   SetYPointValue(int PointIndex, float NewVal);
	void   SetXPointValue(int PointIndex, float NewVal);	
	
	bool GetSerieYMinMax(float& Min, float& Max)  const;
	bool GetSerieXMinMax(float& Min, float& Max)  const;


	void ValueToScreen(float XValue, float YValue, irr::core::position2di& ScreenPoint)  const;

	double YScreenToValue(long YScreenCoord)  const;
	double XScreenToValue(long XScreenCoord)  const;

	bool SetHorizontalAxis(bool bSecond);
	bool SetVerticalAxis(bool bSecond);

	CChartSerie( IGUIEnvironment* env, CChartCtrl* ptrParent);
	virtual ~CChartSerie();

	virtual void SetSeriesOrdering(SERIES_ORDERING newOrdering);
	SERIES_ORDERING GetSeriesOrdering() const		{ return ordering_; } 

	void SetName(const core::stringw& NewName);
	core::stringw GetName() { return name_; }

	virtual void Update() {};

protected:
	std::vector< SChartPoint >   points_; 
	
	int m_iLastDrawnPoint;			// Index of the last point drawn

	CChartAxis* verticalAxis_;    // Related vertical axis
	CChartAxis* horizontalAxis_;  // Related horizontal axis

	core::position2df minPosition_;
	core::position2df maxPosition_;
	core::position2df posOffset_;			  // добавка ко всем точкам	

	SERIES_ORDERING ordering_;

	void GetVisiblePoints(int& iFirst, int& iLast);

private:
	void RefreshMinMax();
	void InsertNewPoint(const SChartPoint& NewPoint);

	struct SPointSort
	{
		bool operator()(SChartPoint& ptStart, SChartPoint& ptEnd)
		{
			if (m_bXSort)
				return ptStart.X < ptEnd.X;
			else
				return ptStart.Y < ptEnd.Y;
		}

		bool m_bXSort;
	};
	void ReorderPoints();
	// костыль добавлен d.a. для удерживания масштаба графиком при перерисовке о_О
	bool hasAnyPoints_;

	core::stringw name_;
};

}//namespace gui

}//namespace irr