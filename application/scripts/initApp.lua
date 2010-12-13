require("elmid")
IncludeScript( "logdefs" )
IncludeScript( "genres" )
IncludeScript( "NerpaActionType" )
IncludeScript( "localization/russian" )
IncludeScript( "globalVariableInitialize" )
IncludeScript( "Tutorial" )

NrpLoadPlugins( "Plugins/" )

--�������� ����� ����������� ������������
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--�������� ������� �����
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--���������� ��������� �����
NrpSetNextScene( "sceneLogin" )

--test_OutputDebugString()