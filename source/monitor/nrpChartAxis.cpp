#include "stdafx.h"
#include "nrpChartAxis.h"
#include "nrpChartAxisLabel.h"
#include "nrpChartGrid.h"
#include "nrpChartCtrl.h"
#include "nrpChartSerie.h"

#include "Math.h"
#include <sstream>

using namespace std;

namespace irr
{

namespace gui
{


CChartAxis::CChartAxis( IGUIEnvironment* env, CChartCtrl* ptrParent, bool horizontal) : CChartObject(env, ptrParent)
{
	AxisType_ = AT_STANDART;

	horizontal_ = horizontal;
	inverted_ = false;
	automatic_ = false;

	secondary_ = false;

	m_MaxValue = m_UnzoomMax = 10;
	m_MinValue = m_UnzoomMin = 0;

	autoTicks_ = true;
	m_TickIncrement = 1;
	m_FirstTickVal = 0;
	m_DecCount = 0;

	startPos_ = endPos_ = 0;

	m_nFontSize = 80;
	m_strFontName = _T("Microsoft Sans Serif");

	m_pAxisGrid = new CChartGrid( environment_, ptrParent, horizontal_ );
	m_pAxisLabel = new CChartAxisLabel( environment_, ptrParent, horizontal_ );

	m_bAutoMargin = true;
	m_iMarginSize = 0;

	m_TextColor = color_;
	m_strDTTickFormat = TChartString( "%d %b" );
	m_bAutoTickFormat = true;
	m_BaseInterval = TI_DAY;
	m_iDTTickIntervalMult = 1;

	m_bZoomEnabled = true;
	m_dZoomLimit = 0.001;

	coeff_ = 1.0;

//	m_pScrollBar = NULL;
}

CChartAxis::~CChartAxis()
{
	if (m_pAxisGrid)
	{
		delete m_pAxisGrid;
		m_pAxisGrid = NULL;
	}
	if (m_pAxisLabel)
	{
		delete m_pAxisLabel;
		m_pAxisLabel = NULL;
	}

//	if (m_pScrollBar)
//	{
//		delete m_pScrollBar;
//		m_pScrollBar = NULL;
//	}
}	

void CChartAxis::SetAxisType( AXIS_TYPE type )  
{ 
	AxisType_ = type; 
}

/*int CChartAxis::ClipMargin(CRect ControlRect,CRect& MarginRect,CDC* pDC)
{
	if (!m_bIsVisible)
		return 0;

	int Size = 0;	

	CSize TickSize = GetLargestTick(pDC);
	CSize LabelSize = m_pAxisLabel->GetSize(pDC);

	if (m_bIsHorizontal)
	{
		if (!m_bAutoMargin)
			Size = m_iMarginSize;
		else
		{
			Size += 4 + 2;		//Space above and under the text

			Size += TickSize.cy;
			Size += LabelSize.cy;

			m_iMarginSize = Size;
		}

		if (!m_bIsSecondary)
		{
			ControlRect.bottom -= Size;
			ControlRect.right -= TickSize.cx/2+3;

			if (ControlRect.bottom < MarginRect.bottom)
				MarginRect.bottom = ControlRect.bottom;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
		else
		{
			ControlRect.top += Size;
			ControlRect.right -= TickSize.cx/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}

	}
	else
	{
		if (!m_bAutoMargin)
			Size = m_iMarginSize;
		else
		{
			Size += 7 + 1;		//Space before and after the text + Tick

			Size += TickSize.cx;
			Size += LabelSize.cx + 2;
			m_iMarginSize = Size;
		}

		if (!m_bIsSecondary)
		{
			ControlRect.left += Size;
			ControlRect.top += TickSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.left > MarginRect.left)
				MarginRect.left = ControlRect.left;
		}
		else
		{
			ControlRect.right -= Size;
			ControlRect.top += TickSize.cy/2+3;

			if (ControlRect.top > MarginRect.top)
				MarginRect.top = ControlRect.top;
			if (ControlRect.right < MarginRect.right)
				MarginRect.right = ControlRect.right;
		}
	}

	return Size;
}*/

int CChartAxis::GetPosition()
{
	if (horizontal_)
	{
		if (secondary_)
			return 0;
		else
			return 100;
	}
	else
	{
		if (secondary_)
			return 100;
		else
			return 0;
	}
}

void CChartAxis::SetAutomatic(bool bNewValue)  
{ 
	automatic_ = bNewValue; 
	if (automatic_)
		m_MinValue = m_MaxValue = 0;
}

void CChartAxis::SetTickIncrement(bool autoTick, double Increment)
{
	if (AxisType_ == AT_DATETIME)
		return;

	autoTicks_ = autoTick;
	if (!autoTicks_)
		m_TickIncrement = Increment;
	else
		CalculateTickIncrement();

	CalculateFirstTick();

}

void CChartAxis::SetDateTimeIncrement(bool autoTick, TIME_INTERVAL interval, int multiplier)
{
	if (AxisType_ != AT_DATETIME)
		return;

	autoTicks_ = autoTick;
	if (!autoTicks_)
	{
		m_BaseInterval = interval;
		m_iDTTickIntervalMult = multiplier;
	}
}

void CChartAxis::SetDateTimeFormat(bool bAutomatic, const TChartString& strFormat)
{
	m_bAutoTickFormat = bAutomatic;
	m_strDTTickFormat = strFormat;

}

void CChartAxis::SetAxisSize( core::recti ControlRect, core::recti MarginRect)
{
	core::recti axisSize = ControlRect;

	if (horizontal_)
	{
		startPos_ = MarginRect.UpperLeftCorner.X;	
		endPos_ = MarginRect.LowerRightCorner.X;
	
		if (!secondary_)
			axisSize.UpperLeftCorner.Y = MarginRect.LowerRightCorner.Y;
		else
			axisSize.LowerRightCorner.Y = MarginRect.UpperLeftCorner.Y;
	}
	else
	{
		startPos_ = MarginRect.LowerRightCorner.Y;
		endPos_ = MarginRect.UpperLeftCorner.Y;

		if (!secondary_)
			axisSize.LowerRightCorner.X = MarginRect.UpperLeftCorner.X;
		else
			axisSize.UpperLeftCorner.X = MarginRect.LowerRightCorner.X;
	}

	SetRect(axisSize);
}

void CChartAxis::Recalculate()
{
	CalculateTickIncrement();
	CalculateFirstTick();
}


/*void CChartAxis::DrawLabel(CDC* pDC)
{
	// Draw the axis label.
	CSize LabelSize = m_pAxisLabel->GetSize(pDC);
	int HalfAxisPos = (int)fabs((m_EndPos + m_StartPos)/2.0);
	int XPos = 0;
	int YPos = 0;
	if (m_bIsHorizontal)
	{
		if (!m_bIsSecondary)
		{
			CString Buffer;
			Buffer.Format(_T("%.*f"),m_DecCount,m_MaxValue);
			CSize TextSize = pDC->GetTextExtent(Buffer);
			
			YPos = m_ObjectRect.top  + TextSize.cy + 2;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
		else
		{
			YPos = m_ObjectRect.top  + 0;
			XPos = HalfAxisPos - LabelSize.cx/2;
		}
	}
	else
	{
		if (!m_bIsSecondary)
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_ObjectRect.left + 0;
		}
		else
		{
			YPos = HalfAxisPos + LabelSize.cy/2;
			XPos = m_ObjectRect.right - LabelSize.cx - 2;
		}
	}
	m_pAxisLabel->SetPosition(XPos,YPos,pDC);
	m_pAxisLabel->Draw(pDC);
}*/

void CChartAxis::SetMinMax(double Minimum, double Maximum)
{
	if (Minimum > Maximum)
	{
		throw ("Maximum axis value must be > minimum axis value");
		return;
	}

	m_MinValue = m_UnzoomMin = Minimum;
	m_MaxValue = m_UnzoomMax = Maximum;
}


void CChartAxis::SetInverted(bool bNewValue)
{
	inverted_ = bNewValue;
}

int CChartAxis::GetAxisLenght() const
{
    int Length = (int)fabs( (endPos_-startPos_) * 1.0);
    return Length;
}

s32 CChartAxis::ValueToScreen(double Value) const
{
    s32 Offset = 0;
	if (m_MaxValue==m_MinValue)
	{
		Offset = (s32)fabs((endPos_-startPos_)/2.0);
		if (horizontal_)
			return startPos_ + Offset;
		else
			return startPos_ - Offset;
	}

	if (AxisType_ != AT_LOGARITHMIC)
        Offset = (s32)floor( (Value - m_MinValue) * GetAxisLenght()/(m_MaxValue-m_MinValue) );
	else
		Offset = (s32)floor((log10(Value)-log10(m_MinValue)) * GetAxisLenght()/(log10(m_MaxValue)-log10(m_MinValue)) );
 
    if (horizontal_)
    {
	   if (!inverted_)
		  return (startPos_ + Offset);
	   else
		   return (endPos_ - Offset);
     }
     else
     {
    	if (!inverted_)
    		return (startPos_ - Offset);
    	else
    		return (endPos_ + Offset);
     }
}

double CChartAxis::ScreenToValue(long ScreenVal) const
{
	if (m_MaxValue==m_MinValue)
		return 0;

    int AxisOffset = 0;
    if (!horizontal_)
    {
        if (inverted_)
			AxisOffset = ScreenVal - endPos_;      
        else
			AxisOffset = startPos_ - ScreenVal;
            
    }
    else
    {
        if (!inverted_)
           AxisOffset = ScreenVal - startPos_;
        else
            AxisOffset = endPos_ - ScreenVal;
    }

	if (AxisType_ != AT_LOGARITHMIC)
		return ( (AxisOffset * 1.0 / GetAxisLenght()*(m_MaxValue-m_MinValue)) + m_MinValue);
	else
        return (pow(10.0,(AxisOffset *1.0 / GetAxisLenght()*(log10(m_MaxValue)-log10(m_MinValue)) ) + log10(m_MinValue)) );
}
   
void CChartAxis::SetTextColor(COLORREF NewColor) 
{ 
	m_TextColor = NewColor; 

}

void CChartAxis::SetFont(int nPointSize, const TChartString&  strFaceName)
{
	m_nFontSize = nPointSize;
	m_strFontName = strFaceName;

}

void CChartAxis::SetMarginSize(bool bAuto, int iNewSize)
{
	m_bAutoMargin = bAuto;
	m_iMarginSize = iNewSize;

}

void CChartAxis::PanAxis(long PanStart, long PanEnd)
{
	double StartVal = ScreenToValue(PanStart);
	double EndVal = ScreenToValue(PanEnd);

	if (AxisType_ != AT_LOGARITHMIC)
	{
		double Shift = StartVal - EndVal;
		SetZoomMinMax(m_MinValue+Shift,m_MaxValue+Shift);
	}
	else
	{
		double Factor = StartVal/EndVal;
		SetZoomMinMax(m_MinValue*Factor,m_MaxValue*Factor);
	}
}

void CChartAxis::SetZoomMinMax(double Minimum, double Maximum)
{
	if (!m_bZoomEnabled)
		return;

	if (Minimum > Maximum)
	{
		throw ("Maximum axis value must be > minimum axis value");
		return;
	}

	m_MinValue = Minimum;
	if ( (Maximum - Minimum) < m_dZoomLimit && AxisType_!=AT_LOGARITHMIC)
		m_MaxValue = m_MinValue + m_dZoomLimit;
	else
		m_MaxValue = Maximum;

}

void CChartAxis::UndoZoom()
{
	SetMinMax(m_UnzoomMin,m_UnzoomMax);
}

void CChartAxis::RegisterSeries(CChartSerie* pSeries)
{
	// First check if the series is already present in the list
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
			return;
	}

