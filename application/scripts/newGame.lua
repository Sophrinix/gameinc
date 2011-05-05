local base = _G

IncludeScript( "updates" )

module( "newGame" )

local company = nil
local applic = base.applic
local updates = base.updates

function CreateStuffLists()
	applic:CreateDirectorySnapshot( "xtras/diskmachines", updates.fileDiskMachines, "diskMachine", "item.dm" )
	applic:CreateDirectorySnapshot( "xtras/gameboxaddon", updates.fileIniAddons, "addon", "item.addon" )
	applic:CreateDirectorySnapshot( "xtras/reklames", updates.fileReklames,	"reklame", "item.reklame" )
	applic:CreateDirectorySnapshot( "xtras/retailers", updates.fileRetailers, "retailer", "item.retailer" )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileScreenshots, "screenshot", "item.desc" )
	applic:CreateDirectorySnapshot( "xtras/engines", updates.fileEngines, "engine", "item.engine" )
	applic:CreateDirectorySnapshot( "xtras/technology", updates.fileTechs, "tech", "item.tech" )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileGames, "game", "item.game" )									
	applic:CreateDirectorySnapshot( "xtras/languages", updates.fileLanguages, "language", "item.lang" )		
	applic:CreateDirectorySnapshot( "xtras/platforms", updates.filePlatforms, "platform", "item.platform" )	
	applic:CreateDirectorySnapshot( "xtras/companies", updates.fileCompanies, "company", "item.company" )
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
	local tech = base.CLuaTech( nil ):Create( filer )
	
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