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
IncludeScript("devRoom" )
IncludeScript("office")
IncludeScript("gameprojectManager")
IncludeScript("monitor")
IncludeScript("pda")
IncludeScript("journals")
IncludeScript("diskManager")
IncludeScript("saleManager")
IncludeScript("safe")
IncludeScript( "testDef" )
IncludeScript( "testerComp" )
IncludeScript( "topGame" )
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

alwaysOnTopWindow[ 1 ] = mainmenu.GetWindow()
alwaysOnTopWindow[ 2 ] = CLuaWindow( browser.window )
alwaysOnTopWindow[ 3 ] = pda.GetWindow()

IncludeScript( "callbacks" )
