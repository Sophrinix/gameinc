local base = _G

module( "gpmSoundPage" )

local function _ShowAvaibleSoundQualityAndSoundTech()
	local companyName = company.name
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		local tg = tech.techGroup
		local techCompany = tech.company
		
		if (techCompany.empty or techCompany.name == companyName) and 
			(tg == base.PT_SOUNDTECH or tg == base.PT_SOUNDQUALITY ) then
				 FlickLinkBox( flick, tech.name, tech.techGroup, tech, 
							   DRG, not project:IsMyTech( tech ),
							   LeftMouseButtonUp, nil, nil )
		end
	end	
end

local function _SetSoundQuality( mp )
	local sender = base.CLuaLinkBox( mp )
	project.soundQuality = sender.data
	localShowSoundPage()
end

local function _SetSoundTech()
	local sender = base.CLuaLinkBox( mp )
		
	if sender.data == nil then
		project:RemoveTech( lastData )
	else 
		project:AddTech( sender.data )
	end
	localShowSoundPage()
end

function Show()
	localUpdateProjectWindow( "sound" )
	_ShowAvaibleSoundQualityAndSoundTech()
		
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	
	local sq = project.soundQuality
	FlickLinkBox( flick, sq.name, base.PT_SOUNDQUALITY, sq, NDRG, ENBL,
				  nil, nil, 
				  "media/buttons/SoundQualityNoImage.png",
				  _SetSoundQuality )

	local maxProjectSoundTech = project:GetNumber( base.PT_SOUNDTECH )
	
	for i=0, maxProjectSoundTech do
		local tech = project:GetTech( i, base.PT_SOUNDTECH )
		local name = "Sound " .. i .. "/" .. maxProjectSoundTech
		
		if not tech.empty then name = tech.name end

		FlickLinkBox( flick, name, base.PT_SOUNDTECH, tech, NDRG, ENBL,
					  nil, nil, 
					  "media/buttons/soundTechNoImage.png",
					  _SetSoundTech )
	end	
	gameprojectManager.ShowParams()
end
