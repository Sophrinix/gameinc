IncludeScript( "swork_wndCreateGameProject" )
local windowWizardProject = nil

function sworkCloseWindowWizardProject( ptr )
	windowWizardProject:Remove()
end

function sworkCreateWindowWizardProject( ptr )
	local hTemp = scrHeight / 6
	
	windowWizardProject = guienv:AddWindow( "media/director_cabinet_slider.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	windowWizardProject:SetDraggable( false )
	local closeBtn = windowWizardProject:GetCloseButton()
    closeBtn:SetVisible( false )
	
	local button = guienv:AddButton( scrWidth / 4 - hTemp, scrHeight / 2 - hTemp, 
									 scrWidth / 4 + hTemp, scrHeight / 2 + hTemp, 
									 windowWizardProject:Self(), -1, "Проект видео движка" )
    button:SetImage( 0, 0, 256, 256, "media/buttons/newEngine_normal.png" )
	button:SetHoveredImage( 0, 0, 256, 256, "media/buttons/newEngine_select.png" )	
	button:SetPressedImage( 0, 0, 256, 256, "media/buttons/newEngine_select.png" )	
	button:SetAction( "sworkCreateGameEngineProject" )

	button = guienv:AddButton( scrWidth * 0.75 - hTemp, scrHeight / 2 - hTemp, 
							   scrWidth * 0.75 + hTemp, scrHeight / 2 + hTemp, 
							   windowWizardProject:Self(), -1, "Проект игры" )
	button:SetImage( 0, 0, 256, 256, "media/buttons/newGame_normal.png" )
	button:SetHoveredImage( 0, 0, 256, 256, "media/buttons/newGame_select.png" )	
	button:SetPressedImage( 0, 0, 256, 256, "media/buttons/newGame_select.png" )	
	button:SetAction( "sworkCreateGameProject" )
	
	button = guienv:AddButton( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
							   windowWizardProject:Self(), -1, "" )
	button:SetImage( 0, 0, 128, 128, "media/buttons/button_down_normal.png" )
	button:SetHoveredImage( 0, 0, 128, 128, "media/buttons/button_down_select.png" )	
	button:SetPressedImage( 0, 0, 128, 128, "media/buttons/button_down_select.png" )	
	button:SetAction( "sworkCloseWindowWizardProject" )
end