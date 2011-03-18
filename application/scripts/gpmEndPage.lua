local base = _G

module( "gpmEndPage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil
local CLuaLinkBox = base.CLuaLinkBox

local function _CreateProjectGame()
	project.name = gpm.gameName
	
	if company:CreateDevelopGame( project ) ~= nil then
		--company:AddToPortfelle( prj )
		--sworkUpdateCompanyPortfelle()
		guienv:MessageBox( "Вы завершили создание проекта игры " .. gpm.gameName, false, false, nil, nil )
	else
		guienv:MessageBox( "Уже есть проект с таким именем", false, false, nil, nil )
	end
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow
	gpm.UpdateProjectWindow( "end" )

	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	base.gpmFunctions.LinkBox( flick, base.STR_ENGINE, 0, project.gameEngine, base.NDRG, nil, nil )
	base.gpmFunctions.LinkBox( flick, base.STR_GENRE, 0, project:GetTech( 0, base.PT_GENRE ), base.NDRG, nil, nil ) 
	base.gpmFunctions.LinkBox( flick, base.STR_SCENARIO, 0, project.scenario, base.NDRG, nil, nil )
	base.gpmFunctions.LinkBox( flick, base.STR_LICENSE, 0, project.license, base.NDRG, nil, nil )
	base.gpmFunctions.LinkBox( flick, base.STR_PLATFORMS, 0, project:GetPlatform(0, base.PT_PLATFORM ), base.NDRG, nil, nil )				 
	base.gpmFunctions.LinkBox( flick, base.STR_LOCALIZATION, 0, project:GetTech( 0, base.PT_LANGUAGE ), base.NDRG, nil, nil )
	local button = guienv:AddButton( "55%", "80%", "95%", "90%", projectWindow, -1, base.STR_FINISH )
	button.action = _CreateProjectGame
	button.enabled = project.projectReady
end
