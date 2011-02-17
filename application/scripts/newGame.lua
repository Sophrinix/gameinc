local base = _G

IncludeScript( "updates" )

module( "newGame" )

local company = nil
local applic = base.applic
local updates = base.updates

function CreateStuffLists()
	base.os.remove( updates.fileDiskMachines )
	applic:CreateDirectorySnapshot( "xtras/diskmachines", updates.fileDiskMachines, "diskMachine", "item.dm" )
									
	base.os.remove( updates.fileIniAddons )
	applic:CreateDirectorySnapshot( "xtras/gameboxaddon", updates.fileIniAddons, "addon", "item.addon" )
							
	base.os.remove( updates.fileReklames )
	applic:CreateDirectorySnapshot( "xtras/reklames", updates.fileReklames,	"reklame", "item.reklame" )

	base.os.remove( updates.fileRetailers )
	applic:CreateDirectorySnapshot( "xtras/retailers", updates.fileRetailers, "retailer", "item.retailer" )
									
	base.os.remove( updates.fileScreenshots )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileScreenshots, "screenshot", "item.desc" )
									
	base.os.remove( updates.fileEngines )
	applic:CreateDirectorySnapshot( "xtras/engines", updates.fileEngines, "engine", "item.engine" )
									
	base.os.remove( updates.fileTechs )
	applic:CreateDirectorySnapshot( "xtras/technology", updates.fileTechs, "tech", "item.tech" )
									
	base.os.remove( updates.fileGames )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileGames, "game", "item.game" )									
	
	base.os.remove( updates.fileLanguages )
	applic:CreateDirectorySnapshot( "xtras/languages", updates.fileLanguages, "language", "item.lang" )		
	
	base.os.remove( updates.filePlatforms )
	applic:CreateDirectorySnapshot( "xtras/platforms", updates.filePlatforms, "platform", "item.platform" )	
end

function ResetData()
	local user = base.CLuaUser( nil )
	
	applic:ResetData()
    user:Create( "RealPlayer", applic.profile )
    company = applic.playerCompany
	company:Create( applic:GetCurrentProfileCompany(), applic.profile ) 
	
	CreateStuffLists()
	updates.LoadLinks()
	AddCompanyTechnologies()
	AddPublicTechnologies()
end

function AddCompanyTechnologies()
	local ge = base.CLuaGameEngine( nil )
	ge:Create( "simpleEngine" )
	ge:Load( "xtras/engines/simpleEngine" )
	company:AddGameEngine( ge )
	
	base.LogScript("SCRIPT-TEST:Создан движок simpleEngine")
end

local function localLoadTechnology( typer, filer )
	local tech = base.CLuaTech( nil )
	tech:Create( typer )
	tech.status = TS_READY
	tech:Load( filer )
	
	base.LogScript("!!!!!!LOAD TECH FROM "..filer )
	applic:AddPublicTechnology( tech )
end

function AddPublicTechnologies()
	localLoadTechnology( base.PT_GENRE, "genre_skill" )
	localLoadTechnology( base.PT_GENRE, "genre_contact" )
	localLoadTechnology( base.PT_SOUNDTECH, "sound_speaker" )
    localLoadTechnology( base.PT_VIDEOTECH, "video_textoutput" )	
	localLoadTechnology( base.PT_ADVTECH, "advtech_memory" )	
	localLoadTechnology( base.PT_ADVTECH, "advtech_joystick" )	
	localLoadTechnology( base.PT_SCRIPTS, "techscript_configfile" )	
	localLoadTechnology( base.PT_MINIGAME, "minigame_textquest" )	
	localLoadTechnology( base.PT_PHYSIC, "phtech_2dconclusion" )	
	localLoadTechnology( base.PT_SCENARIO, "scnq_writeself" )
	localLoadTechnology( base.PT_SOUNDQUALITY, "sndq_writeself" )
	localLoadTechnology( base.PT_VIDEOQUALITY, "vidq_selfrender" )
end