	m_pRelatedSeries.push_back(pSeries);
}

void CChartAxis::UnregisterSeries(CChartSerie* pSeries)
{
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if ( (*iter) == pSeries)
		{
			m_pRelatedSeries.erase(iter);
			return;
		}
	}
}

/*COleDateTime CChartAxis::AddMonthToDate(const COleDateTime& Date, int iMonthsToAdd)
{
	COleDateTime newDate;
	int nMonths = Date.GetMonth()-1 + iMonthsToAdd;
	int nYear = Date.GetYear() + nMonths/12;

	newDate.SetDateTime(nYear,nMonths%12+1,Date.GetDay(),Date.GetHour(),
		Date.GetMinute(),Date.GetSecond());
	return newDate;
}*/

bool CChartAxis::RefreshAutoAxis()
{
	bool bNeedRefresh = false;
	if (!automatic_)
		return bNeedRefresh;

	float SeriesMin = 0;
	float SeriesMax = 0;
	GetSeriesMinMax(SeriesMin, SeriesMax);

	if ( (SeriesMax!=m_MaxValue) || (SeriesMin!=m_MinValue) )
		SetMinMax(SeriesMin,SeriesMax);

	return bNeedRefresh;
}

void CChartAxis::GetSeriesMinMax(float& Minimum, float& Maximum)
{
	Minimum = 0;
	Maximum = 0;
	float TempMin = 0;
	float TempMax = 0;
	
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if (iter != m_pRelatedSeries.end())
	{
		if (horizontal_)
			(*iter)->GetSerieXMinMax(Minimum,Maximum);
		else
			(*iter)->GetSerieYMinMax(Minimum,Maximum);
	}

	for (iter; iter!=m_pRelatedSeries.end(); iter++)
	{
		if (horizontal_)
			(*iter)->GetSerieXMinMax(TempMin,TempMax);
		else
			(*iter)->GetSerieYMinMax(TempMin,TempMax);

		if (TempMin < Minimum)
			Minimum = TempMin;
		if (TempMax > Maximum)
			Maximum = TempMax;
	}
}

