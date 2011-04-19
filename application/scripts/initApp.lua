require("logdefs")
require("elmid")

IncludeScript( "genres" )
IncludeScript( "NerpaActionType" )
IncludeScript( "localization/russian" )
IncludeScript( "globalVariableInitialize" )
IncludeScript( "Tutorial" )

--NrpLoadPlugins( "Plugins/" )

--загрузка сцены авторизации пользователя
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--загрузка рабочей сцены
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--устнановка начальной сцены
NrpSetNextScene( "sceneLogin" )

applic:AddResourceDirectory( "media/maps" )
applic:AddResourceDirectory( "media/textures" )