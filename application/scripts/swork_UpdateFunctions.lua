local fileIniPlatforms = "xtras/platforms.list"
local fileIniAddons	= "xtras/gameboxaddons.list"
local fileDiskMachines = "xtras/diskmachines.list"
local fileRetailers = "xtras/retailers.list"
local fileImages = "xtras/screenshots.list"

function ApplicationUpdateGamePlatforms( ptr )
	
	local iniFile = CLuaIniFile( nil, fileIniPlatforms )
	
	local plNumber = iniFile:ReadInteger( "options", "platformNumber", 0 )
    local plIniFile = ""
	for i=1, plNumber do
		local platform = CLuaPlatform( applic:CreatePlatform( "none" ) )
		
		plIniFile = iniFile:ReadString( "platform_"..(i-1), fileIniPlatforms, "" ) 
		platform:Load( plIniFile ) 
		if applic:ValidTime( platform:Self() ) then
			if not applic:PlatformLoaded( platform:Self() ) then
				local pl2 = CLuaPlatform( applic:CreatePlatform( "none" ) )
				pl2:Load( plIniFile )
				applic:AddPlatform( pl2:Self() )
			end
		else
			if platform:AlsoLoaded() then
				applic:RemovePlatform( platform:GetName() )
			end
		end
		
		platform:Remove()
	end
end

function ApplicationUpdateGameBoxAddons( ptr )
	local iniFile = CLuaIniFile( nil, fileIniAddons )

	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 )
    local plIniFile = ""
	
	LogScript( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	for i=1, plNumber do
		local tech = CLuaTech( nil )
		tech:Create( 0 )

		plIniFile = iniFile:ReadString( "options", "addon_"..(i-1), "" ) 
		tech:Load( plIniFile ) 

		if tech:ValidTime() then
			if not tech:IsLoaded() then
				applic:LoadGameBoxAddon( plIniFile )
				LogScript( "Load addon "..tech:GetName().." from "..plIniFile )
			end
		else
			if tech:IsLoaded() then
				applic:RemoveGameBoxAddon( tech:GetName() )
				LogScript( "Remove addon "..tech:GetName() )
			end
		end
		
		tech:Remove()
	end
end

function ApplicationUpdateDiskMachines( ptr )
	local iniFile = CLuaIniFile( nil, fileDiskMachines )

	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 )
    local dmIniFile = ""
	
	LogScript( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	for i=1, dmNumber do
		local dm = CLuaDiskMachine( nil )
		dm:Create()

		dmIniFile = iniFile:ReadString( "options", "diskMachine_"..(i-1), "" ) 
		dm:Load( dmIniFile ) 

		if dm:ValidTime() then
			if not dm:IsLoaded() then
				applic:LoadDiskMachine( dmIniFile )
				LogScript( "Load diskMachine "..dm:GetName().." from "..dmIniFile )
			end
		else
			if dm:IsLoaded() then
				applic:RemoveDiskMachine( dm:GetName() )
				LogScript( "Remove diskMachine "..dm:GetName() )
			end
		end
		
		dm:Remove()
	end
end

function ApplicationUpdateRetailers( ptr )
	local iniFile = CLuaIniFile( nil, fileRetailers )

	local retlNumber = iniFile:ReadInteger( "options", "retailerNumber", 0 )
    local retlIniFile = ""
	
	LogScript( "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
	for i=1, retlNumber do
		local retailer = CLuaRetailer( nil )
		retailer:Create()

		retlIniFile = iniFile:ReadString( "options", "retailer_"..(i-1), "" ) 
		retailer:Load( retlIniFile ) 

		if retailer:ValidTime() then
			if not retailer:IsLoaded() then
				applic:LoadRetailer( retlIniFile )
				LogScript( "Load retailer "..retailer:GetName().." from "..retlIniFile )
			end
		else
			if retailer:IsLoaded() then
				applic:RemoveRetiler( retailer:GetName() )
				LogScript( "Remove retailer "..retailer:GetName() )
			end
		end
		
		retailer:Remove()
	end
end

function ApplicationUpdateScreenshots( ptr )
	local iniFile = CLuaIniFile( nil, fileImages )

	local maxYear = applic:GetGameTime()
	
	local descNumber = iniFile:ReadInteger( maxYear, "DescriptionNumber", 0 )
    local descIniFile = ""
    applic:ClearImageList()
	
	LogScript( "Open config file "..fileImages.." with DescriptionNumber="..descNumber )
	for year=1983, maxYear do
		for i=1, descNumber do
			descIniFile = iniFile:ReadString( maxYear, "description_"..(i-1), "" ) 
			applic:LoadImageList( descIniFile )	
		end
	end
end