local base = _G

IncludeScript("projectManager")	
IncludeScript("userManager")
IncludeScript("houseSelect" )
IncludeScript("projectChief" )
IncludeScript("companyChief" )

module( "monitor" )

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local window = base.window

local mainWindow = nil
local width = 80

local function _Hide()
	base.package.loaded[ "projectManager" ] = false
	base.package.loaded[ "userManager" ] = false
	base.package.loaded[ "houseSelect" ] = false
	base.package.loaded[ "projectChief" ] = false
	base.package.loaded[ "companyChief" ] = false
end

local function _ShowProjectManager()
	if company.devProjectNumber > 0 then
		base.projectManager.Show()
	else
		guienv:MessageBox( "Нет проектов в разработке", false, false, button.CloseParent, button.NoFunction )
	end
end


function Show()
	company = applic.playerCompany
	
	mainWindow = window.fsWindow( "media/textures/monitor.png", _Hide )
	local flick = guienv:AddFlick( 150, 90, "150e", "90e", 4, -1, mainWindow )
	
	button.Stretch( 0, 0, width, width, "m_inDevelop", flick, -1, base.STR_DEVPROJECTS, _ShowProjectManager )
	--button.Stretch( 0, 0, width, width, "m_buhgalter", flick, -1, base.STR_BUHGALTERIA, buhgalter.Show )
	button.Stretch( 0, 0, width, width, "m_personal", flick, -1, base.STR_PERSONAL, base.userManager.Show )
	button.Stretch( 0, 0, width, width, "btnHouseSelect", flick, -1, base.STR_HOUSE_SELECT, base.houseSelect.Show )
	button.Stretch( 0, 0, width, width, "m_ProjectChief", flick, -1, base.STR_PROJECT_CHIEF, base.projectChief.Show )
	button.Stretch( 0, 0, width, width, "m_CompanyChief", flick, -1, base.STR_COMPANY_CHIEF, base.companyChief.Show )
end