--���������� ��������  ��������
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
IncludeScript("rightPanel")

--�������� ������� ����� 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 75, "������������� �����" )

guienv:SetCursor( "media/textures/MouseCursor.png" )

--�������� �������� ����
mainmenu.Show()
--������ ������ � ���������
rightPanel.Show()
sceneManager:DrawProgress( 80, "������ ���������������� ���������" )

--�������� ���� ����������� ����������� ��������
AddPortfelleWindow()
sceneManager:DrawProgress( 90, "��������� ��������" )

city.Show()
sceneManager:DrawProgress( 100, "������ �����" )

guienv:AddTopElement( browser.window )

--����� ������� �� ���������� ��� ��������� �������� ����
sceneManager:Bind( SCENE_KEY_INPUT_EVENT, sworkKeyboardEvent )

--��������� ����� ���� ����� ������� �����
sceneManager:Bind( SCENE_AFTER_RENDER, function() 
												 applic:Update()
												 mainmenu.UpdateTime() 
				 				      end )

applic:Bind( APP_DAY_CHANGE, sworkAppDayChange )
applic:Bind( APP_MONTH_CHANGE, sworkAppMonthChange )
applic:Bind( APP_YEAR_CHANGE, sworkAppYearChange )
applic:Bind( APP_INVENTION_FINISHED, sworkInventionFinished )
applic:Bind( APP_MODULE_FINISHED, sworkModuleFinished )
applic:Bind( APP_REKLAME_FINISHED, sworkReklameFinished )
applic:Bind( APP_PRODUCE_FINISHED, sworkProduceFinished )
applic:Bind( APP_USER_MARKETUPDATE, sworkUserMarketUpdated )
applic:Bind( APP_PROJECT_FINISHED, sworkPlayerCompanyReadyProject )
applic:Bind( APP_MODULE_TESTED, sworkModuleTestedFinished )
applic:Bind( APP_NEWGAME_ONMARKET, sworkNewGameOnMarket )