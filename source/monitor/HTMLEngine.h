#pragma once
#include <irrlicht.h>
#include <llmozlib2.h>

namespace irr
{
	namespace gui
	{
		class CNrpBrowserWindow;
	}

	namespace video
	{
		class ITexture;
	}
}

namespace nrp
{

class HTMLEngine : public LLEmbeddedBrowserWindowObserver
{
public:
	static HTMLEngine& Instance();
	irr::gui::CNrpBrowserWindow& GetBrowserWindow(irr::core::dimension2du size);
	void SetBrowserWindow( irr::gui::CNrpBrowserWindow* newWindow ) { browserWindow_ = newWindow; }
	void Update();

	void onPageChanged( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onNavigateBegin( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onNavigateComplete( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onUpdateProgress( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onStatusTextChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onLocationChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onClickLinkHref( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn );
	void Navigate( std::string url );
	void MouseDown( size_t x, size_t y );
	void MouseUp(size_t x, size_t y);
	void ScrollByLines( int lines );
	void SetFocus();
	void KeyPress( int key );

private:

	HTMLEngine();
	HTMLEngine(const HTMLEngine &other);
	~HTMLEngine();
	irr::video::ITexture* pageTexture_;
	int browserWindowId_;
	size_t browserRasterSize_;
	size_t lastTimeUpdate_;
	irr::gui::CNrpBrowserWindow* browserWindow_;

	bool dataUpdated_;
	LLMozLib* llmozlib_;

	CRITICAL_SECTION rasterCs_;
};

}