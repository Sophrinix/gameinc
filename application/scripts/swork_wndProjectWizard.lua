IncludeScript( "swork_wndCreateGameProject" )

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 

local width = 800
local height = 600

function sworkCreateWindowWizardProject( ptr )
	
	local windowg = CLuaWindow( guienv:AddWindow( "ProjectWizzard", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_PROJECTWIZ_NAME )
	
	local button = CLuaButton( guienv:AddButton( 10, 40, 10 + 140, 40 + 20, windowg:Self(), -1, "Проект видео движка" ) )
	button:SetAction( "sworkCreateGameEngineProject" )

	button:SetObject( guienv:AddButton( 10, 70, 10 + 140, 70 + 20, windowg:Self(), -1, "Проект скриптового движка" ) )
	button:SetAction( "sworkCreateScriptEngineProject" )
	
	button:SetObject( guienv:AddButton( 10, 100, 10 + 140, 100 + 20, windowg:Self(), -1, "Проект физического движка" ) )
	button:SetAction( "sworkCreatePhisicsEngineProject" )
	
	button:SetObject( guienv:AddButton( 10, 130, 10 + 140, 130 + 20, windowg:Self(), -1, "Проект звукового движка" ) )
	button:SetAction( "sworkCreateSoundEngineProject" )
	
	button:SetObject( guienv:AddButton( 10, 160, 10 + 140, 160 + 20, windowg:Self(), -1, "Проект игры" ) )
	button:SetAction( "sworkCreateGameProject" )
	
	button:SetObject( guienv:AddButton( 10, 190, 10 + 140, 190 + 20, windowg:Self(), -1, "Сценарий" ) )
	button:SetAction( "sworkCreateScenario" )

end