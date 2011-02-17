--безопасная загрузка  скриптов

IncludeScript("button")
IncludeScript("window")
IncludeScript("pda")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("city")
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
IncludeScript("monitor")
IncludeScript("journals")
IncludeScript("diskManager")
IncludeScript("saleManager")
IncludeScript("safe")
IncludeScript("testDef" )
IncludeScript("testerComp" )
IncludeScript("topGame" )
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
