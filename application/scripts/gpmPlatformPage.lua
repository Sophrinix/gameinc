local base = _G

module( "gpmPlatformPage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil
local CLuaLinkBox = base.CLuaLinkBox
local CheckType = base.gpmFunctions.CheckType

local _flickPlatform = nil
local _flickAllPlatform = nil

local function _ShowMistakeForPlatform( video, sound, cpuPercent, memPercent )
	guienv:MessageBox( "Not all tech from engine are avaible", 
	                   false, false, 
	                   button.CloseParent, button.NoFunction )
end

local function _CheckPlatformsForProject( pl )
	local result = true
	local vTechAv = {}
	for i=1, project:GetNumber( base.PT_VIDEOTECH ) do
		local tech = project:GetTech( i-1, base.PT_VIDEOTECH )
		vTechAv[ i ] = { name=tech.name, avaible=false }
		vTechAv[ i ].avaible = pl:IsMyTech( tech )
		result = result and vTechAv[ i ].avaible
	end
	
	local sTechAv = {}
	for i=1, project:GetNumber( base.PT_SOUNDTECH ) do
		local tech = project:GetTech( i-1, base.PT_SOUNDTECH )
		sTechAv[ i ] = { name=tech.name, avaible=false }
		sTechAv[ i ].avaible = pl:IsMyTech( tech )
		result = result and sTechAv[ i ].avaible
	end
	
	local indexCpu = project.cpu / pl.cpu
	local indexMem = project.memory / pl.ram
	
	if not result then _ShowMistakeForPlatform( vTechAv, sTechAv, indexCpu, indexMem ) end
	return result
end

local function _SetPl( mp, sender )
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	
	local redraw = false
	if CheckType( mp, sender, base.PT_PLATFORM ) then
		project:AddPlatform( sender.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmPlatformPage.Show()
	end
end

local function _UnsetPl( mp, _ )
	mp = base.CLuaLinkBox( mp )
	project:RemovePlatform( mp.data )
	guienv:SetDragObject( nil, "" )
	base.gpmPlatformPage.Show()
end

local function _ShowAllPlatforms()
	for i=1, applic.platformNumber do
		local platform = applic:GetPlatform( i-1 )
		
		local lnk = base.gpmFunctions.LinkBox( _flickAllPlatform, platform.name, base.PT_PLATFORM, platform, base.DRG, nil, nil )
	end
end

local function _ShowAddedPlatforms()
	for i=1, applic.platformNumber do
		local platform = applic:GetPlatform( i-1 )
		
		if project:IsMyPlatform( platform ) then
			local lnk = base.gpmFunctions.LinkBox( _flickPlatform, platform.name, base.PT_PLATFORM, platform, base.NDRG, _SetPl, _UnsetPl )
			base.table.insert( gpm.links, lnk )
		end
	end
	
	local lnk = base.gpmFunctions.LinkBox( _flickPlatform, "Добавить", base.PT_PLATFORM, nil, base.NDRG, _SetPl, _UnsetPl )
	base.table.insert( gpm.links, lnk )
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	
	gpm.UpdateProjectWindow( "platform" )

	_flickPlatform = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	_flickAllPlatform = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	_ShowAddedPlatforms()
	_ShowAllPlatforms()
	
	gpm.ShowParams()
end
