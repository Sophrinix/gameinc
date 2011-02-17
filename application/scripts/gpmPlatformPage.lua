local base = _G

module( "gpmPlatformPage" )

local function localShowMistakeForPlatform( video, sound, cpuPercent, memPercent )
	guienv:MessageBox( "Not all tech from engine are avaible", false, false, "", "" )
end

local function localCheckPlatformsForProject( pl )
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
	
	if not result then localShowMistakeForPlatform( vTechAv, sTechAv, indexCpu, indexMem ) end
	return result
end

function Show()
	localUpdateProjectWindow( "platform" )

	local ge = project.gameEngine
	picFlowLang = guienv:AddPictureFlow( "5%", "5%", "95%", "45%", -1, projectWindow )	
	picFlowPlatform = guienv:AddPictureFlow( "5%", "55%", "95%", "95%", -1, projectWindow )
		
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech.techGroup == base.PT_LANGUAGE then
			picFlowLang:AddItem( tech.texture, tech.name, tech )
			
			if not project:IsMyTech( tech ) then
				picFlowLang:SetItemBlend( index, 0xC0 )
			else
				picFlowLang:SetItemBlend( index, 0xFF )
			end
		end
	end
	
	for i=1, applic.platformNumber do
		local platform = applic:GetPlatform( i-1 )
		local index = picFlowPlatform:AddItem( platform.texture, platform.name, platform )
		
		if not project:IsMyPlatform( platform ) then
			picFlowPlatform:SetItemBlend( index, 0xC0 )
		else
			picFlowPlatform:SetItemBlend( index, 0xFF )
		end
	end
end
