local base = _G

module( "projectSelect" )

local guienv = base.guienv
local window = base.window
local button = base.button
local tutorial = base.tutorial

local mainWindow = nil

btnEngine = nil
btnGame = nil

function Hide()
	base.package.loaded[ "gameprojectManager" ] = nil
end

function StartEngineProject()
	--engineprojectManager.Show()
	--Hide()
end

function StartGameProject()
	base.IncludeScript("gameprojectManager")
	base.gameprojectManager.Show()
	--Hide()
end

function FadeEnterAction()
	mainWindow.visible = true
	--guienv:FadeAction( base.FADE_TIME, true, true )
end

function Show()
	tutorial.Update( tutorial.STEP_CREATE_NEW_PROJECT )
	
	mainWindow = window.fsWindow( "director_cabinet_slider.png", Hide )
	
	local layout = guienv:AddLayout( "33%", "25%", "50%+", "33%+", 2, -1, mainWindow )
	btnEngine = button.LayoutButton( "newEngine", layout, -1, base.STR_NEW_GAME_ENGINE, StartEngineProject )
	btnGame = button.LayoutButton( "newGame", layout, -1, base.STR_NEW_GAME, StartGameProject )
end