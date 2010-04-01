#include "stdafx.h"
#include "nrpChartLegend.h"
#include "nrpChartSerie.h"
#include "nrpChartCtrl.h"

namespace irr
{

namespace gui
{

class TraceGeometry
{
public:
	explicit TraceGeometry( core::position2di origin );
	core::recti GetRect(int x, int y) const;
	size_t GetTotalHeight(int rows) const;
	size_t GetTotalWidth() const;
	core::recti GetTotalRect(int rows) const;
	core::recti GetColorLabelRect(int row) const;
private:
	enum {COLOR_CELL_WIDTH = 30, TEXT_CELL_WIDTH = 200, CELL_HEIGHT = 20};
	core::position2di leftUpperCorner_;
};

TraceGeometry::TraceGeometry(core::position2di origin) : leftUpperCorner_(origin) { }

core::recti TraceGeometry::GetRect(int x, int y) const
{
	switch (x)
	{
	case 0:
		return core::recti(	leftUpperCorner_.X,
							leftUpperCorner_.Y + y * CELL_HEIGHT,
							leftUpperCorner_.X + COLOR_CELL_WIDTH,
							leftUpperCorner_.Y + (y + 1) * CELL_HEIGHT);
	case 1:
		return core::recti(	leftUpperCorner_.X  + COLOR_CELL_WIDTH,
							leftUpperCorner_.Y + y * CELL_HEIGHT,
							leftUpperCorner_.X + COLOR_CELL_WIDTH + TEXT_CELL_WIDTH,
							leftUpperCorner_.Y + (y + 1) * CELL_HEIGHT);
	default:
		throw "this error for nonselected x:(TraceGeometry Constructor)";
	}
}
//////////////////////////////////////////////////////////////////////////

size_t TraceGeometry::GetTotalHeight(int rows) const
{
	return rows * CELL_HEIGHT;
}
//////////////////////////////////////////////////////////////////////////

size_t TraceGeometry::GetTotalWidth() const
{
	return COLOR_CELL_WIDTH + TEXT_CELL_WIDTH;
}
//////////////////////////////////////////////////////////////////////////

core::recti TraceGeometry::GetTotalRect(int rows) const
{
	return core::recti(leftUpperCorner_, core::dimension2di(GetTotalWidth(), GetTotalHeight(rows)));
}
//////////////////////////////////////////////////////////////////////////

core::recti TraceGeometry::GetColorLabelRect(int row) const
{
	core::recti colorRect = GetRect(0, row);
	colorRect.UpperLeftCorner += core::position2di(10,5);
	colorRect.LowerRightCorner -= core::position2di(10,5);
	return colorRect;
}
//////////////////////////////////////////////////////////////////////////

CChartLegend::CChartLegend( IGUIEnvironment* env, CChartCtrl* pParent)
			 :CChartObject( env, pParent)
{
	color_ = video::SColor( 0xffffffff );

	visible_ = false;

	docked_ = true;
	dockSide_ = DS_RIGHT;
	m_iLeftPos = m_iTopPos = 0;
	transparent_ = false;
	horizontal_ = false;

}

CChartLegend::~CChartLegend()
{
}

void CChartLegend::SetFont(int iPointSize, const TChartString& strFaceName)
{
	m_strFontName = strFaceName;
}

void CChartLegend::SetTransparent(bool bTransparent)
{ 
	transparent_ = bTransparent; 
}

void CChartLegend::SetHorizontalMode(bool bHorizontal)
{
	horizontal_ = bHorizontal;

}

void CChartLegend::DockLegend(DOCK_SIDE dsSide)
{
	docked_ = true;
	dockSide_ = dsSide;

}

void CChartLegend::UndockLegend(int iLeftPos, int iTopPos)
{
	docked_ = false;
	m_iLeftPos = iLeftPos;
	m_iTopPos = iTopPos;

}

void CChartLegend::DrawBorder( core::recti bounds) const
{
	video::SColor borderColor(128, 0,0,0);
	video::IVideoDriver *driver = environment_->getVideoDriver();
	driver->draw2DLine( bounds.UpperLeftCorner, 
		core::position2di( bounds.UpperLeftCorner.X, bounds.LowerRightCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.LowerRightCorner, 
		core::position2di( bounds.UpperLeftCorner.X, bounds.LowerRightCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.LowerRightCorner, 
		core::position2di( bounds.LowerRightCorner.X, bounds.UpperLeftCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.UpperLeftCorner, 
		core::position2di( bounds.LowerRightCorner.X, bounds.UpperLeftCorner.Y ),
		borderColor );

}

void CChartLegend::Draw()
{
	if (!visible_)
		return;
	if (!parent_->GetSeriesCount())
		return;

	const core::recti ClippingRect =  parent_->getAbsolutePosition();
	core::position2di origin(parent_->getAbsolutePosition().UpperLeftCorner);
	origin += core::position2di(50,20);	
	TraceGeometry geom(origin);
	environment_->getVideoDriver()->draw2DRectangle(video::SColor(150,255,255,255), geom.GetTotalRect(parent_->GetSeriesCount()), &ClippingRect);
	
	core::recti BorderRect = geom.GetTotalRect(parent_->GetSeriesCount());
	BorderRect.clipAgainst(ClippingRect);
	DrawBorder(BorderRect);
 
	for (size_t nSerie = 0; nSerie < parent_->GetSeriesCount(); nSerie ++)
	{
		core::recti SquareRect = geom.GetColorLabelRect(nSerie);
		SquareRect.clipAgainst(ClippingRect);
		DrawColorLabel(SquareRect, parent_->GetSerie(nSerie)->GetColor());
		environment_->getBuiltInFont()->draw( parent_->GetSerie(nSerie)->GetName().c_str(), 
			                                  geom.GetRect(1, nSerie), 
											  video::SColor(200, 0,0,0), true, true, &ClippingRect);
	}
}

void CChartLegend::DrawColorLabel( irr::core::rect<irr::s32> bounds, irr::video::SColor color ) const
{
	bounds -= core::position2di(1,1);
	environment_->getVideoDriver()->draw2DRectangle(color, bounds);

	video::SColor borderColor(128, 0,0,0);
	video::IVideoDriver *driver = environment_->getVideoDriver();
	driver->draw2DLine( bounds.UpperLeftCorner, 
		core::position2di( bounds.UpperLeftCorner.X, bounds.LowerRightCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.LowerRightCorner, 
		core::position2di( bounds.UpperLeftCorner.X, bounds.LowerRightCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.LowerRightCorner, 
		core::position2di( bounds.LowerRightCorner.X, bounds.UpperLeftCorner.Y ),
		borderColor );
	driver->draw2DLine( bounds.UpperLeftCorner, 
		core::position2di( bounds.LowerRightCorner.X, bounds.UpperLeftCorner.Y ),
		borderColor );
}

}//namespace gui

}//namespace irr