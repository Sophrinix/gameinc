local base = _G

module( "gpmSoundPage" )

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

local function _ShowAvaibleSoundQualityAndSoundTech()
	local companyName = company.name
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		local tg = tech.techGroup
		local techCompany = tech.company
		
		if (techCompany.empty or techCompany.name == companyName) and 
			(tg == base.PT_SOUNDTECH or tg == base.PT_SOUNDQUALITY ) then
				 base.gpmFunctions.LinkBox( flick, tech.name, tech.techGroup, tech, base.DRG, nil, nil )
		end
	end	
end

local function _Set( mp, sender )
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if CheckType( mp, sender, base.PT_SOUNDQUALITY ) then 
		project.soundQuality = sender.data
		redraw = true
	elseif CheckType( mp, sender, base.PT_SOUNDTECH ) then
		project:AddTech( sender.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmSoundPage.Show()
	end
end

local function _Unset( mp, sender )
	mp = CLuaLinkBox( mp )
	local redraw = false
	
	if base.PT_SOUNDTECH == mp.type then 
		project:RemoveTech( mp.data )
		redraw = true
	elseif base.PT_SOUNDQUALITY == mp.type then 
		project.soundQuality = nil
		redraw = true
	end
	
	if redraw then 
		guienv:SetDragObject( nil, "" )
		base.gpmSoundPage.Show()
	end
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	gpm.UpdateProjectWindow( "sound" )

	_ShowAvaibleSoundQualityAndSoundTech()
		
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	
	local sq = project.soundQuality
	local lnk = base.gpmFunctions.LinkBox( flick, sq.name, base.PT_SOUNDQUALITY, sq, base.NDRG, _Set, _Unset )
	lnk.defaultTexture = "media/buttons/SoundQualityNoImage.png"
		
	local maxProjectSoundTech = project:GetNumber( base.PT_SOUNDTECH )
	
	for i=0, maxProjectSoundTech do
		local tech = project:GetTech( i, base.PT_SOUNDTECH )
		local name = "Sound " .. i .. "/" .. maxProjectSoundTech
		
		if not tech.empty then name = tech.name end

		lnk = base.gpmFunctions.LinkBox( flick, name, base.PT_SOUNDTECH, tech, base.NDRG, _Set, _Unset )
		lnk.defaultTexture = "media/buttons/soundTechNoImage.png"
	end	
	gpm.ShowParams()
end
