--безопасная загрузка  скриптов

IncludeScript("button")
IncludeScript("window")
IncludeScript("pda")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("city")
IncludeScript("inventionManager") 
IncludeScript("testDef" )
IncludeScript("callbacks" )
IncludeScript("mainmenu")
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
sceneManager:DrawProgress( 100, "Создаю сцену" )

guienv:AddTopElement( browser.window )

sceneManager:AddSceneFunction( SCENE_KEY_INPUT_EVENT, sworkKeyboardEvent )

applic:AddLuaFunction( APP_DAY_CHANGE, sworkAppDayChange )
applic:AddLuaFunction( APP_MONTH_CHANGE, sworkAppMonthChange )
applic:AddLuaFunction( APP_YEAR_CHANGE, sworkAppYearChange )
applic:AddLuaFunction( APP_INVENTION_FINISHED, sworkInventionFinished )
applic:AddLuaFunction( APP_MODULE_FINISHED, sworkModuleFinished )
applic:AddLuaFunction( APP_REKLAME_FINISHED, sworkReklameFinished )
applic:AddLuaFunction( APP_USER_MARKETUPDATE, sworkUserMarketUpdated )
applic:AddLuaFunction( APP_PROJECT_FINISHED, sworkPlayerCompanyReadyProject )
applic:AddLuaFunction( APP_MODULE_TESTED, sworkModuleTestedFinished )