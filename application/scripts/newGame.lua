local base = _G

IncludeScript( "updates" )

module( "newGame" )

local company = nil
local applic = base.applic
local updates = base.updates
local sceneManager = base.sceneManager;

function UpdateDm( num )
	sceneManager:DrawProgress( 10, "Найдено описаний производств "..num )
end

function UpdateAddon( num )
	sceneManager:DrawProgress( 15, "Найдено дополнений к коробкам "..num )
end

function UpdateReklame( num )
	sceneManager:DrawProgress( 20, "Найдено описаний рекламы "..num )
end

function UpdateRetailer( num )
	sceneManager:DrawProgress( 25, "Найдено издателей "..num )
end

function UpdateScreenshot( num )
	sceneManager:DrawProgress( 30, "Найдено изображений игр "..num )
end

function UpdateEngines( num )
	sceneManager:DrawProgress( 35, "Найдено игровых движков "..num )
end

function UpdateTechs( num )
	sceneManager:DrawProgress( 40, "Найдено технологий "..num )
end

function UpdateGames( num )
	sceneManager:DrawProgress( 45, "Найдено игр "..num )
end

function UpdateLangs( num )
	sceneManager:DrawProgress( 50, "Найдено переводов "..num )
end

function UpdatePlatforms( num )
	sceneManager:DrawProgress( 60, "Найдено платформ "..num )
end

function UpdateCompanies( num )
	sceneManager:DrawProgress( 70, "Найдено компаний "..num )
end

local function _CreateStuffLists()
	applic:CreateDirectorySnapshot( "xtras/diskmachines", updates.fileDiskMachines, "diskMachine", "item.dm", "newGame.UpdateDm" )
	applic:CreateDirectorySnapshot( "xtras/gameboxaddon", updates.fileIniAddons, "addon", "item.addon", "newGame.UpdateAddon" )
	applic:CreateDirectorySnapshot( "xtras/reklames", updates.fileReklames,	"reklame", "item.reklame", "newGame.UpdateReklame" )
	applic:CreateDirectorySnapshot( "xtras/retailers", updates.fileRetailers, "retailer", "item.retailer", "newGame.UpdateRetailer" )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileScreenshots, "screenshot", "item.desc", "newGame.UpdateScreenshot" )
	applic:CreateDirectorySnapshot( "xtras/engines", updates.fileEngines, "engine", "item.engine", "newGame.UpdateEngines" )
	applic:CreateDirectorySnapshot( "xtras/technology", updates.fileTechs, "tech", "item.tech", "newGame.UpdateTechs" )
	applic:CreateDirectorySnapshot( "xtras/games", updates.fileGames, "game", "item.game", "newGame.UpdateGames" )									
	applic:CreateDirectorySnapshot( "xtras/languages", updates.fileLanguages, "language", "item.lang", "newGame.UpdateLangs" )		
	applic:CreateDirectorySnapshot( "xtras/platforms", updates.filePlatforms, "platform", "item.platform", "newGame.UpdatePlatforms" )	
end

local function _AddCompanyTechnologies()
	local ge = base.CLuaGameEngine( nil )
	ge:Create( "simpleEngine" )
	ge:Load( "xtras/engines/simpleEngine" )
	company:AddGameEngine( ge )
	
	base.LogScript("SCRIPT-TEST:Создан движок simpleEngine")
end

local function _LoadTechnology( typer, filer )
	local tech = base.CLuaTech( nil ):Create( filer )
	
	base.LogScript("!!!!!!LOAD TECH FROM "..filer )
	applic:AddPublicTechnology( tech )
end

local function _AddPublicTechnologies()
	_LoadTechnology( base.PT_GENRE, "genre_action" )
	_LoadTechnology( base.PT_GENRE, "genre_adventure" )
	_LoadTechnology( base.PT_SOUNDTECH, "sound_speaker" )
    _LoadTechnology( base.PT_VIDEOTECH, "video_textoutput" )	
	_LoadTechnology( base.PT_ADVTECH, "advtech_memory" )	
	_LoadTechnology( base.PT_ADVTECH, "advtech_joystick" )	
	_LoadTechnology( base.PT_SCRIPTS, "techscript_configfile" )	
	_LoadTechnology( base.PT_MINIGAME, "minigame_textquest" )	
	_LoadTechnology( base.PT_PHYSIC, "phtech_2dconclusion" )	
	_LoadTechnology( base.PT_SCENARIO, "scnq_writeself" )
	_LoadTechnology( base.PT_SOUNDQUALITY, "sndq_writeself" )
	_LoadTechnology( base.PT_VIDEOQUALITY, "vidq_selfrender" )
end

function ResetData()
	local user = base.CLuaUser( nil )
	
	applic:ResetData()
    user:Create( "RealPlayer", applic.profile )
    company = applic.playerCompany
	company:Create( applic:GetCurrentProfileCompany(), applic.profile ) 
	
	_CreateStuffLists()
	updates.LoadLinks( false )
	_AddCompanyTechnologies()
	_AddPublicTechnologies()
end