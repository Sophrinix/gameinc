#pragma once

#include "nrpChartObject.h"
#include <list>

namespace irr
{

namespace gui
{

class CChartGrid;
class CChartSerie;
class CChartAxisLabel;
    
class CChartAxis : public CChartObject  
{
	friend CChartCtrl;
	friend CChartGrid;
	friend CChartSerie;

public:

	enum AXIS_TYPE
	{
		AT_STANDART = 0,
		AT_LOGARITHMIC,
		AT_DATETIME
	};
	void	 SetAxisType(AXIS_TYPE Type);
	AXIS_TYPE GetAxisType() const		 { return AxisType_; }

	enum TIME_INTERVAL
	{
		TI_SECOND,
		TI_MINUTE,
		TI_HOUR,
		TI_DAY,
		TI_MONTH,
		TI_YEAR
	};
	void SetDateTimeIncrement(bool autoTick, TIME_INTERVAL interval, int multiplier);
	void   SetTickIncrement(bool autoTick, double Increment);
	double GetTickIncrement() const  { return m_TickIncrement; }

	void SetDateTimeFormat(bool bAutomatic, const TChartString& strFormat);

	int GetPosition();

	void SetInverted(bool newValue);
	bool IsInverted() const  { return inverted_; }

	void SetLogarithmic(bool bNewValue);
	bool IsLogarithmic() const          { return (AxisType_ == AT_LOGARITHMIC); }
	void SetAutomatic(bool bNewValue);  
	bool IsAutomatic()  const  { return automatic_; }

	void SetMinMax(double Minimum, double Maximum);
	void GetMinMax(double& Minimum, double& Maximum) const
	{
		Minimum = m_MinValue;
		Maximum = m_MaxValue;
	}

	irr::s32 ValueToScreen(double Value) const;
    double ScreenToValue(long ScreenVal) const;
  
	void	 SetTextColor(COLORREF NewColor);
	irr::video::SColor GetTextColor() const			  { return m_TextColor;		}

	void SetFont(int nPointSize, const TChartString& strFaceName);

	CChartAxisLabel* GetLabel() const  { return m_pAxisLabel; }
	CChartGrid*		 GetGrid()	const  { return m_pAxisGrid;  }

	CChartAxis( IGUIEnvironment* env, CChartCtrl* ptrParent, bool horizontal);
	virtual ~CChartAxis();

	void SetMarginSize(bool bAuto, int iNewSize);

	void SetPanZoomEnabled(bool bEnabled) { m_bZoomEnabled = bEnabled; }
	void SetZoomLimit(double dLimit)   { m_dZoomLimit = dLimit; }

	void EnableScrollBar(bool bEnabled);
	void SetAutoHideScrollBar(bool bAutoHide);
	bool GetAutoHideScrollBar() const;
	void Draw();

	// добавлено d.a. для возможности быстрого переключения шкалы графика 
	// с точек на метры и обратно
	// по умолчанию стоит 1.0
	void SetScaleCoeff(double coeff);
private:
	void CalculateTickIncrement();
	void CalculateFirstTick();
	double GetNextTickValue(double Previous);
	TChartString GetTickLabel(double Tick);
	void Recalculate();
	void RefreshDTTickFormat();

	void PanAxis(long PanStart, long PanEnd);
	void SetZoomMinMax(double Minimum, double Maximum);
	void UndoZoom();

	void SetDecimals(int NewValue)  { m_DecCount = NewValue; }
	bool IsHorizontal() const  { return horizontal_; }

    int  GetAxisLenght() const;
	void SetSecondary(bool bNewVal)  { secondary_ = bNewVal; }
	bool GetSecondary() const		 { return secondary_; }

	bool RefreshAutoAxis();
	void GetSeriesMinMax(float& Minimum, float& Maximum);

	void SetAxisSize( core::recti ControlRect,
					  core::recti MarginRect);

	// To register/Unregister series related to this axis
	void RegisterSeries(CChartSerie* pSeries);
	void UnregisterSeries(CChartSerie* pSeries);

	void CreateScrollBar();
	void UpdateScrollBarPos();

	AXIS_TYPE AxisType_;		// Type of the axis (standard, log or date/time)

	bool horizontal_;	  // Indicates if this is an horizontal or vertical axis
	bool inverted_;		  // Indicates if the axis is inverted
	bool automatic_;      // Indicates if the axis is automatic

	bool secondary_;	// If the axis is secondary, it will be positioned to 
                            // the right (vertical) or to the top (horizontal)
	double m_MaxValue;		// Maximum value on the axis
	double m_MinValue;		
	double m_UnzoomMin;		// Min and max values of the axis before it has been zoomed
	double m_UnzoomMax;		// (used when we unzoom the chart -> go back to previous state)

	bool   autoTicks_;		// Specify if the tick increment is manual or automatic
	double m_TickIncrement;		// Indicates the space between ticks (in axis value) or for log axis the mult base between two ticks
	double m_FirstTickVal;

	unsigned int m_DecCount;	// Number of decimals to display
	int startPos_;			    // Start position of the axis 
	int endPos_;

	int  m_nFontSize;			
	TChartString m_strFontName;

	CChartGrid*			m_pAxisGrid;
	CChartAxisLabel*	m_pAxisLabel;

	typedef std::list<CChartSerie*> SeriesList;
	SeriesList m_pRelatedSeries;		// List containing pointers to series related to this axis

	// The user can specify the size of the margin, instead of
	// having it calculated automatically
	bool m_bAutoMargin;
	int m_iMarginSize;

	video::SColor m_TextColor;

	// Data for the date/time axis type.
	TChartString m_strDTTickFormat;		// Format of the date/time tick labels
	bool m_bAutoTickFormat;
	TIME_INTERVAL m_BaseInterval;
	int m_iDTTickIntervalMult;

	bool m_bZoomEnabled;
	double m_dZoomLimit;

	// добавлено d.a. для возможности быстрого переключения шкалы графика 
	// с точек на метры и обратно
	// по умолчанию стоит 1.0
	double coeff_;
};

}//namespace gui

}//namespace irr