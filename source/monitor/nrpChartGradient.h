#pragma once
#include <irrlicht.h>

using namespace irr;

enum EGradientType
{
	gtHorizontal,
	gtVertical,
	gtHorizontalDouble,
	gtVerticalDouble
};

class CChartGradient
{
public:
	CChartGradient();
	~CChartGradient();

	static void Draw( video::IVideoDriver* driver, 
					  const core::recti& rectangle, video::SColor Color1, 
					  video::SColor Color2, EGradientType GradientType);
};
