local applic = CLuaApplication( NrpGetApplication() )

local fileIniPlatforms = "xtras/platforms.list"
local fileIniAddons	= "xtras/gameboxaddons.list"

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
		local tech = CLuaTech( applic:CreateTechnology( 0 ) )

		plIniFile = iniFile:ReadString( "options", "addon_"..(i-1), "" ) 
		tech:Load( plIniFile ) 

		if applic:ValidTime( tech:Self() ) then
			if not applic:IsGameBoxAddonLoaded( tech:GetName() ) then
				applic:LoadGameBoxAddon( plIniFile )
				LogScript( "Load addon "..tech:GetName().." from "..plIniFile )
			end
		else
			if applic:IsGameBoxAddonLoaded( tech:GetName() ) then
				applic:RemoveGameBoxAddon( tech:GetName() )
				LogScript( "Remove addon "..tech:GetName() )
			end
		end
		
		tech:Remove()
	end
end