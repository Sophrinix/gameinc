#pragma once
#include "nrpChartSerie.h"

namespace irr
{

namespace gui
{
	
class CChartPointsSerie : public CChartSerie  
{
public:
	enum PointType
	{
		ptEllipse=0,
		ptRectangle=1,
		ptTriangle=2
	};

	void SetPointSize(int XSize, int YSize);
	void GetPointSize(int& XSize, int& YSize) const
	{
        XSize = m_iXPointSize;
        YSize = m_iYPointSize;
    }
	void SetPointType(PointType Type);
	PointType GetPointType() const     { return m_iPointType; }

	CChartPointsSerie( IGUIEnvironment* env, CChartCtrl* pParent);
	virtual ~CChartPointsSerie();

private:
//	void DrawLegend(CDC* pDC, const CRect& rectBitmap) const;

//	void DrawAll(CDC *pDC);
//	void Draw(CDC* pDC);


	int m_iXPointSize;		// Width of the points
	int m_iYPointSize;		// Height of the points
	PointType m_iPointType;

};

}//namespace gui

}//namespace irr