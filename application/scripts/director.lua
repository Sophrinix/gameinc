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

function Hide()
	base.package.loaded[ "projectSelect" ] = false
	base.package.loaded[ "monitor" ] = false
	base.package.loaded[ "safe" ] = false
end

function ShowHelp()
	tutorial.Update( "office/director" )
end

function Show( ptr )
	mainWindow = window.fsWindow( "media/maps/director_cabinet.png", Hide )
	
	base.rightPanel.AddYesNo( "Хотите больше узнать об управлении предприятием?", ShowHelp, button.CloseParent )
	
	btnProject = button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", base.projectSelect.Show )
	--employers manager
	btnEmployer = button.EqualeTexture( 805, 238, "employersManager", mainWindow, -1, "", base.userManager.Show )
	--project manager
	btnComp = button.EqualeTexture( 612, 251, "projectManager", mainWindow, -1, "", base.monitor.Show )
	
	btnSafe = button.EqualeTexture( 750, 233, "button_safe", mainWindow, -1, "", base.safe.Show )
end