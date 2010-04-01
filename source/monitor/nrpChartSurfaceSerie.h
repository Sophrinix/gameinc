#pragma once

#include "nrpChartSerie.h"

namespace irr
{

namespace gui
{

class CChartSurfaceSerie : public CChartSerie  
{
public:
	CChartSurfaceSerie( IGUIEnvironment* env, CChartCtrl* pParent);
	virtual ~CChartSurfaceSerie();

	enum FillStyle
	{
		fsSolid = 0,
		fsHatchDownDiag,
		fsHatchUpDiag,
		fsHatchCross,
		fsHatchDiagCross,
		fsHatchHorizontal,
		fsHatchVertical
	};

	void SetFillStyle(FillStyle NewStyle);
	FillStyle GetFillStyle() const		   { return m_FillStyle; }

	void SetHorizontal(bool bHoriz);
	bool GetHorizontal() const		{ return m_bHorizontal;   }

	void SetSeriesOrdering(SERIES_ORDERING newOrdering);

private:
	//void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

	//void DrawAll(CDC *pDC);
	//void Draw(CDC* pDC);

	FillStyle m_FillStyle;
	// The surface serie can be either horizontal (the fill will be done between the curve 
	// and the corresponding horizontal axis) or vertical (the fill will be done between 
	// the curve and the corresponding vertical axis)
	bool m_bHorizontal;
};

}//namespace gui

}//namespace irr