void CChartAxis::CalculateTickIncrement()
{
	if (!autoTicks_)
		return;

	if (m_MaxValue == m_MinValue)
	{
		m_iDTTickIntervalMult = 0;
		m_TickIncrement = 0;
		return;
	}

	int PixelSpace;
	if (horizontal_)
	{
		if (AxisType_ == AT_DATETIME)
			PixelSpace = 60;
		else
			PixelSpace = 40;
	}
	else
		PixelSpace = 20;

	int MaxTickNumber = (int)fabs((endPos_-startPos_)/PixelSpace * 1.0);

	//Calculate the appropriate TickSpace (1 tick every 30 pixel +/-)
	switch (AxisType_)
	{
	case AT_LOGARITHMIC:
	   m_TickIncrement = 10;
	   break;

	case AT_STANDART:
		{
	   		//Temporary tick increment
    		double TickIncrement = (m_MaxValue-m_MinValue)/MaxTickNumber;
	    
    		// Calculate appropriate tickSpace (not rounded on 'strange values' but 
    		// on something like 1, 2 or 5*10^X  where X is optimalized for showing the most
    		// significant digits)
    		int Zeros = (int)floor(log10(TickIncrement));
    		double MinTickIncrement = pow(10.0,Zeros);
	    
    		int Digits = 0;
    		if (Zeros<0)		
    		{
				//We must set decimal places. In the other cases, Digits will be 0.
    			Digits = (int)fabs(Zeros*1.0);
    		}
	    
    		if (MinTickIncrement>=TickIncrement)
    		{
    			m_TickIncrement = MinTickIncrement;
    			SetDecimals(Digits);
    		}
    		else if (MinTickIncrement*2>=TickIncrement)
    		{
    			m_TickIncrement = MinTickIncrement*2;
    			SetDecimals(Digits);
    		}
    		else if (MinTickIncrement*5>=TickIncrement)
    		{
    			m_TickIncrement = MinTickIncrement*5;
    			SetDecimals(Digits);
    		}
    		else if (MinTickIncrement*10>=TickIncrement)
    		{
    			m_TickIncrement = MinTickIncrement*10;
    			if (Digits)
    				SetDecimals(Digits-1);
    			else
    				SetDecimals(Digits);
    		}
		}
		break;

/*	case AT_DATETIME:
		{
			COleDateTime StartDate(m_MinValue);
			COleDateTime EndDate(m_MaxValue);

			COleDateTimeSpan minTickInterval = (EndDate - StartDate)/MaxTickNumber;
			double Seconds = minTickInterval.GetTotalSeconds();
			double Minutes = minTickInterval.GetTotalMinutes();
			double Hours = minTickInterval.GetTotalHours();
			double Days = minTickInterval.GetTotalDays();
			if (Seconds < 60)
			{
				m_BaseInterval = tiSecond;
				if (Seconds > 30)
				{
					m_BaseInterval = tiMinute;
					m_iDTTickIntervalMult = 1;
				}
				else if (Seconds > 10)
					m_iDTTickIntervalMult = 30;
				else if (Seconds > 5)
					m_iDTTickIntervalMult = 10;
				else if (Seconds > 2)
					m_iDTTickIntervalMult = 5;
				else 
					m_iDTTickIntervalMult = 1;
			}
			else if (Minutes < 60)
			{
				m_BaseInterval = tiMinute;
				if (Minutes > 30)
				{
					m_BaseInterval = tiHour;
					m_iDTTickIntervalMult = 1;
				}
				else if (Minutes > 10)
					m_iDTTickIntervalMult = 30;
				else if (Minutes > 5)
					m_iDTTickIntervalMult = 10;
				else if (Minutes > 2)
					m_iDTTickIntervalMult = 5;
				else 
					m_iDTTickIntervalMult = 2;
			}
			else if (Hours < 24)
			{
				m_BaseInterval = tiHour;
				if (Hours > 12)
				{
					m_BaseInterval = tiDay;
					m_iDTTickIntervalMult = 1;
				}
				else if (Hours > 6)
					m_iDTTickIntervalMult = 12;
				else if (Hours > 2)
					m_iDTTickIntervalMult = 6;
				else 
					m_iDTTickIntervalMult = 2;
			}
			else if (Days < 31)
			{
				m_BaseInterval = tiDay;
				if (Days > 7)
				{
					m_BaseInterval = tiMonth;
					m_iDTTickIntervalMult = 1;
				}
				else if (Days > 2)
				{
					m_BaseInterval = tiDay;
					m_iDTTickIntervalMult = 7;
				}
				else 
					m_iDTTickIntervalMult = 2;
			}
			else if (Days < 365)
			{
				m_BaseInterval = tiMonth;
				if (Days > 186)	 // Approx 6 months
				{
					m_BaseInterval = tiYear;
					m_iDTTickIntervalMult = 1;
				}
				else if (Days > 124)
					m_iDTTickIntervalMult = 6;
				else if (Days > 62)
					m_iDTTickIntervalMult = 4;
				else
					m_iDTTickIntervalMult = 2;
			}
			else
			{
				m_BaseInterval = tiYear;
				m_iDTTickIntervalMult = (int)Days/365 + 1;
			}

			if (m_bAutoTickFormat)
				RefreshDTTickFormat();
		}
		break; */
	}
}

