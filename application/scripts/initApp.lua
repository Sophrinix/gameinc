require("logdefs")
require("elmid")

IncludeScript( "genres" )
IncludeScript( "NerpaActionType" )
IncludeScript( "localization/russian" )
IncludeScript( "globalVariableInitialize" )
IncludeScript( "Tutorial" )

--NrpLoadPlugins( "Plugins/" )

--�������� ����� ����������� ������������
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--�������� ������� �����
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--���������� ��������� �����
NrpSetNextScene( "sceneLogin" )

applic:AddResourceDirectory( "media/maps" )
applic:AddResourceDirectory( "media/textures" )