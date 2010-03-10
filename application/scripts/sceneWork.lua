-- Проверка работы логов
require("logdefs")
require("elmid")
require("swork_terrainConfig")
require("swork_MainMenu")
require("swork_minimap")
require("swork_formular")
require("swork_globalmap")
require("swork_camera")
require("swork_ObjectsLoading")

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Инициализация работы с БД")
-- Проверка вспомогательных функций
LogVideo("Идет создание ландшафта", 0xDEADBEEF)

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )

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
AddFormular()
AddEventTable()
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
sceneManager:DrawProgress( 100 )

Log({src=AUTH, dev=ALL}, "Авторизация пользователя Debug" )



