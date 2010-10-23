#pragma once
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

	void render( int progress, const core::stringw& text );
private:
	core::stringc background_,
				  loading_bar_,
				  fillbar_;

	video::IVideoDriver* _driver;
	gui::IGUIFont* _font;
};

}//namespace scene

}//namespace irr