void CChartAxis::CalculateFirstTick()
{
	switch (AxisType_)
	{
	case AT_STANDART:
		{
			m_FirstTickVal = 0;
			if (m_TickIncrement!=0)
			{
				if (m_MinValue == 0)
					m_FirstTickVal = 0;
				else if (m_MinValue>0)
				{
					m_FirstTickVal = (int)(m_MinValue/m_TickIncrement) * m_TickIncrement;
					while (m_FirstTickVal<m_MinValue)
						m_FirstTickVal += m_TickIncrement;
				}
				else
				{
					m_FirstTickVal = (int)(m_MinValue/m_TickIncrement) * m_TickIncrement;
					while (m_FirstTickVal>m_MinValue)
						m_FirstTickVal -= m_TickIncrement;
					if (!(m_FirstTickVal == m_MinValue))
						m_FirstTickVal += m_TickIncrement;
				}
			}
			else	// m_TickIncrement!=0
			{
				m_FirstTickVal = m_MinValue;
			}
		}
		break;

	case AT_LOGARITHMIC:
		{
			int LogBase = (int)log10(m_MinValue);
			m_FirstTickVal = pow(10.0,LogBase);
		}
		break;

/*	case AT_DATETIME:
		{
			COleDateTime dtMin((DATE)m_MinValue);
			COleDateTime dtFirstTick(dtMin);
			switch (m_BaseInterval)
			{
			case tiSecond:
				dtFirstTick.SetDateTime(dtMin.GetYear(),dtMin.GetMonth(),dtMin.GetDay(),
										dtMin.GetHour(),dtMin.GetMinute(),dtMin.GetSecond()+1);
				break;

			case tiMinute:
				dtFirstTick.SetDateTime(dtMin.GetYear(),dtMin.GetMonth(),dtMin.GetDay(),
										dtMin.GetHour(),dtMin.GetMinute(),0);
				if (dtMin.GetSecond() != 0)
					dtFirstTick += COleDateTimeSpan(0,0,1,0);					
				break;

			case tiHour:
				dtFirstTick.SetDateTime(dtMin.GetYear(),dtMin.GetMonth(),dtMin.GetDay(),
										dtMin.GetHour(),0,0);
				if ( (dtMin.GetMinute()!=0) || (dtMin.GetSecond()!=0) )
					dtFirstTick += COleDateTimeSpan(0,1,0,0);					
				break;

			case tiDay:
				dtFirstTick.SetDate(dtMin.GetYear(),dtMin.GetMonth(),dtMin.GetDay());
				if ( (dtMin.GetHour()!=0) || (dtMin.GetMinute()!=0) || 
					 (dtMin.GetSecond()!=0) )
				{
					dtFirstTick += COleDateTimeSpan(1,0,0,0);
				}
				break;

			case tiMonth:
				{
					dtFirstTick.SetDate(dtMin.GetYear(),dtMin.GetMonth(),1);
					if ((dtMin.GetDay()!=1) || (dtMin.GetHour()!=0) || 
						(dtMin.GetMinute()!=0) || (dtMin.GetSecond()!=0) )
					{
						dtFirstTick = AddMonthToDate(dtFirstTick,1);
					}
				}
				break;

			case tiYear:
				break;
			}
			m_FirstTickVal = (DATE)dtFirstTick;

		}
		break;*/
	}
}

