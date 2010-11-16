#pragma once
#include <irrlicht.h>
#include <llmozlib2.h>
#include "NrpText.h"

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

/*! Класс веб-движка Gecko 1.8.1
	Описывает взаимодействие с движком и выполняемые ункции
*/

class HTMLEngine : public LLEmbeddedBrowserWindowObserver
{
public:
	//*! Инициализация движка при первом вызове
	static HTMLEngine& Instance();
	
	irr::gui::CNrpBrowserWindow& GetBrowserWindow(irr::core::dimension2du size=irr::core::dimension2du( 512, 512 ) );
	void SetBrowserWindow( irr::gui::CNrpBrowserWindow* newWindow ) { browserWindow_ = newWindow; }
	void Update();

	//*! Событие, которое возникает при завершении навигации по ссылке
	void onPageChanged( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! Функция вызывается после начала навигации
	void onNavigateBegin( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! Функция вызывается после окончания навигации
	void onNavigateComplete( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onUpdateProgress( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onStatusTextChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onLocationChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! Функция вызывается, когда пользователь перешел по новой ссылке
	void onClickLinkHref( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! Функция вызывается, когда пользователь перешел по неправильносй ссылке... например http://dosomething
	void onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! Указание движку перейти по заданной ссылке
	void Navigate( const NrpText& url );
	//*! Передача в веб-движок события ЛКМ
	void MouseDown( size_t x, size_t y );
	//*! Передача в веб движок события ЛКМ
	void MouseUp(size_t x, size_t y);
	void ScrollByLines( int lines );
	void SetFocus();
	void KeyPress( int key );
	void MouseMoved( size_t x, size_t y );

	static NrpText ClassName() { return "CHtmlEngine"; }
private:

	HTMLEngine();
	HTMLEngine(const HTMLEngine &other);
	~HTMLEngine();
	//*! текстура, куда сохраняется изображение отрендериной страницы
	irr::video::ITexture* pageTexture_;
	int browserWindowId_;
	size_t browserRasterSize_;
	size_t _lastTimeUpdate;
	irr::gui::CNrpBrowserWindow* browserWindow_;

	bool dataUpdated_;
	//*! библиотека мозлибы
	LLMozLib* llmozlib_;
	bool _noFollowLinkExec, _firstMessage;

	CRITICAL_SECTION rasterCs_;
};

}