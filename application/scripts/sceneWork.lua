-- Проверка работы логов
require("logdefs")
require("elmid")
require("swork_terrainConfig")
require("swork_MainMenu")
require("swork_minimap")
require("swork_formular")
require("swork_camera")
require("swork_ObjectsLoading")

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Инициализация работы с БД")
-- Проверка вспомогательных функций
LogVideo("Идет создание ландшафта", 0xDEADBEEF)

local sceneManager = CLuaSceneManager( NrpGetSceneManager() 
local app = CLuaConfig( NrpGetApplication() )

citySceneObjects = { } 
bankSceneObjects = { }

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
AddStorePanel()
sceneManager:DrawProgress( 77 )

--создание миникарты
AddMiniMap()
sceneManager:DrawProgress( 85 )

--создание неба
sceneManager:AddSkyDomeSceneNode( "media/sky/panorama.jpg", 20, 12, 1, 2.0, 10000 )
sceneManager:DrawProgress( 90 )

--создание двумерной карты
--AddGlobalMap()
sceneManager:DrawProgress( 95 )

AddObjectsToCityScene()
AddObjectsToBankScene()
sceneManager:DrawProgress( 100 )

sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
local company = CLuaCompany( app:CreateCompany( "DaleTeam" ) )