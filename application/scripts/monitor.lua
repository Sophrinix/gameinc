local base = _G

IncludeScript("projectManager")	
IncludeScript("userManager")

module( "monitor" )

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local LogScript = base.LogScript
local window = base.window

local mainWindow = nil
local width = 80

function Hide()
	mainWindow:Remove()
	mainWindow = nil
	
	base.package.loaded[ "projectManager" ] = nil
	base.package.loaded[ "userManager" ] = nil
end

function Show()
	company = applic.playerCompany
	
	if mainWindow == nil then
		mainWindow = window.fsWindow( "media/textures/monitor.png", Hide )
		
		local flick = guienv:AddFlick( 150, 90, "150e", "90e", 4, -1, mainWindow )
	
		button.Stretch( 0, 0, width, width, "m_inDevelop", flick, -1, base.STR_DEVPROJECTS, base.projectManager.Show )
		--button.Stretch( 0, 0, width, width, "m_buhgalter", flick, -1, base.STR_BUHGALTERIA, buhgalter.Show )
		button.Stretch( 0, 0, width, width, "m_personal", flick, -1, base.STR_PERSONAL, base.userManager.Show )
	else
		mainWindow.visible = true
	end
end