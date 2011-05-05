#ifndef CNRP_APPLICATION_H_
#define CNRP_APPLICATION_H_

#include "INrpObject.h"
#include <map>


class CNrpAppEventer;

namespace irr
{
	class IrrlichtDevice;

	namespace video
	{
		class IVideoDriver;
	}

	namespace scene
	{
		class ISceneManager;
		class INrpScene;
	}

	namespace gui
	{
		class IGUIEnvironment;
		class CNrpConsole;
	}

	namespace io
	{
		class IFileSystem;
	}
}

namespace nrp
{
	class INrpConfig;
}

class CNrpEngine : public nrp::INrpObject
{
	friend class CNrpAppEventer;
public:
	typedef enum { STATUS_UNKNOWN=0, CREATE_OK, CREATE_ERR, DRAW, LOAD_SCENE, CLOSE  } RUNNING_STATUS;
	typedef enum { UNKONOW_SCENE=0, LOGIN_SCENE, WORK_SCENE } SCENE_CLASS;

	static CNrpEngine& Instance();
	
	bool InitVideo();
	bool InitConsole();
	void InitializeScene( SCENE_CLASS who );

	RUNNING_STATUS GetStatus() { return status_; }
	void SetStatus( RUNNING_STATUS new_status, void* data );

	void Run();

	//irr::IrrlichtDevice* GetDevice() { return device_; }
	irr::video::IVideoDriver* GetVideoDriver();	//Текущий видеодрайвер 
	irr::scene::ISceneManager* GetSceneManager();  //текущий менеджер сцен
	irr::gui::IGUIEnvironment* GetGuiEnvironment();
	irr::io::IFileSystem*  GetFileSystem();
	irr::gui::CNrpConsole* GetConsole() { return console_; }
	irr::IrrlichtDevice* GetDevice();

	HWND GetWindowHandle() { return windowHandle_; }
	void AddScene( const NrpText& name, const NrpText& fileName );

	// by d.a, для управления объектами на сцене
	irr::scene::INrpScene* GetCurrentScene() const;

	static NrpText ClassName();
private:
	// закрыл конструкторы (синглетон-акцесс)
	CNrpEngine();
	CNrpEngine(CNrpEngine&); // не реализован
	CNrpEngine& operator=(CNrpEngine&); // не реализован
	
	void CreateScreenShot_();
	void ActivateScene_( const NrpText& name );

	irr::IrrlichtDevice* device_;											//устройство видео вывода 
	irr::scene::INrpScene* currentScene_;
	RUNNING_STATUS status_;

	bool engine_started_, terminated_;
	HWND windowHandle_;
	RUNNING_STATUS run_state_;
	NrpText next_scene_;
	irr::gui::IGUIEnvironment* guienv_;
	irr::gui::CNrpConsole* console_;

	std::map< NrpText, NrpText > scenes_;
};

#define _nrpEngine CNrpEngine::Instance()

#endif /* CNRP_APPLICATION_H_ */