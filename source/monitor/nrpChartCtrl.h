#pragma once

#include "nrpChartGradient.h"
#include <vector>

namespace irr
{

namespace gui
{
const size_t EGUIET_CHART = EGUIET_ELEMENT+2;

class CChartLegend;
class CChartTitle;
class CChartAxis;
class CChartSerie;
	
class CChartCtrl : public IGUIElement
{
public:
	typedef enum { AN_BOTTOM=0, AN_LEFT, AN_TOP, AN_RIGTH } CHART_AXIS_NAME;

	CChartLegend* GetLegend() const  { return ptrLegend_; }
	CChartTitle*  GetTitle()  const  { return ptrTitles_; }

	CChartAxis* GetAxis( CHART_AXIS_NAME axisName ) const;

	UINT GetEdgeType() const        { return edgeType_;    }
	void SetEdgeType(UINT newEdge)  { edgeType_ = newEdge; }

	const c8* getTypeName() const { return "chart"; }

	video::SColor GetBackColor() const			{ return backColor_;   }
	void SetBackColor(video::SColor color);

	video::SColor GetBorderColor() const			{ return borderColor_;   }
	void SetBorderColor(video::SColor color)	{ borderColor_ = color; }

	video::SColor GetZoomRectColor() const		{ return zoomRectColor_;   }
	void SetZoomRectColor(video::SColor color)	{ zoomRectColor_ = color; }

	void SetBackGradient(video::SColor Color1, video::SColor Color2, EGradientType GradientType);

	inline bool IsVisible() { return visible_; }
	inline void SetVisible( bool value ) { visible_ = value; }

	CChartSerie* AddSerie(int type);
	CChartSerie* GetSerie(size_t index) const;

	void RemoveSerie(size_t Index);
	void RemoveSerie(CChartSerie* ptrSerie);
	void RemoveAllSeries();
	size_t GetSeriesCount() const;
	
	void SetPanEnabled(bool bEnabled)  { panEnabled_ = bEnabled;  }
	bool GetPanEnabled() const		   { return panEnabled_;	    }

	void SetZoomEnabled(bool bEnabled) { zoomEnabled_ = bEnabled; }
	bool GetZoomEnabled() const		   { return zoomEnabled_;	    }

	void UndoPanZoom();

	void EnableRefresh(bool enable);

	//static double DateToValue(const COleDateTime& Date);
	//static COleDateTime ValueToDate(double Value);

 	CChartCtrl( IGUIEnvironment* environment, 
				IGUIElement* parent, 
				s32 id, 
				core::recti rectangle);

    virtual ~CChartCtrl();

	void draw();
	bool OnEvent(const SEvent& event);
	void Update();

private:
   // void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
   // void OnPrint(CDC *pDC, CPrintInfo *pInfo);
   // void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	void OnMouseMove_( const core::position2di& point );
	void OnLButtonDown_( core::position2di& point);
	void DrawZoomRectangle_();
	void OnLButtonUp_( core::position2di& point );
	void OnRButtonDown_( core::position2di& point );
	void OnRButtonUp_( core::position2di& point );
	void DrawBorder_();
	bool visible_;
   
	int RefreshCounter_ ;
	bool pendingRefresh_;
	bool m_bMemDCCreated;
	int timeLastUpdate_;

	bool backGradient_;	// Specifies if the background is gradient or solid
	video::SColor backGradientColor1_;
	video::SColor backGradientColor2_;
	EGradientType backGradientType_;
	video::SColor backColor_;	
	video::SColor borderColor_;
	UINT edgeType_;		

	irr::core::rect< irr::s32 > plottingRect_;	// Zone in wich the series will be plotted

	std::vector<CChartSerie*> seriesList_;		// Table containing all the series (dynamic)
	std::vector<CChartAxis*>  axisList_;			// Table containing all the axes (dynamic)

	CChartLegend* ptrLegend_;	// Chart legend
	CChartTitle*  ptrTitles_;	// Chart titles

	// Support for mouse panning
	bool panEnabled_;
	bool rMouseDown_;		// If the right mouse button is pressed
	core::position2di panAnchor_;

	// Support for manual zoom
	bool  zoomEnabled_;
	bool  leftMouseDown_;	// If the left mouse button is pressed
	core::recti rectZoomArea_;
	video::SColor zoomRectColor_;

	// Specifies if the toolbars have already been created
	bool m_bToolBarCreated;

    //CFont  m_PrinterFont;  
    // Printing information
	core::dimension2di m_LogicalPageSize;      // Page size in chartctrl units.
	core::dimension2di m_PaperSize;            // Page size in device units.
};

}//namespace gui

}//namespace irr