double CChartAxis::GetNextTickValue(double Previous)
{
	double NewTick = 0;
	switch (AxisType_)
	{
	case AT_STANDART:
		NewTick = Previous + m_TickIncrement;
		break;

	case AT_LOGARITHMIC:
		NewTick = Previous * m_TickIncrement;
		break;

/*	case AT_DATETIME:
		{
			COleDateTime dtTick((DATE)Previous);
			COleDateTimeSpan dtSpan;
			switch (m_BaseInterval)
			{
			case tiSecond:
				dtSpan.SetDateTimeSpan(0,0,0,m_iDTTickIntervalMult);
				dtTick += dtSpan;
				break;
			case tiMinute:
				dtSpan.SetDateTimeSpan(0,0,m_iDTTickIntervalMult,0);
				dtTick += dtSpan;
				break;
			case tiHour:
				dtSpan.SetDateTimeSpan(0,m_iDTTickIntervalMult,0,0);
				dtTick += dtSpan;
				break;
			case tiDay:
				dtSpan.SetDateTimeSpan(m_iDTTickIntervalMult,0,0,0);
				dtTick += dtSpan;
				break;
			case tiMonth:
				{
					dtTick = AddMonthToDate(dtTick,m_iDTTickIntervalMult);
				}
				break;
			case tiYear:
				break;
			}

			NewTick = (DATE)dtTick;
		}
		break;*/
	}

	return NewTick;
}

