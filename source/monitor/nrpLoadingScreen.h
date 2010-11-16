#pragma once
#include "NrpText.h"
class CVideoOptions;

namespace irr
{

namespace scene
{

class CLoadingScreen
{
public:
	CLoadingScreen( video::IVideoDriver* driver, gui::IGUIFont* font );
	~CLoadingScreen(void);

	void render( int progress, const nrp::NrpText& text );
private:
	nrp::NrpText background_,
				  loading_bar_,
				  fillbar_;

	video::IVideoDriver* _driver;
	gui::IGUIFont* _font;
};

}//namespace scene

}//namespace irr