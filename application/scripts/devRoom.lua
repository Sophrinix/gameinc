local base = _G

IncludeScript( "testerComp" )

module( "devRoom" )

local guienv = base.guienv
local button = base.button
local tutorial = base.tutorial
local window = base.window

local mainWindow = nil

function FadeEnterAction()
	mainWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
end

function Show( ptr )
	if mainWindow then
		mainWindow.visible = true
	else
		mainWindow = window.fsWindow( "media/maps/developers.png", Hide )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DEVELOPERS_ROOM )
	
	button.EqualeTexture( 820, 384, "devComp", mainWindow, -1, "", base.testerComp.Show )
	--employers manager
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end

