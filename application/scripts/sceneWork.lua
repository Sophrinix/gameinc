--���������� ��������  ��������

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
sceneManager:DrawProgress( 100, "������ �����" )

guienv:AddTopElement( browser.window )
