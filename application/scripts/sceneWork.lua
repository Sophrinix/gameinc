--безопасная загрузка  скриптов

IncludeScript("button")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("city")
IncludeScript("projectSelect")
IncludeScript("userManager")
IncludeScript("shop")
IncludeScript("reklameManager")
IncludeScript("inventionManager") 
IncludeScript("bank")
IncludeScript("univer")
IncludeScript("projectManager")	
IncludeScript("gameboxManager")
IncludeScript("plant")
IncludeScript("laboratory" )
IncludeScript("office")
IncludeScript("gameprojectManager")
IncludeScript("monitor")
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

city.Show()
sceneManager:DrawProgress( 100 )

applic:GetPlayerCompany():AddLuaFunction( COMPANY_READY_PROJECT, "sworkPlayerCompanyReadyProject" )
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