TChartString CChartAxis::GetTickLabel(double TickValue)
{
	TChartString strLabel;
	switch (AxisType_)
	{
	case AT_STANDART:
	//	strLabel.Format(_T("%.*f"),m_DecCount,TickValue);
	//	ssLabel << setprecision(m_DecCount) << TickValue;
	//	sprintf(szBuffer,"%.*f",m_DecCount,TickValue);
		break;

	case AT_LOGARITHMIC:
		{
			double fLogDecCount; 
			int    nLogDecCount; 

			fLogDecCount = log10(TickValue); 

			if (fLogDecCount < 0.0) 
				nLogDecCount = (int)(fabs(fLogDecCount) + 0.1); 
			else 
				nLogDecCount = 0; 

		//	strLabel.Format(_T("%.*f"), nLogDecCount, TickValue); 
		}
		break;

	/*case AT_DATETIME:
		{
			COleDateTime tickTime((DATE)TickValue);
			strLabel = tickTime.Format(m_strDTTickFormat.c_str());

		//	ssLabel << tickTime.Format(m_strDTTickFormat.c_str());
		//	strcpy(szBuffer,strLabel);
		}
		break;*/
	}

	return strLabel;
}

void CChartAxis::RefreshDTTickFormat()
{
	switch (m_BaseInterval)
	{
	case TI_SECOND:
		m_strDTTickFormat = _T("%H:%M:%S");
		break;
	case TI_MINUTE:
		m_strDTTickFormat = _T("%H:%M");
		break;
	case TI_HOUR:
		m_strDTTickFormat = _T("%H:00");
		break;
	case TI_DAY:
		m_strDTTickFormat = _T("%d %b");
		break;
	case TI_MONTH:
		m_strDTTickFormat = _T("%b %Y");
		break;
	case TI_YEAR:
		m_strDTTickFormat = _T("%Y");
		break;
	}
}

