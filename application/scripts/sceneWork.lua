-- Проверка работы логов
require("logdefs")
require("elmid")
IncludeScript("swork_terrainConfig")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("swork_camera")
IncludeScript("swork_ObjectsLoading")
IncludeScript("swork_wndProjectWizard")
IncludeScript("swork_startDefinition")

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Инициализация работы с БД")
-- Проверка вспомогательных функций
LogVideo("Идет создание ландшафта", 0xDEADBEEF)

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local app = CLuaApplication( NrpGetApplication() )

citySceneObjects = { } 
bankSceneObjects = { }
univerSceneObjects = { }
officeSceneObjects = { }

--создание рабочей сцены 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--добавление камеры к сцене
AddCamera()
sceneManager:DrawProgress( 25 )

--добавление ланшафта к сцене
AddTerrain()
sceneManager:DrawProgress( 50 )

--создание главного меню
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--создание таблицы целей
AddStoreWindow()
sceneManager:DrawProgress( 77 )

--создание миникарты
sceneManager:DrawProgress( 85 )

--создание неба
sceneManager:AddSkyDomeSceneNode( "media/sky/panorama.jpg", 20, 12, 1, 2.0, 10000 )
sceneManager:DrawProgress( 90 )

--создание двумерной карты
--AddGlobalMap()
sceneManager:DrawProgress( 95 )

AddObjectsToCityScene()
AddObjectsToBankScene()
AddObjectsToOfficeScene()
AddObjectsToUniverScene()
sceneManager:DrawProgress( 100 )

AddStartPlayerDef()
AddStartCompanyTechnology()
AddStartVideoContentTechnology()
AddStartSoundContentTechnology()
AddStartScenarioContentTechnology()
AddStartEmployer()
DebugFunctionCall( AddStartPublicTechnology )

sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )