#pragma once

#include "nrpChartSerie.h"

namespace irr
{

namespace gui
{
	
class CChartLineSerie : public CChartSerie  
{
public:
	int  GetPenStyle() const        { return penStyle_; }
	void SetPenStyle(int NewStyle);

	int	 GetWidth() const       { return lineWidth_; }
	void SetWidth(int NewValue);

	CChartLineSerie( IGUIEnvironment* env, CChartCtrl* pParent);
	virtual ~CChartLineSerie();

	SERIES_TYPE GetType() { return ST_LINE_SERIE; }
	void Draw();
	void Update();
private:
//	void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	std::vector< core::position2di >   drawingPoints_;	

	int lineWidth_;
	int penStyle_;
};

}//namespace gui

}//namespace irr