void CChartAxis::Draw()
{
	if (!visible_)
		return;

	video::IVideoDriver* driver = environment_->getVideoDriver();
	core::position2di parentOffset = parent_->getAbsolutePosition().UpperLeftCorner;

	//CSize LabelSize = m_pAxisLabel->GetSize(pDC);

	// Draw the axis line 
	int Pos = 0;
	if (horizontal_)
	{
		if (!secondary_)
			Pos = rectangle_.UpperLeftCorner.Y+1;
		else
			Pos = rectangle_.LowerRightCorner.Y-1;

		driver->draw2DLine( parentOffset + core::position2di( startPos_, Pos ), 
							parentOffset + core::position2di( endPos_,   Pos ), 
							video::SColor( 0xff000000 ) );
	}
	else
	{
		if (!secondary_)
			Pos = rectangle_.LowerRightCorner.X-1;
		else
			Pos = rectangle_.UpperLeftCorner.X+1;
		
		driver->draw2DLine( parentOffset + core::position2di( Pos, startPos_), 
							parentOffset + core::position2di( Pos, endPos_), 
							video::SColor( 0xff000000 ) );
	}

	// Draw the label
	//DrawLabel(pDC);
	m_pAxisGrid->ClearTicks();

	//char szBuffer[255];
	core::stringw strBuffer;
	
	double TickValue = m_FirstTickVal;//GetNextTickValue(m_FirstTickVal);
	int TickPos = ValueToScreen(TickValue);
	do 
	{
		wchar_t strBuffer[ 16 ] = { 0 }; 
		swprintf( strBuffer, 15, L"%.2f", TickValue * coeff_ );

		TickPos = ValueToScreen(TickValue);

		if (horizontal_)
		{
			if (!secondary_)
			{
				driver->draw2DLine( parentOffset + core::position2di( TickPos, rectangle_.UpperLeftCorner.Y+1 ),
									parentOffset + core::position2di( TickPos, rectangle_.UpperLeftCorner.Y+4 ), 
									video::SColor( 0xff000000 ) );
				environment_->getBuiltInFont()->draw( strBuffer, 
													  core::recti( TickPos-20, rectangle_.UpperLeftCorner.Y+5,
																   TickPos+20, rectangle_.UpperLeftCorner.Y+20 ) + parentOffset,
													  video::SColor( 0xff000000 ) );
			}
			else
			{
				driver->draw2DLine( parentOffset + core::position2di( TickPos, rectangle_.UpperLeftCorner.Y-1),
									parentOffset + core::position2di( TickPos, rectangle_.UpperLeftCorner.Y-4), 
									video::SColor( 0xff000000 ) );

				environment_->getBuiltInFont()->draw( strBuffer, 
													  core::recti( TickPos-20,rectangle_.LowerRightCorner.Y+5,
																   TickPos+20,rectangle_.LowerRightCorner.Y+20 ) + parentOffset,
												 	  video::SColor( 0xff000000 ) );
			}
		}
		else
		{
			if (!secondary_)
			{
				driver->draw2DLine( parentOffset + core::position2di( -1, TickPos ),
									parentOffset + core::position2di( -4, TickPos ), 
									video::SColor( 0xff000000 ) );

				environment_->getBuiltInFont()->draw(	strBuffer, 
														core::recti( 5, TickPos,
																	 20, TickPos+20 ) + parentOffset,
														video::SColor( 0xff000000 ) );
			}
			else
			{
				driver->draw2DLine( parentOffset + core::position2di( 1, TickPos ),
									parentOffset + core::position2di( 4, TickPos ), 
									video::SColor( 0xff000000 ) );
				
				environment_->getBuiltInFont()->draw(	strBuffer, 
														core::recti( 5, TickPos,
																	 20, TickPos+20 ) + parentOffset,
														video::SColor( 0xff000000 ) );
			}
		}

		m_pAxisGrid->AddTick(TickPos);
		TickValue = GetNextTickValue(TickValue);
	} while ((TickValue < m_MaxValue+0.0000001) && (m_TickIncrement || m_iDTTickIntervalMult) );

	m_pAxisGrid->SetRect( parent_->getAbsolutePosition() );
	m_pAxisGrid->Draw();
}

