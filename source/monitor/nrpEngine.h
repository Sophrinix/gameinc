#ifndef CNRP_APPLICATION_H_
#define CNRP_APPLICATION_H_

#include "INrpObject.h"
#include <map>
#include <string>

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
	CLASS_NAME CLASS_NRPENGINE( "CNrpEngine" );
}

class CNrpEngine : public nrp::INrpObject
{
	friend class CNrpAppEventer;
public:
	typedef enum { STATUS_UNKNOWN=0, CREATE_OK, CREATE_ERR, DRAW, LOAD_SCENE, CLOSE  } RUNNING_STATUS;
	typedef enum { UNKONOW_SCENE=0, LOGIN_SCENE, WORK_SCENE } SCENE_CLASS;

	static CNrpEngine& Instance();
	
	void SetConfig( nrp::INrpConfig* config );
	bool InitVideo();
	bool InitConsole();
	void InitializeScene( SCENE_CLASS who );
	nrp::INrpConfig* GetConfig( CONFIG_TYPE name_conf );


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
	void AddScene( const std::string& name, const std::string& fileName )
	{
		scenes_[ name ] = fileName;
	}

	// by d.a, для управления объектами на сцене
	irr::scene::INrpScene* GetCurrentScene() const;

	static std::string ClassName() { return nrp::CLASS_NRPENGINE; }
private:
	// закрыл конструкторы (синглетон-акцесс)
	CNrpEngine();
	CNrpEngine(CNrpEngine&); // не реализован
	CNrpEngine& operator=(CNrpEngine&); // не реализован
	
	void CreateScreenShot_();
	void ActivateScene_( std::string name );

	irr::IrrlichtDevice* device_;											//устройство видео вывода 
	irr::scene::INrpScene* currentScene_;
	RUNNING_STATUS status_;

	bool engine_started_, terminated_;
	HWND windowHandle_;
	RUNNING_STATUS run_state_;
	std::string next_scene_;
	irr::gui::IGUIEnvironment* guienv_;
	irr::gui::CNrpConsole* console_;

	std::map< std::string, nrp::INrpConfig* > config_;
	std::map< std::string, std::string > scenes_;
};

#endif /* CNRP_APPLICATION_H_ */