--���������� ��������  ��������

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

--�������� ������� ����� 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0, "������������� �����" )

--�������� �������� ����
mainmenu.Show()
sceneManager:DrawProgress( 70, "������ ����" )

--�������� ���� ����������� ����������� ��������
AddPortfelleWindow()
sceneManager:DrawProgress( 77, "��������� �������" )

city.Show()
city.SetVisible( false )
guienv:FadeAction( 100, FADE_OUT, REMOVE_ON_END )
sceneManager:DrawProgress( 100, "������ �����" )

guienv:AddTimer( 1000, "city.SetVisible( true )" )
guienv:AddTimer( 1000, "guienv:FadeAction( 2000, FADE_IN, true )" )

alwaysOnTopWindow[ 1 ] = mainmenu.GetWindow()
alwaysOnTopWindow[ 2 ] = CLuaWindow( browser.window )
alwaysOnTopWindow[ 3 ] = pda.GetWindow()

IncludeScript( "callbacks" )
