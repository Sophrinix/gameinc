--безопасная загрузка  скриптов
IncludeScript("button")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("swork_ObjectsLoading")
IncludeScript("swork_wndProjectWizard")
IncludeScript("userManager")
IncludeScript("swork_wndShop")
IncludeScript("reklameManager")
IncludeScript("swork_InventionManager") 
IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndLoanAction")
IncludeScript("swork_wndUniverStuffUp")
IncludeScript("projectManager")	--as module
IncludeScript("swork_wndGameBoxCreate")
IncludeScript("swork_wndDiskPlant")
IncludeScript("swork_wndLaboratory" )
--------------------------------------------

alwaysOnTopWindow = {}

--создание рабочей сцены 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--создание главного меню
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--создание окна отображения содержимого портфеля
AddPortfelleWindow()
sceneManager:DrawProgress( 77 )

--создание пользователей
applic:CreateNewFreeUsers()
sceneManager:DrawProgress( 95 )

ApplicationLoadCityScene()
sceneManager:DrawProgress( 100 )

local playerCompany = applic:GetPlayerCompany()
playerCompany:AddLuaFunction( COMPANY_READY_PROJECT, "sworkPlayerCompanyReadyProject" )
--[[for i=1, applic:GetCompanyNumber() do
	local cmp = CLuaCompany( applic:GetCompany( i - 1 ) )
	cmp:AddLuaFunction( COMPANY_READY_PROJECT, "sworkCompanyReadyProject" )
end
--]]

if tutorial.startGameWithTutorial then
	tutorial.Start()
end

alwaysOnTopWindow[ 1 ] = mainMenuWindow
alwaysOnTopWindow[ 2 ] = CLuaWindow( browser:GetWindow() )