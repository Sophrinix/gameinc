#include "stdafx.h"
#include "nrpChartGradient.h"

CChartGradient::CChartGradient()
{
}

CChartGradient::~CChartGradient()
{
}

void CChartGradient::Draw( video::IVideoDriver* driver, 
						   const core::recti& rectangle, 
						   video::SColor Color1, video::SColor Color2, 
						   EGradientType GradientType )
{
	switch( GradientType )
	{
	case gtVertical:
		driver->draw2DRectangle( rectangle, Color1, Color1, Color2, Color2 );
	break;

	case gtHorizontal:
		driver->draw2DRectangle( rectangle, Color1, Color2, Color2, Color1 );
	break;

	};
}
