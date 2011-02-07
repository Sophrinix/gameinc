local company = nil

function CreateStuffLists()
	os.remove( updates.fileDiskMachines )
	applic:CreateDirectorySnapshot( "xtras/diskmachines", updates.fileDiskMachines, "diskMachine", "item.dm" )
									
	os.remove( updates.fileIniAddons )
	applic:CreateDirectorySnapshot( "xtras/gameboxaddon", updates.fileIniAddons, "addon", "item.addon" )
							
	os.remove( updates.fileReklames )
	applic:CreateDirectorySnapshot( "xtras/reklames", updates.fileReklames,	"reklame", "item.reklame" )

	os.remove( updates.fileRetailers )
	applic:CreateDirectorySnapshot( "xtras/retailers", updates.fileRetailers, "retailer", "item.retailer" )
									
	os.remove( updates.fileScreenshots )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileScreenshots, "screenshot", "item.desc" )
									
	os.remove( updates.fileEngines )
	applic:CreateDirectorySnapshot( "xtras/engines", updates.fileEngines, "engine", "item.engine" )
									
	os.remove( updates.fileTechs )
	applic:CreateDirectorySnapshot( "xtras/technology", updates.fileTechs, "tech", "item.tech" )
									
	os.remove( updates.fileGames )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileGames, "game", "item.game" )									
	
	os.remove( updates.fileLanguages )
	applic:CreateDirectorySnapshot( "xtras/languages", updates.fileLanguages, "language", "item.lang" )		
	
	os.remove( updates.filePlatforms )
	applic:CreateDirectorySnapshot( "xtras/platforms", updates.filePlatforms, "platform", "item.platform" )	
	
end

function sloginResetDataForNewGame()
	local user = CLuaUser( nil )
	
	applic:ResetData()
    user:Create( "RealPlayer", applic.profile )
    company = applic.playerCompany
	company:Create( applic:GetCurrentProfileCompany(), applic.profile ) 
end

function sloginAddStartCompanyTechnology()
	local ge = CLuaGameEngine( nil )
	ge:Create( "simpleEngine" )
	ge:Load( "xtras/engines/simpleEngine" )
	company:AddGameEngine( ge )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создан движок simpleEngine")
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
	localLoadTechnology( PT_GENRE, "genre_skill" )
	localLoadTechnology( PT_GENRE, "genre_contact" )
	localLoadTechnology( PT_SOUNDTECH, "sound_speaker" )
    localLoadTechnology( PT_VIDEOTECH, "video_textoutput" )	
	localLoadTechnology( PT_ADVTECH, "advtech_memory" )	
	localLoadTechnology( PT_ADVTECH, "advtech_joystick" )	
	localLoadTechnology( PT_SCRIPTS, "techscript_configfile" )	
	localLoadTechnology( PT_MINIGAME, "minigame_textquest" )	
	localLoadTechnology( PT_PHYSIC, "phtech_2dconclusion" )	
	localLoadTechnology( PT_SCENARIO, "scnq_writeself" )
	localLoadTechnology( PT_SOUNDQUALITY, "sndq_writeself" )
	localLoadTechnology( PT_VIDEOQUALITY, "vidq_selfrender" )
end