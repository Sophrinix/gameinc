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

local function _PureFunc()

end

local function _ShowMistakeForPlatform( video, sound, cpuPercent, memPercent )
	guienv:MessageBox( "Not all tech from engine are avaible", false, false, _PureFunc, _PureFunc )
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

local function _SelectLang( sender )
	local lbx = base.CLuaPictureFlow( sender )
	local selIndex = lbx.itemSelected
	
	local lang = base.CLuaTech( lbx.objectSelected )
		
	--уже есть язык, надо убрать его из списка
	if project:IsMyTech( lang ) then 
		project:RemoveTech( lang )
		lbx:SetItemBlend( selIndex, 0xC0 )	
		--такого языка нет в игре, надо бы добавить
	else 
		project:AddTech( lang )	
		lbx:SetItemBlend( selIndex, 0xff )
	end		
end

local function _SelectPlatform( sender )
	local lbx = base.CLuaPictureFlow( sender )
	local selIndex = lbx.itemSelected
	
	local platform = base.CLuaPlatform( lbx.objectSelected )
	
	--такая платформа заявлена в игре, игрок хочет её убрать
	if project:IsMyPlatform( platform ) then
		project:RemovePlatform( platform )
		lbx:SetItemBlend( selIndex, 0xC0 )				
	else
		if _CheckPlatformsForProject( platform ) then
			project:AddPlatform( platform )
			lbx:SetItemBlend( selIndex, 0xff )			
		end
	end	
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	gpm.UpdateProjectWindow( "platform" )

	local ge = project.gameEngine
	picFlowLang = guienv:AddPictureFlow( "5%", "5%", "95%", "45%", -1, projectWindow )	
	picFlowLang.onSelect = _SelectLang
	
	picFlowPlatform = guienv:AddPictureFlow( "5%", "55%", "95%", "95%", -1, projectWindow )
	picFlowPlatform.onSelect = _SelectPlatform
		
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech.techGroup == base.PT_LANGUAGE then
			local index = picFlowLang:AddItem( tech.texture, tech.name, tech )
			
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
