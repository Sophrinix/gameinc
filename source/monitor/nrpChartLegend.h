#pragma once

#include "nrpChartObject.h"

namespace irr
{

namespace gui
{

class CChartSerie;

class CChartLegend : public CChartObject  
{
	friend CChartCtrl;

public:
	void SetFont(int iPointSize, const TChartString& strFaceName);

	CChartLegend( IGUIEnvironment* env, CChartCtrl* pParent);
	virtual ~CChartLegend();

	enum DOCK_SIDE
	{
		DS_RIGHT,
		DS_LEFT,
		DS_TOP,
		DS_BOTTOM
	};

	void DockLegend(DOCK_SIDE dsSide);
	void UndockLegend(int iLeftPos, int iTopPos);

	void SetTransparent(bool bTransparent);  
	void SetHorizontalMode(bool bHorizontal);
	void Draw();
	void DrawBorder(irr::core::rect<irr::s32> bounds) const;
	void DrawColorLabel(irr::core::rect<irr::s32> bounds, irr::video::SColor color) const;
private:
	//void UpdatePosition(CDC* pDC, const CRect& rcControl);

	TChartString m_strFontName;

	bool docked_;	// true if the legend is docked
	DOCK_SIDE dockSide_;

	// If the legend is not docked:
	int m_iLeftPos;
	int m_iTopPos;

	bool transparent_;
	bool horizontal_;
};

}//namespace gui

}//namespace irr