#pragma once
#include <irrlicht.h>

namespace irr
{

namespace gui
{

typedef core::stringc TChartString;

class CChartCtrl;

class CChartObject  
{
	friend CChartCtrl;

public:
	CChartObject( IGUIEnvironment* env, CChartCtrl* ptrOwner);
	virtual ~CChartObject();

	void SetVisible(bool visible);
	bool IsVisible()  const         { return visible_; }

	video::SColor GetColor() const	{ return color_; }
	void SetColor( irr::video::SColor newColor );

	bool Clip(int& dX, int& dY)  const;

	virtual void Draw() = 0;
protected:
	void SetRect( const core::recti newTect);
    
	core::recti		rectangle_;		// Size of the object
	video::SColor	color_;		// Color of the objet

	CChartCtrl*	parent_;			// Owner of the object
	bool        visible_;

	IGUIEnvironment* environment_;

};

}//namespace gui

}//namespace irr