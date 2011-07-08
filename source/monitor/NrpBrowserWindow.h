#pragma once
#include "NrpWindow.h"

namespace irr
{

namespace gui
{

class IGUIImage;

class CNrpBrowserWindow : public CNrpWindow
{
public:
	CNrpBrowserWindow( gui::IGUIEnvironment* env, 
					   video::ITexture* texture, 
					   s32 id, 
					   core::position2di pos, 
					   core::dimension2du size );

	~CNrpBrowserWindow(void);

	bool OnEvent(const SEvent& event);
	void SetTexture( video::ITexture* texture);
	void draw();
private:
	CNrpBrowserWindow(void);
    void _ApplyViewerStyle( core::dimension2du size, CImageGUISkin* skin );

	gui::IGUIImage* _image;
};

}//namespace gui

}//namespace irr