--����������� ���������� ���������� ��� ������
guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
applic = CLuaApplication( NrpGetApplication() )
driver = CLuaDriver( NrpGetVideoDriver() )
sceneManager = CLuaSceneManager( NrpGetSceneManager() )

scrWidth, scrHeight = driver:GetScreenSize()
--����� ������ ���������� ����������

