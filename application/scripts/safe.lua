local base = _G

module( "safe" )

local mainWindow = nil
local company = nil
local applic = base.applic
local button = base.button
local guienv = base.guienv
local window = base.window

function FadeEnterAction()
	mainWindow.visible= true
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

function Show()
	company = applic.playerCompany
	
	if mainWindow then
		mainWindow.visible = true
	else
		mainWindow = window.fsWindow( "media/maps/safe_open.png", Hide )
		
		guienv:FadeAction( base.FADE_TIME, false, false )			
		guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
	end
	
end