local base = _G

module( "gpmLanguagesPage" )

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

local _flickLanguages = nil
local _flickAllLanguages = nil

local function _SetLang( mp, sender )
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if CheckType( mp, sender, base.PT_LANGUAGE ) then 
		project:AddTech( sender.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmLanguagesPage.Show()
	end
end

local function _UnsetLang( mp, _ )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if base.PT_LANGUAGE == mp.type then
		project:RemoveTech( mp.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmLanguagesPage.Show()
	end
end

local function _ShowAllLanguages()		
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech.techGroup == base.PT_LANGUAGE then
			local lnk = base.gpmFunctions.LinkBox( _flickAllLanguages, tech.name, base.PT_LANGUAGE, tech, base.DRG, nil, nil )
		end
	end
end

local function _ShowAddedLanguages()		
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech.techGroup == base.PT_LANGUAGE and project:IsMyTech( tech ) then
			local lnk = base.gpmFunctions.LinkBox( _flickLanguages, tech.name, base.PT_LANGUAGE, tech, base.NDRG, _SetLang, _UnsetLang )
			base.table.insert( gpm.links, lnk )
		end
	end
	
	base.gpmFunctions.LinkBox( _flickLanguages, "Добавить", base.PT_LANGUAGE, nil, base.NDRG, _SetLang, _UnsetLang )
	base.table.insert( gpm.links, lnk )
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	
	gpm.UpdateProjectWindow( "languages" )

	_flickLanguages = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	_flickAllLanguages = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	_ShowAddedLanguages()		
	_ShowAllLanguages()
	
	gpm.ShowParams()
end
