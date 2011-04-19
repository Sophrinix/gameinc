local base = _G

IncludeScript("projectManager")	
IncludeScript("userManager")
IncludeScript("houseSelect" )

module( "monitor" )

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local window = base.window

local mainWindow = nil
local width = 80

function Hide()
	base.package.loaded[ "projectManager" ] = false
	base.package.loaded[ "userManager" ] = false
	base.package.loaded[ "houseSelect" ] = false
end

function Show()
	company = applic.playerCompany
	
	mainWindow = window.fsWindow( "media/textures/monitor.png", Hide )
	local flick = guienv:AddFlick( 150, 90, "150e", "90e", 4, -1, mainWindow )
	
	button.Stretch( 0, 0, width, width, "m_inDevelop", flick, -1, base.STR_DEVPROJECTS, base.projectManager.Show )
	--button.Stretch( 0, 0, width, width, "m_buhgalter", flick, -1, base.STR_BUHGALTERIA, buhgalter.Show )
	button.Stretch( 0, 0, width, width, "m_personal", flick, -1, base.STR_PERSONAL, base.userManager.Show )
	button.Stretch( 0, 0, width, width, "m_houseSelect", flick, -1, base.STR_HOUSE_SELECT, base.houseSelect.Show )
end