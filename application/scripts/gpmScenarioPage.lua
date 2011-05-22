local base = _G

module( "gpmScenarioPage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil

local function _SetScenario( _, sender )
	sender = base.CLuaLinkBox( sender )
	project.scenario = sender.data
	guienv:SetDragObject( nil, "" )
	base.gpmScenarioPage.Show()
end

local function _SetLicense( _, sender )
	sender = base.CLuaLinkBox( sender )
	project.license = sender.data
	guienv:SetDragObject( nil, "" )
	base.gpmScenarioPage.Show()
end

local function _UnsetScenario( mp, _ )
	mp = base.CLuaLinkBox( mp )
	project.scenario = nil
	guienv:SetDragObject( nil, "" )
	base.gpmScenarioPage.Show()
end

local function _UnsetLicense( mp, _ )
	mp = base.CLuaLinkBox( mp )
	project.license = nil
	guienv:SetDragObject( nil, "" )
	base.gpmScenarioPage.Show()
end

local function _ShowAvaibleScenarioAndLicense()
	local companyName = applic.playerCompany.name
	local maxScenarioNum = applic.techNumber
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )	
	
	for i=1, maxScenarioNum do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech.company
		local tg = tech.techGroup
		
		if ( techCompany.empty or techCompany.name == companyName) and
		   ( tg == base.PT_SCENARIO or tg == base.PT_LICENSE )
		then
			local refunc
			if tg == base.PT_SCENARIO 
				then refunc = _SetScenario 
				else refunc = _SetLicense 
			end
				
			local lnk = base.gpmFunctions.LinkBox( flick, tech.name, tg, tech,
												   base.DRG, refunc, nil )		 
		end
	end
end

function Show()
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	gpm.UpdateProjectWindow( "scenario" )

	_ShowAvaibleScenarioAndLicense()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	--Log("SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = project.scenario
	local lnk = base.gpmFunctions.LinkBox( flick, base.STR_SCENARIO, base.PT_SCENARIO, tech, 	
										   base.NDRG, _SetScenario, _UnsetScenario )
	lnk.defaultTexture = "media/buttons/scenarioNoImage.jpg"
	base.table.insert( gpm.links, lnk )
	
	if not tech.empty then lnk.text = tech.name end
	
	local lic = project.license
	lnk = base.gpmFunctions.LinkBox( flick, base.STR_LICENSE, base.PT_LICENSE, lic, 		
									 base.NDRG, _SetLicense, _UnsetLicense )
	lnk.defaultTexture = "media/buttons/licenseNoImage.jpg"
	base.table.insert( gpm.links, lnk )

	if not lic.empty then linkLicense.text = lic.name end
	
	--Log( "SCRIPT-CREATESL:CreateScenarioLicensePage end " )
	gpm.ShowParams()
end
