#include "stdafx.h"
#include "nrpChartCtrl.h"

#include "nrpChartSerie.h"
#include "nrpChartAxis.h"
#include "nrpChartLineSerie.h"
#include "nrpChartPointsSerie.h"
#include "nrpChartSurfaceSerie.h"
#include "nrpChartGradient.h"
#include "nrpChartLegend.h"
#include "nrpChartTitle.h"

namespace irr
{

namespace gui
{

video::SColor pSeriesColorTable[] = { video::SColor( 0xffff0000 ), video::SColor( 0xff009600 ), video::SColor( 0xff0000ff ), 
									  video::SColor( 0xffffff00 ), video::SColor( 0xff00ffff ), video::SColor( 0xffff8000 ), 
									  video::SColor( 0xff800080 ), video::SColor( 0xff808000 ), video::SColor( 0xffff00ff ), 
									  video::SColor( 0xff408080 ) };

/////////////////////////////////////////////////////////////////////////////
// CChartCtrl

CChartCtrl::CChartCtrl( IGUIEnvironment* environment, 
					    IGUIElement* parent, 
						s32 id, core::recti rectangle)
					   : IGUIElement( static_cast< EGUI_ELEMENT_TYPE >( EGUIET_CHART ), environment, parent, id, rectangle)
{
	RefreshCounter_ = 1;
	pendingRefresh_ = false;
	borderColor_ = video::SColor( 0xff000000 );
	backColor_ = GetSysColor(COLOR_BTNFACE);
	edgeType_ = EDGE_RAISED;
	backGradientType_ = gtVertical;
	backGradient_ = false;
	backGradientColor1_ = backGradientColor2_ = backColor_;
	timeLastUpdate_ = 0;

	CChartAxis* axisBottom = new CChartAxis( Environment, this, true );
	// чтобы шкала была в метрах
	axisBottom->SetScaleCoeff(3.0 / 20.0);

	CChartAxis* axisLeft = new CChartAxis( Environment, this, false );
	
	CChartAxis* axisTop = new CChartAxis( Environment, this,true);
	axisTop->visible_ = false;
	axisTop->SetSecondary(true);
	
	CChartAxis* axisRight = new CChartAxis( Environment, this,false);
	axisRight->visible_ = false;
	axisRight->SetSecondary(true);

	axisList_.push_back(axisBottom);
	axisList_.push_back(axisLeft);
	axisList_.push_back(axisTop);
	axisList_.push_back(axisRight);

	ptrLegend_ = new CChartLegend( Environment, this );
	ptrTitles_ = new CChartTitle( Environment, this);
	
	m_bMemDCCreated = false;
	panEnabled_ = true;
	rMouseDown_ = false;

	zoomEnabled_ = true;
	leftMouseDown_ = false;
	zoomRectColor_ = video::SColor( 0xffffffff );

	m_bToolBarCreated = false;
}
/////////////////////////////////////////////////////////////////////////////

CChartCtrl::~CChartCtrl()
{
	size_t SeriesCount = seriesList_.size();
	for (size_t i=0;i<SeriesCount;i++)
		delete seriesList_[i];

	size_t AxisCount = axisList_.size();
	for (size_t j=0;j<AxisCount;j++)
		delete axisList_[j];

	if (ptrLegend_)
	{
		delete ptrLegend_;
		ptrLegend_ = NULL;
	}

	if (ptrTitles_)
	{
		delete ptrTitles_;
		ptrTitles_ = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////

void CChartCtrl::SetBackGradient(video::SColor Color1, video::SColor Color2, EGradientType GradientType)
{
	backGradient_ = true;
	backGradientColor1_ = Color1;
	backGradientColor2_ = Color2;
	backGradientType_ = GradientType;
}
/////////////////////////////////////////////////////////////////////////////

void CChartCtrl::EnableRefresh(bool bEnable)
{
	if (bEnable)
		RefreshCounter_++;
	else
		RefreshCounter_--;

	if (RefreshCounter_ > 0 && pendingRefresh_)
	{
		pendingRefresh_ = false;
	}
}
/////////////////////////////////////////////////////////////////////////////

void CChartCtrl::UndoPanZoom()
{
	EnableRefresh(false);
	axisList_.at( AN_BOTTOM )->UndoZoom();
	axisList_.at( AN_TOP )->UndoZoom();
	axisList_.at( AN_LEFT )->UndoZoom();
	axisList_.at( AN_RIGTH )->UndoZoom();
	EnableRefresh(true);
}
/////////////////////////////////////////////////////////////////////////////

void CChartCtrl::draw()
{
	if( !visible_ )
		return;

	core::dimension2di TitleSize = ptrTitles_->GetSize();
	core::recti rcTitle;

	rcTitle = AbsoluteRect;
	rcTitle.LowerRightCorner.Y = TitleSize.Height;
	
	//m_pTitles->Draw(pDC);

	//m_pLegend->ClipArea(ChartRect,pDC);

	//Clip all the margins (axis) of the client rect
	size_t AxisCount = axisList_.size();
	// TODO: find a better way to handle this.

	std::vector<CChartSerie*>::iterator pIter = seriesList_.begin();
	for(; pIter != seriesList_.end(); pIter++ )
		(*pIter)->Draw();

	std::vector<CChartAxis*>::iterator aIter = axisList_.begin();
	for(; aIter != axisList_.end(); aIter++ )
		(*aIter)->Draw();

	// Draw the zoom rectangle
	if (zoomEnabled_ && leftMouseDown_)
		DrawZoomRectangle_();

	DrawBorder_();
	// Draw the legend at the end (when floating it should come over the plotting area).
	ptrLegend_->Draw();
}
/////////////////////////////////////////////////////////////////////////

void CChartCtrl::DrawZoomRectangle_()
{
	video::IVideoDriver* driver = Environment->getVideoDriver();

	driver->draw2DRectangleOutline( rectZoomArea_ + AbsoluteRect.UpperLeftCorner, zoomRectColor_ );
}
/////////////////////////////////////////////////////////////////////////////

void CChartCtrl::DrawBorder_()
{
	video::IVideoDriver* driver = Environment->getVideoDriver();

	driver->draw2DRectangleOutline( AbsoluteRect, borderColor_ );
}
//////////////////////////////////////////////////////////////////////////

CChartSerie* CChartCtrl::AddSerie(int Type)
{
	size_t Count = seriesList_.size();

	size_t ColIndex = Count%10;
	CChartSerie* pNewSeries = NULL;

	switch (Type)
	{
	case CChartSerie::ST_LINE_SERIE:
		pNewSeries = new CChartLineSerie( Environment, this);
		break;

//	case CChartSerie::stPointsSerie:
//		pNewSeries = new CChartPointsSerie(this);
//		break;

//	case CChartSerie::stSurfaceSerie:
//		pNewSeries = new CChartSurfaceSerie(this);
//		break;

//	case CChartSerie::stBarSerie:
//		pNewSeries = new CChartBarSerie(this);
//		break;

	default:
		pNewSeries = NULL;
		break;
	}

	if (pNewSeries)
	{
		pNewSeries->SetRect( core::recti( 0, 0, RelativeRect.getWidth(), RelativeRect.getHeight() ) );
		pNewSeries->SetColor( pSeriesColorTable[ColIndex] );
		seriesList_.push_back( pNewSeries );
	}
	//m_pLegend->RecalculateArea();

	return pNewSeries;
}

CChartSerie* CChartCtrl::GetSerie(size_t Index) const
{
	if (Index >= seriesList_.size())
		return NULL;

	return seriesList_[Index];
}

void CChartCtrl::RemoveSerie(size_t Index)
{
	if( Index >= seriesList_.size() )
		return;

	CChartSerie* seriesToDelete = seriesList_[Index];

    RemoveSerie( seriesToDelete );
}

void CChartCtrl::RemoveSerie(CChartSerie* ptrSerie)
{
    std::vector<CChartSerie*>::iterator iter = seriesList_.begin();

	for (iter; iter!=seriesList_.end();iter++)
		if (*iter == ptrSerie)
		{
			seriesList_.erase(iter);
			break;
		}

	if (ptrSerie)
	{
		EnableRefresh(false);

		ptrSerie->verticalAxis_->UnregisterSeries(ptrSerie);
		ptrSerie->horizontalAxis_->UnregisterSeries(ptrSerie);
		ptrSerie->verticalAxis_->RefreshAutoAxis();
		ptrSerie->horizontalAxis_->RefreshAutoAxis();
		delete ptrSerie;

		EnableRefresh(true);
	}
}

void CChartCtrl::RemoveAllSeries()
{
	std::vector<CChartSerie*>::iterator iter = seriesList_.begin();
	for (iter; iter != seriesList_.end(); iter++)
	{
		delete (*iter);
	}

	seriesList_.clear();

}

size_t CChartCtrl::GetSeriesCount() const
{
	return seriesList_.size();
}

bool CChartCtrl::OnEvent( const SEvent& event )
{
	switch( event.EventType )
	{
	case EET_MOUSE_INPUT_EVENT:
		 switch( event.MouseInput.Event )
		 {
		 case EMIE_MOUSE_MOVED:
			  OnMouseMove_( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
		      return true;

		 case EMIE_LMOUSE_PRESSED_DOWN:
			  OnLButtonDown_( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
		      return true;

		 case EMIE_LMOUSE_LEFT_UP:
			  OnLButtonUp_( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
		      return true;

		 case EMIE_RMOUSE_PRESSED_DOWN:
			  OnRButtonDown_( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
			  return true;
		 
		 case EMIE_RMOUSE_LEFT_UP:
			  OnRButtonUp_( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
			  return true;
		 }
	break;
	}

	return IGUIElement::OnEvent( event );
}
//////////////////////////////////////////////////////////////////////////

void CChartCtrl::OnMouseMove_( const core::position2di& point ) 
{
	if (rMouseDown_ && panEnabled_)
	{
		if(point != panAnchor_)
		{
			axisList_.at( AN_BOTTOM )->PanAxis(panAnchor_.X, point.X - AbsoluteRect.UpperLeftCorner.X );
			axisList_.at( AN_TOP )->PanAxis(panAnchor_.X, point.X - AbsoluteRect.UpperLeftCorner.X );
			axisList_.at( AN_RIGTH )->PanAxis(panAnchor_.Y, point.Y - AbsoluteRect.UpperLeftCorner.Y );
			axisList_.at( AN_LEFT ) ->PanAxis(panAnchor_.Y, point.Y - AbsoluteRect.UpperLeftCorner.Y );

			panAnchor_ = point - AbsoluteRect.UpperLeftCorner;

			Update();
		}
	}

	if (leftMouseDown_ && zoomEnabled_)
	{
		rectZoomArea_.LowerRightCorner = point - AbsoluteRect.UpperLeftCorner;
	}	
}
//////////////////////////////////////////////////////////////////////////

void CChartCtrl::Update()
{
	if( GetTickCount() - timeLastUpdate_ < 30 )
		return;

	timeLastUpdate_ = GetTickCount();

	std::vector<CChartAxis*>::iterator aIter = axisList_.begin();
	for(; aIter != axisList_.end(); aIter++ )
	{
		(*aIter)->SetAxisSize( AbsoluteRect, core::recti( 0, 0, RelativeRect.getWidth(), RelativeRect.getHeight() ));
		(*aIter)->Recalculate();
	}

	std::vector<CChartSerie*>::iterator pIter = seriesList_.begin();
	for(; pIter != seriesList_.end(); pIter++ )
	{
		core::recti drawingRect = core::recti( 0, 0, RelativeRect.getWidth(), RelativeRect.getHeight() );
		drawingRect.UpperLeftCorner.X += 1;
		drawingRect.LowerRightCorner.Y += 1;
		
		(*pIter)->SetRect( drawingRect );
		(*pIter)->Update();
	}
}

void CChartCtrl::OnRButtonDown_( core::position2di& point ) 
{
	rMouseDown_ = true;

	if (panEnabled_)
		panAnchor_ = point - AbsoluteRect.UpperLeftCorner;
}
//////////////////////////////////////////////////////////////////////////

void CChartCtrl::OnRButtonUp_( core::position2di& point ) 
{
	rMouseDown_ = false;
}
//////////////////////////////////////////////////////////////////////////

void CChartCtrl::OnLButtonDown_( core::position2di& point) 
{
	if (zoomEnabled_)
	{
		leftMouseDown_ = true;
		rectZoomArea_.UpperLeftCorner = point - AbsoluteRect.UpperLeftCorner;
		rectZoomArea_.LowerRightCorner = point - AbsoluteRect.UpperLeftCorner;
	}
}
//////////////////////////////////////////////////////////////////////////

void CChartCtrl::OnLButtonUp_( core::position2di& point ) 
{
	leftMouseDown_ = false;

	if (zoomEnabled_)
	{
		if ( (rectZoomArea_.UpperLeftCorner.X > rectZoomArea_.LowerRightCorner.X ) ||
			 (rectZoomArea_.UpperLeftCorner.Y > rectZoomArea_.LowerRightCorner.Y ))
		{
			UndoPanZoom();
		}
		else if ( (rectZoomArea_.UpperLeftCorner.X != rectZoomArea_.LowerRightCorner.X ) &&
				  (rectZoomArea_.UpperLeftCorner.Y != rectZoomArea_.LowerRightCorner.Y ))
		{
			CChartAxis* pBottom = axisList_.at( AN_BOTTOM );
			double MinVal = 0;			
			double MaxVal = 0;
			
			if( pBottom->IsVisible() )
			{
				if( pBottom->IsInverted() )
				{
					MaxVal = pBottom->ScreenToValue(rectZoomArea_.UpperLeftCorner.X );
					MinVal = pBottom->ScreenToValue(rectZoomArea_.LowerRightCorner.X);
				}
				else
				{
					MinVal = pBottom->ScreenToValue(rectZoomArea_.UpperLeftCorner.X);
					MaxVal = pBottom->ScreenToValue(rectZoomArea_.LowerRightCorner.X);
				}
				pBottom->SetZoomMinMax(MinVal,MaxVal);
			}

			CChartAxis* pLeft = axisList_.at( AN_LEFT );
			if( pLeft->IsVisible() )
			{
				if (pLeft->IsInverted() )
				{
					MaxVal = pLeft->ScreenToValue(rectZoomArea_.LowerRightCorner.Y);
					MinVal = pLeft->ScreenToValue(rectZoomArea_.UpperLeftCorner.Y);
				}
				else
				{
					MinVal = pLeft->ScreenToValue(rectZoomArea_.LowerRightCorner.Y );
					MaxVal = pLeft->ScreenToValue(rectZoomArea_.UpperLeftCorner.Y );
				}
				pLeft->SetZoomMinMax(MinVal,MaxVal);
			}

			CChartAxis* pTop = axisList_.at( AN_TOP );
			if( pTop->IsVisible() )
			{
				if (pTop->IsInverted() )
				{
					MaxVal = pTop->ScreenToValue(rectZoomArea_.UpperLeftCorner.X);
					MinVal = pTop->ScreenToValue(rectZoomArea_.LowerRightCorner.Y);
				}
				else
				{
					MinVal = pTop->ScreenToValue(rectZoomArea_.UpperLeftCorner.X);
					MaxVal = pTop->ScreenToValue(rectZoomArea_.LowerRightCorner.Y);
				}
				pTop->SetZoomMinMax(MinVal,MaxVal);
			}

			CChartAxis* pRight = axisList_.at( AN_RIGTH );
			if( pRight->IsVisible() )
			{
				if (pRight->IsInverted() )
				{
					MaxVal = pRight->ScreenToValue(rectZoomArea_.LowerRightCorner.Y);
					MinVal = pRight->ScreenToValue(rectZoomArea_.UpperLeftCorner.Y);
				}
				else
				{
					MinVal = pRight->ScreenToValue(rectZoomArea_.LowerRightCorner.Y);
					MaxVal = pRight->ScreenToValue(rectZoomArea_.UpperLeftCorner.Y);
				}
				pRight->SetZoomMinMax(MinVal,MaxVal);
			}
		}
	}

	Update();
}

CChartAxis* CChartCtrl::GetAxis( CHART_AXIS_NAME axisName ) const
{
	if( (size_t)axisName >= axisList_.size() )
		return NULL;

	return axisList_[ axisName ];
}

void irr::gui::CChartCtrl::SetBackColor( video::SColor NewCol )
{
	backColor_ = NewCol;  
	backGradient_ = false;
}

}//namespace gui

}//namespace irr