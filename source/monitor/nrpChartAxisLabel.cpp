#include "stdafx.h"
#include "nrpChartAxisLabel.h"
#include "nrpChartCtrl.h"

namespace irr
{

namespace gui
{


CChartAxisLabel::CChartAxisLabel( IGUIEnvironment* env, CChartCtrl* ptrParent, bool horizontal) : CChartObject( env, ptrParent)
{
	horizontal_ = horizontal;

	fontSize_ = 100;
	strFontName_ = "";

	strLabelText_ = "";
}

CChartAxisLabel::~CChartAxisLabel()
{

}

void CChartAxisLabel::SetText(const TChartString& NewText)  
{ 
	strLabelText_ = NewText; 
}

void CChartAxisLabel::SetFont(int nPointSize, const TChartString& strFaceName)
{
	fontSize_ = nPointSize;
	strFontName_ = strFaceName;
}

void CChartAxisLabel::Draw()
{
/*	if (!m_bIsVisible)
		return;
	if (strLabelText_ == _T(""))
		return;

	if (!isHorizontal_)
	{
		pDC->ExtTextOut(m_ObjectRect.left + 2,m_ObjectRect.top,
			ETO_CLIPPED,NULL,strLabelText_.c_str(),NULL);
	}
	else
	{		
		pDC->ExtTextOut(m_ObjectRect.left,m_ObjectRect.top + 2,
			ETO_CLIPPED,NULL,strLabelText_.c_str(),NULL);
	}
*/
}

/*CSize CChartAxisLabel::GetSize(CDC *pDC) const
{
	CSize LabelSize;
	LabelSize.cx = 0;
	LabelSize.cy = 0;

	if (!m_bIsVisible)
		return LabelSize;
	if (!pDC->GetSafeHdc())
		return LabelSize;
	if (strLabelText_ == _T(""))
		return LabelSize;

	CFont NewFont;
	CFont* pOldFont;
	NewFont.CreatePointFont(fontSize_,m_strFontName.c_str(),pDC);
	pOldFont = pDC->SelectObject(&NewFont);

	LabelSize = pDC->GetTextExtent(strLabelText_.c_str());
	LabelSize.cx += 4;
	LabelSize.cy += 4;
	if (!isHorizontal_)
	{
		int Width = LabelSize.cy;
		int Height = LabelSize.cx;
		LabelSize.cx = Width;
		LabelSize.cy = Height;
	}
	pDC->SelectObject(pOldFont);
	DeleteObject(NewFont);

	return LabelSize;
}*/

/*void CChartAxisLabel::SetPosition(int LeftBorder, int TopBorder, CDC *pDC)
{
	CSize NewSize = GetSize(pDC);
	CRect NewRect;
	NewRect.top = TopBorder;
	NewRect.bottom = TopBorder + NewSize.cy;
	NewRect.left = LeftBorder;
	NewRect.right = LeftBorder + NewSize.cx;

	CChartObject::SetRect(NewRect);
}*/

}//namespace gui

}//namespace irr