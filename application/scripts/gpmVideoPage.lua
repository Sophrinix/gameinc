local base = _G

module( "gpmVideoPage" )

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

local function _ShowAvaibleVideoQualityAndVideoTech()
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		local tg = tech.techGroup
		local techCompany = tech.company
		
		if (techCompany.empty or techCompany.name == companyName) and 
		   (tg == base.PT_VIDEOTECH or tg == base.PT_VIDEOQUALITY ) then
				base.gpmFunctions.LinkBox( flick, tech.name, tech.techGroup, tech, 	base.DRG, _Set, _Unset )
		end
	end	
	--Log("SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function _Set( mp, sender )
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if CheckType( mp, sender, base.PT_VIDEOQUALITY ) then 
		project.videoQuality = sender.data
		redraw = true
	elseif CheckType( mp, sender, base.PT_VIDEOTECH ) then
		project:AddTech( sender.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmVideoPage.Show()
	end
end

local function _Unset( mp, _ )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if base.PT_VIDEOQUALITY == mp.type then 
		project.videoQuality = nil 
		redraw = true
	elseif base.PT_VIDEOTECH == mp.type then
		project:RemoveTech( mp.data )
		redraw = true
	end
	
	if redraw then
		guienv:SetDragObject( nil, "" )
		base.gpmVideoPage.Show()
	end
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow
	
	gpm.UpdateProjectWindow( "video" )
	_ShowAvaibleVideoQualityAndVideoTech()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	
	local vq = project.videoQuality
	local lnk = base.gpmFunctions.LinkBox( flick, vq.name, base.PT_VIDEOQUALITY, vq, base.NDRG, _Set, _Unset )
	lnk.defaultTexture = "media/buttons/SoundQualityNoImage.png"						  	
	local maxProjectVideoTech = project:GetNumber( base.PT_VIDEOTECH )
	
	for i=0, maxProjectVideoTech do
		local tech = project:GetTech( i, base.PT_VIDEOTECH )
		local name = "Видео " .. i .. "/" .. maxProjectVideoTech
		
		if not tech.empty then name = tech.name end
		base.gpmFunctions.LinkBox( flick, name, base.PT_VIDEOTECH, tech, base.NDRG, _Set, _Unset )
		lnk.defaultTexture = "media/buttons/videoTechNoImage.png"
	end	

	gpm.ShowParams()
end
