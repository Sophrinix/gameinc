local base = _G

module( "projectSelect" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local mainWindow = nil

btnEngine = nil
btnGame = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "projectSelect.FadeExitAction()" )	
end

function Show()
	tutorial.Update( tutorial.STEP_CREATE_NEW_PROJECT )
	
	local hTemp = (scrHeight / 6).."+"
	
	mainWindow = guienv:AddWindow( "media/maps/director_cabinet_slider.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	mainWindow:SetDraggable( false )
	mainWindow:SetVisible( false )
	mainWindow:GetCloseButton():SetVisible( false )
	
	btnEngine = button.Stretch( "25%", "50%", hTemp, hTemp, "newEngine", 
								mainWindow, -1, base.STR_NEW_GAME_ENGINE, "./projectSelect.OpenGameProject()" )
	btnGame = button.Stretch( "75%", "50%", hTemp, hTemp, "newGame", 
							  mainWindow, -1, base.STR_NEW_GAME, "./gameprojectManager.Show()" )
	button.Stretch( "80e", "80e", "0e", "0e", "button_down",  mainWindow, -1, "", "./projectSelect.Hide()" )
					
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