#pragma once
#include <string>

class CVideoOptions;

namespace irr
{

namespace scene
{

class CLoadingScreen
{
public:
	CLoadingScreen( irr::video::IVideoDriver* driver );
	~CLoadingScreen(void);

	void render( int progress );
private:
	std::string background_,
				loading_bar_,
				fillbar_;

	irr::video::IVideoDriver* driver_;
};

}//namespace scene

}//namespace irr