#include "stdafx.h"
#include "nrpChartTitle.h"
#include "nrpChartCtrl.h"
#include "Math.h"

using namespace std;

namespace irr
{

namespace gui
{


CChartTitle::CChartTitle( IGUIEnvironment* env, CChartCtrl* pParent) : CChartObject( env, pParent)
{
	color_ = video::SColor( 0xff, 0,0,0);
}

CChartTitle::~CChartTitle()
{

}

void CChartTitle::SetFont( const TChartString& new_name )
{
	font_name_ = new_name;
}

void CChartTitle::AddString(const TChartString& NewString)
{
	strings_.push_back(NewString);
}

size_t CChartTitle::GetStringCount() const
{
	return strings_.size();
}

TChartString CChartTitle::GetString(size_t Index) const
{
	return strings_.at( Index );
}

void CChartTitle::RemoveAll()
{
	strings_.clear();
}

core::dimension2di CChartTitle::GetSize()
{
	core::dimension2di TitleSize;

	size_t strings_count = strings_.size();

	if (!visible_ || strings_count == 0 )
	{
		return core::dimension2di( 0, 0 );
	}

	s32 Height = 4;		//Upper space
	s32 TextSize;
	int MaxTextWidth = 0;

	for (size_t nom = 0; nom < strings_count; nom++)
	{
		TextSize = strings_[ nom ].size() * 4;
		Height += /*TextSize.cy */ 10 + 2;

		if ( TextSize > MaxTextWidth )
			 MaxTextWidth = TextSize;
	}

	TitleSize = core::dimension2di( MaxTextWidth + 2, Height );

	//m_ObjectRect.bottom = m_ObjectRect.top + Height;

	return TitleSize;
}

void CChartTitle::Draw()
{
	if (!visible_)
		return;

	//Draw all entries
/*	int YPos = 4;
	size_t strings_count = strings_.size();
	for (size_t i=0;i<strings_count;i++)
	{
		//Draw Text
		int TextWidth = pDC->GetTextExtent(strings_[i].c_str()).cx;
		int TextHeigh = pDC->GetTextExtent(strings_[i].c_str()).cy;

		int XPos = m_ObjectRect.left + (int)fabs((m_ObjectRect.left-m_ObjectRect.right)/2.0) - TextWidth/2;

		if (m_bShadow)
		{
			pDC->SetTextColor(m_ShadowColor);
			pDC->ExtTextOut(XPos+m_iShadowDepth,m_ObjectRect.top+YPos+m_iShadowDepth,
				ETO_CLIPPED,NULL,strings_[i].c_str(),NULL);
			pDC->SetTextColor(m_ObjectColor);
		}
		pDC->ExtTextOut(XPos,m_ObjectRect.top+YPos,ETO_CLIPPED,NULL,strings_[i].c_str(),NULL);

		YPos += TextHeigh + 2;
	}
*/
}

}//namespace gui

}//namespace irr