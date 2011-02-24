local base = _G

IncludeScript("projectSelect")
IncludeScript("monitor")
IncludeScript("safe")

module( "director" )

local guienv = base.guienv
local button = base.button
local window = base.window
local tutorial = base.tutorial
local mainWindow = nil

btnComp = nil
btnProject = nil
btnEmployer = nil
btnSafe = nil

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
	
	base.package.loaded[ "projectSelect" ] = nil
	base.package.loaded[ "monitor" ] = nil
	base.package.loaded[ "safe" ] = nil
end

function Show( ptr )
	if mainWindow then
		mainWindow.visible = true
	else
		mainWindow = window.fsWindow( "media/maps/director_cabinet.png", Hide )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DIRECTORS_ROOM )
	
	btnProject = button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", base.projectSelect.Show )
	--employers manager
	btnEmployer = button.EqualeTexture( 805, 238, "employersManager", mainWindow, -1, "", base.userManager.Show )
	--project manager
	btnComp = button.EqualeTexture( 612, 251, "projectManager", mainWindow, -1, "", base.monitor.Show )
	
	btnSafe = button.EqualeTexture( 750, 233, "button_safe", mainWindow, -1, "", base.safe.Show )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end