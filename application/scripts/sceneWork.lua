--безопасная загрузка  скриптов

IncludeScript("button")
IncludeScript("window")
IncludeScript("pda")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("city")
IncludeScript("inventionManager") 
IncludeScript("gameboxManager")
IncludeScript("devRoom" )
IncludeScript("diskManager")
IncludeScript("saleManager")
IncludeScript("testDef" )
IncludeScript("testerComp" )
IncludeScript("bridge" )
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
