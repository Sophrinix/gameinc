require("elmid")
IncludeScript("logdefs")
IncludeScript( "NerpaActionType" )
IncludeScript( "HelpSubSystem" )
IncludeScript( "localization/russian" )

NrpLoadPlugins( "Plugins/" )

--�������� ����� ����������� ������������
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--�������� ������� �����
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--���������� ��������� �����
NrpSetNextScene( "sceneLogin" )

--test_OutputDebugString()