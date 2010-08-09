-- Проверка работы логов
IncludeScript("swork_terrainConfig")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("swork_camera")
IncludeScript("swork_ObjectsLoading")
IncludeScript("swork_wndProjectWizard")
IncludeScript("swork_wndEmployersManage")
IncludeScript("swork_wndShop")
IncludeScript("swork_InventionManager") 

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Инициализация работы с БД")
-- Проверка вспомогательных функций
LogVideo("Идет создание ландшафта", 0xDEADBEEF)

--создание рабочей сцены 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--создание главного меню
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--создание окна отображения содержимого портфеля
AddPortfelleWindow()
sceneManager:DrawProgress( 77 )

--создание неба
--sceneManager:AddSkyDomeSceneNode( "media/sky/panorama.jpg", 20, 12, 1, 2.0, 10000 )
--sceneManager:DrawProgress( 90 )

--создание пользователей
applic:CreateNewFreeUsers()
sceneManager:DrawProgress( 95 )

ApplicationLoadCityScene()
sceneManager:DrawProgress( 100 )

local playerCompany = CLuaCompany( applic:GetPlayerCompany() )
playerCompany:AddLuaFunction( COMPANY_READY_PROJECT, "sworkPlayerCompanyReadyProject" )
--[[for i=1, applic:GetCompanyNumber() do
	local cmp = CLuaCompany( applic:GetCompany( i - 1 ) )
	cmp:AddLuaFunction( COMPANY_READY_PROJECT, "sworkCompanyReadyProject" )
end
--]]

if startGameWithTutorial then
	StartDescriptionGame()
end