
IncludeScript("button")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("swork_ObjectsLoading")
IncludeScript("swork_wndProjectWizard")
IncludeScript("swork_wndEmployersManage")
IncludeScript("swork_wndShop")
IncludeScript("swork_InventionManager") 


alwaysOnTopWindow = {}

--�������� ������� ����� 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--�������� �������� ����
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--�������� ���� ����������� ����������� ��������
AddPortfelleWindow()
sceneManager:DrawProgress( 77 )

--�������� �������������
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

if startGameWithTutorial then
	StartDescriptionGame()
end

alwaysOnTopWindow[ 1 ] = mainMenuWindow
alwaysOnTopWindow[ 2 ] = CLuaWindow( browser:GetWindow() )

