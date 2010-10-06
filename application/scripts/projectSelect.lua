local base = _G

module( "projectSelect" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local mainWindow = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "projectSelect.FadeExitAction()" )	
end

function Show()
	tutorial.Update( tutorial.STEP_CREATE_NEW_PROJECT )
	
	local hTemp = scrHeight / 6
	
	mainWindow = guienv:AddWindow( "media/maps/director_cabinet_slider.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	mainWindow:SetDraggable( false )
	mainWindow:SetVisible( false )
	mainWindow:GetCloseButton():SetVisible( false )
	
	button.Stretch( scrWidth / 4 - hTemp, scrHeight / 2 - hTemp, 
					scrWidth / 4 + hTemp, scrHeight / 2 + hTemp,									 
					"newEngine", mainWindow:Self(), -1, base.STR_NEW_GAME_ENGINE, "./projectSelect.OpenGameProject()" )

	button.Stretch( scrWidth * 0.75 - hTemp, scrHeight / 2 - hTemp, 
					scrWidth * 0.75 + hTemp, scrHeight / 2 + hTemp, 
					"newGame", mainWindow:Self(), -1, base.STR_NEW_GAME, "./gameprojectManager.Show()" )
	
	button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
					"button_down",  mainWindow:Self(), -1, "", "./projectSelect.Hide()" )
					
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "projectSelect.FadeEnterAction()" )
end

function OpenGameProject()
	engineprojectManager.Show()
	mainWindow:Remove()
	mainWindow = nil
end

function FadeEnterAction()
	mainWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end