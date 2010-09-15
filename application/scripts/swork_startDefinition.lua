local company = nil
local user = CLuaUser( nil )

function sloginResetDataForNewGame()
	applic:ResetData()
    user:Create( "RealPlayer", applic:GetCurrentProfile() )
    company = applic:GetPlayerCompany()
	company:Create( applic:GetCurrentProfileCompany(), applic:GetCurrentProfile() ) 
end

function sloginAddStartCompanyTechnology()
	local ge = CLuaGameEngine( nil )
	ge:Create( "simpleEngine" )
	ge:Load( "xtras/engines/simpleEngine" )
	company:AddGameEngine( ge:Self() )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создан движок simpleEngine")
end

function sloginAddStartPlayerDef()
	company:SetCEO( user:Self() )
end

local function localLoadTechnology( typer, filer )
	local tech = CLuaTech( nil )
	tech:Create( typer )
	tech:SetStatus( TS_READY )
	tech:Load( filer )
	
	Log({src=SCRIPT, dev=ODS|CON}, "!!!!!!LOAD TECH FROM "..filer )
	applic:AddPublicTechnology( tech:Self() )
end

function sloginAddStartPublicTechnology()
	localLoadTechnology( PT_GENRE, "xtras/technology/genre_skill/item.tech" )
	localLoadTechnology( PT_GENRE, "xtras/technology/genre_contact/item.tech" )
	localLoadTechnology( PT_SOUNDTECH, "xtras/technology/sound_speaker/item.tech" )
    localLoadTechnology( PT_VIDEOTECH, "xtras/technology/video_textoutput/item.tech" )	
	localLoadTechnology( PT_ADVTECH, "xtras/technology/advtech_memory/item.tech" )	
	localLoadTechnology( PT_ADVTECH, "xtras/technology/advtech_joystick/item.tech" )	
	localLoadTechnology( PT_SCRIPTS, "xtras/technology/techscript_configfile/item.tech" )	
	localLoadTechnology( PT_MINIGAME, "xtras/technology/minigame_textquest/item.tech" )	
	localLoadTechnology( PT_PHYSIC, "xtras/technology/phtech_2dconclusion/item.tech" )	
end

function sloginAddStartScenarioContentTechnology( ptr )
	localLoadTechnology( PT_SCENARIOQUALITY, "xtras/technology/scnq_writeself/item.tech" )
end

function sloginAddStartSoundContentTechnology( ptr )
	localLoadTechnology( PT_SOUNDQUALITY, "xtras/technology/sndq_writeself/item.tech" )
end

function sloginAddStartVideoContentTechnology( ptr )
	localLoadTechnology( PT_VIDEOQUALITY, "xtras/technology/vidq_selfrender/item.tech" )
end