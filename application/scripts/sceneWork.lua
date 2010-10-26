--безопасная загрузка  скриптов

IncludeScript("button")
IncludeScript("mainmenu")
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
IncludeScript("pda")
IncludeScript("journals")
--------------------------------------------

--создание рабочей сцены 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0, "Инициализирую сцену" )

--создание главного меню
mainmenu.Show()
sceneManager:DrawProgress( 70, "Создаю меню" )

--создание окна отображения содержимого портфеля
AddPortfelleWindow()
sceneManager:DrawProgress( 77, "Подгружаю профиль" )

city.Show()
city.SetVisible( false )
guienv:FadeAction( 100, FADE_OUT, REMOVE_ON_END )
sceneManager:DrawProgress( 100, "Создаю сцену" )

guienv:AddTimer( 1000, "city.SetVisible( true )" )
guienv:AddTimer( 1000, "guienv:FadeAction( 2000, FADE_IN, true )" )

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
sceneManager:AddSceneFunction( SCENE_AFTER_RENDER, "sworkDrawOnTopWindows" )

applic:GetPlayerCompany():AddLuaFunction( COMPANY_READY_PROJECT, "sworkPlayerCompanyReadyProject" )
applic:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
applic:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
applic:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )
applic:AddLuaFunction( APP_INVENTION_FINISHED, "sworkInventionFinished" )
applic:AddLuaFunction( APP_MODULE_FINISHED, "sworkModuleFinished" )
applic:AddLuaFunction( APP_REKLAME_FINISHED, "sworkReklameFinished" )
applic:AddLuaFunction( APP_USER_MARKETUPDATE, "sworkUserMarketUpdated" )
--[[for i=1, applic:GetCompanyNumber() do
	local cmp = applic:GetCompany( i - 1 )
	cmp:AddLuaFunction( COMPANY_READY_PROJECT, "sworkCompanyReadyProject" )
end
--]]

if tutorial.startGameWithTutorial then
	tutorial.Start()
end

alwaysOnTopWindow[ 1 ] = mainmenu.GetWindow()
alwaysOnTopWindow[ 2 ] = CLuaWindow( browser:GetWindow() )
alwaysOnTopWindow[ 3 ] = pda.GetWindow()

IncludeScript( "callbacks" )
