local applic = CLuaApplication( NrpGetApplication() )

local fileIniPlatforms = "xtras/platforms.list"
local fileIniAddons	= "xtras/gameboxaddons.list"

function sworkUpdateGamePlatforms( ptr )
	local platform = CLuaPlatform( applic:CreatePlatform( "none" ) )
	
	local plNumber = IniReadInteger( "platformNumber", fileIniPlatforms )
    local plIniFile = ""
	for i=1, plNumber do
		plIniFile = IniReadString( "platform_"..(i-1), fileIniPlatforms ) 
		platform:Load( plIniFile ) 
		if platform:ValidTime() then
			if not platform:AlsoLoaded() then
				applic:AddPlatform( applic:CreatePlatform( plIniFile ) )
			end
		else
			if platform:AlsoLoaded() then
				applic:RemovePlatform( platform:GetName() )
			end
		end
		
		platform:Remove()
	end
end

function sworkUpdateGamePlatforms( ptr )
	local tech = CLuaTech( applic:CreateTechnology( 0 ) )

	local plNumber = IniReadInteger( "addonNumber", fileIniAddons )
    local plIniFile = ""
	
	for i=1, plNumber do
		plIniFile = IniReadString( "addon_"..(i-1), fileIniAddons ) 
		tech:Load( plIniFile ) 
		if tech:ValidTime() then
			if not tech:AlsoLoaded() then
				local tech2 = applic:CreateTechnology( 0 )
				tech2:Load(plIniFile)
				applic:AddGameBoxAddon( tech )
			end
		else
			if tech:AlsoLoaded() then
				applic:RemovePlatform( tech:GetName() )
			end
		end
		
		tech:Remove()
	end
end