void CChartAxis::SetScaleCoeff( double coeff )
{
	coeff_ = coeff;
}
/*CSize CChartAxis::GetLargestTick(CDC* pDC)
{
	CSize TickSize;

	CFont* pOldFont;
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC);
	pOldFont = pDC->SelectObject(&NewFont);

	CString strBuffer;
	strBuffer.Format(_T("%.*f"),m_DecCount,m_MaxValue);
	if (m_bIsHorizontal)
		TickSize = pDC->GetTextExtent(strBuffer);
	else
	{
		switch (AxisType_)
		{
		case atStandard:
			{
				int MaxChars = abs( (int)log10(fabs(m_MaxValue) )) + 1;
    			int MinChars = abs( (int)log10(fabs(m_MinValue) )) + 1;
    			if (m_MinValue<0)
    				MinChars++;
    			if (m_MaxValue<0)
    				MaxChars++;
    			if (MaxChars>MinChars)
					strBuffer.Format(_T("%.*f"),m_DecCount,m_MaxValue);
    			else
					strBuffer.Format(_T("%.*f"),m_DecCount,m_MinValue);
			}
			break;

		case AT_LOGARITHMIC:
			{
				CString strBuffMax;
				CString strBuffMin;
				int MaxDecCount = (int)log10(m_MaxValue);
				if (MaxDecCount < 0)
					MaxDecCount = -MaxDecCount;
				else
					MaxDecCount = 0;
				strBuffMax.Format(_T("%.*f"),MaxDecCount,m_MaxValue);

				int MinDecCount = (int)log10(m_MinValue);
				if (MinDecCount < 0)
					MinDecCount = -MinDecCount;
				else
					MinDecCount = 0;
				strBuffMin.Format(_T("%.*f"),MinDecCount,m_MinValue);

				if (strBuffMin.GetLength() > strBuffMax.GetLength() )
					strBuffer = strBuffMin;
				else
					strBuffer = strBuffMax;
			}			
			break;

		case AT_DATETIME:
			{
				double TickValue = m_FirstTickVal;
				CString strTemp;
				do 
				{
					strTemp = GetTickLabel(TickValue);
					if (strTemp.GetLength() > strBuffer.GetLength() )
						strBuffer = strTemp;
					TickValue = GetNextTickValue(TickValue);
				} while ((TickValue < m_MaxValue+0.0000001) && (m_TickIncrement|| m_iDTTickIntervalMult) ); 
			}
			break;
		}

 		TickSize = pDC->GetTextExtent(strBuffer);
	}

	pDC->SelectObject(pOldFont);
	DeleteObject(NewFont);
	return TickSize;
}*/

/*void CChartAxis::EnableScrollBar(bool bEnabled)
{
	if (m_pScrollBar)
	{
		m_pScrollBar->SetEnabled(bEnabled);
		if (bEnabled)
			m_pScrollBar->ShowWindow(SW_SHOW);
		else
			m_pScrollBar->ShowWindow(SW_HIDE);
	}
}*/

/*void CChartAxis::SetAutoHideScrollBar(bool bAutoHide)
{
	if (m_pScrollBar)
		m_pScrollBar->SetAutoHide(bAutoHide);
}*/

/*bool CChartAxis::GetAutoHideScrollBar() const
{ 
	if (m_pScrollBar)
		return (m_pScrollBar->GetAutoHide()); 
	else
		return false;
}*/

/*void CChartAxis::CreateScrollBar()
{
	m_pScrollBar = new CChartScrollBar(this);
	m_pScrollBar->CreateScrollBar(m_pParent->GetPlottingRect());
}*/

/*void CChartAxis::UpdateScrollBarPos()
{
	CRect PlottingRect = m_pParent->GetPlottingRect();
	PlottingRect.top++; PlottingRect.left++;

	CRect Temp;
	m_pScrollBar->GetWindowRect(&Temp);
	if (m_bIsHorizontal && !m_bIsSecondary)
		PlottingRect.top = PlottingRect.bottom - Temp.Height();
	if (!m_bIsHorizontal && !m_bIsSecondary)
		PlottingRect.right = PlottingRect.left + Temp.Width();
	if (m_bIsHorizontal && m_bIsSecondary)
		PlottingRect.bottom = PlottingRect.top + Temp.Height();
	if (!m_bIsHorizontal && m_bIsSecondary)
		PlottingRect.left = PlottingRect.right - Temp.Width();

	m_pScrollBar->MoveWindow(&PlottingRect);
}*/

/*void CChartAxis::RefreshScrollBar()
{
	if (m_pScrollBar)
		m_pScrollBar->Refresh();
		}*/

void irr::gui::CChartAxis::SetLogarithmic( bool bNewValue )
{
	if (bNewValue)
		AxisType_ = AT_LOGARITHMIC; 
	else
		AxisType_ = AT_STANDART;
	RefreshAutoAxis();
}

}//namespace gui

}//namespace irr