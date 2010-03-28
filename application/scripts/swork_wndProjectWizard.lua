IncludeScript( "swork_wndCreateGameProject" )

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 

local width = 800
local height = 600

function sworkCreateWindowWizardProject( ptr )
	
	local itemHeight = 30
	local i = 1
	local hTemp = height / 7
	local windowg = CLuaWindow( guienv:AddWindow( "ProjectWizzard", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_PROJECTWIZ_NAME )
	
	local button = CLuaButton( guienv:AddButton( width / 2 - 200, hTemp * i , width / 2 + 200, hTemp * i + itemHeight, windowg:Self(), -1, "Проект видео движка" ) )
	button:SetAction( "sworkCreateGameEngineProject" )
	i = i + 1

	button:SetObject( guienv:AddButton( width / 2 - 200, hTemp * i, width / 2 + 200, hTemp * i + itemHeight, windowg:Self(), -1, "Проект физического движка" ) )
	button:SetAction( "sworkCreatePhisicsEngineProject" )
	i = i + 1
	
	button:SetObject( guienv:AddButton( width / 2 - 200, hTemp * i, width / 2 + 200, hTemp * i + itemHeight, windowg:Self(), -1, "Проект звукового движка" ) )
	button:SetAction( "sworkCreateSoundEngineProject" )
	i = i + 1
	
	button:SetObject( guienv:AddButton( width / 2 - 200, hTemp * i, width / 2 + 200, hTemp * i+ itemHeight, windowg:Self(), -1, "Проект игры" ) )
	button:SetAction( "sworkCreateGameProject" )
	i = i + 1
	
	button:SetObject( guienv:AddButton( width / 2 - 200, hTemp * i, width / 2 + 200, hTemp * i + itemHeight, windowg:Self(), -1, "Сценарий" ) )
	button:SetAction( "sworkCreateScenario" )
	i = i + 1

end