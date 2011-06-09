local base = _G

IncludeScript("projectSelect")
IncludeScript("monitor")
IncludeScript("safe")
IncludeScript("worldMap")

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

function Hide()
	base.package.loaded[ "projectSelect" ] = false
	base.package.loaded[ "monitor" ] = false
	base.package.loaded[ "safe" ] = false
	base.package.loaded[ "worldMap" ] = false
end

function ShowHelp()
	tutorial.Update( "office/director" )
end

function Show( ptr )
	mainWindow = window.fsWindow( "director_cabinet.png", Hide )
	
	base.rightPanel.AddYesNo( "Хотите больше узнать об управлении предприятием?", ShowHelp, button.CloseParent )
	
	btnProject = button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", base.projectSelect.Show )
	--employers manager
	btnEmployer = button.EqualeTexture( 805, 238, "employersManager", mainWindow, -1, "", base.userManager.Show )
	
	btnWorld = button.EqualeTexture( 620, 205, "button_world", mainWindow, -1, "", base.worldMap.Show )

	--project manager
	btnComp = button.EqualeTexture( 612, 251, "button_comp", mainWindow, -1, "", base.monitor.Show )
	
	btnSafe = button.EqualeTexture( 750, 233, "button_safe", mainWindow, -1, "", base.safe.Show )
end