require("elmid")
IncludeScript("logdefs")
IncludeScript( "NerpaActionType" )

--����������� ���������� ���������� ��� ������
guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
applic = CLuaApplication( NrpGetApplication() )
driver = CLuaDriver( NrpGetVideoDriver() )
sceneManager = CLuaSceneManager( NrpGetSceneManager() )

scrWidth, scrHeight = driver:GetScreenSize()
--����� ������ ���������� ����������

NrpLoadLanguageFile( "localization/russian.lng" )

NrpLoadPlugins( "Plugins/" )

--�������� ����� ����������� ������������
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--�������� ������� �����
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--���������� ��������� �����
NrpSetNextScene( "sceneLogin" )

--test_OutputDebugString()