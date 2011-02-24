 local base = _G

module( "updates" )

local Log = base.LogScript
local SCRIPT = base.SCRIPT
local ODS = base.ODS
local CON = base.CON
local applic = base.applic
local plant = base.applic.plant

fileIniAddons		= "xtras/gameboxaddons.list"
fileDiskMachines	= "xtras/diskmachines.list"
fileRetailers		= "xtras/retailers.list"
fileImages			= "xtras/screenshots.list"
fileTechs			= "xtras/technologies.list"
fileReklames		= "xtras/reklames.list"
fileScreenshots		= "xtras/screenshots.list"
fileEngines			= "xtras/engines.list"
fileGames			= "xtras/games.list"
fileLanguages		= "xtras/languages.list"
filePlatforms		= "xtras/platforms.list"

local function GetString( fileName, key ) 
	return base.CLuaIniFile( nil, fileName ):ReadString( "properties", key, "error" )
end

local function GetDate( fileName, keyName )
	local ini = base.CLuaIniFile( nil, fileName )
	local cYear, cMonth, cDay = ini:ReadTime( "properties", keyName )
	
	return base.os.time( {year=cYear, month=cMonth, day=cDay} )
end

local function GetCurrentDate()
	local cYear, cMonth, cDay = applic:GetGameTime()
	return base.os.time( {year=cYear, month=cMonth, day=cDay} )
end

function LoadLinks()
	applic:LoadLinks( fileDiskMachines, "diskMachine" )
	applic:LoadLinks( fileIniAddons, "addon" )
	applic:LoadLinks( fileReklames,	"reklame" )
	applic:LoadLinks( fileRetailers, "retailer" )
	--applic:LoadLinks( updates.fileScreenshots, "screenshot" )
	applic:LoadLinks( fileEngines, "engine" )
	applic:LoadLinks( fileTechs, "tech" )
	applic:LoadLinks( fileGames, "game" )									
	applic:LoadLinks( fileLanguages, "language" )		
	applic:LoadLinks( filePlatforms, "platform" )	
end

function FindInventionLoadFile( inventionName )
	
	local iniFile = base.CLuaIniFile( nil, fileTechs )
	local techNumber = iniFile:ReadInteger( "options", "techNumber", 0 ) 
	
	for i=1, techNumber do
		if iniFile:ReadString( "options", "name"..(i-1), "" ) == inventionName then
			return 	iniFile:ReadString( "options", "tech"..(i-1), "" )	
		end
	end
end

function CheckPlatforms( showPdaForNewPlatform )
	local iniFile = base.CLuaIniFile( nil, filePlatforms )
	
	local plNumber = iniFile:ReadInteger( "options", "platformNumber", 0 ) 
	Log( "Open config file "..filePlatforms.." with platformNumber="..plNumber )

	--получим текущую дату

	local curTime = GetCurrentDate()
	--пройдемся по списку платформ, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием платформы
		local tmpPlatformIni = iniFile:ReadString( "options", "platform"..(i-1), "" )
		Log( "platform"..(i-1).."="..tmpPlatformIni )
		
		--прочитаем параметры платформы
		local plName = GetString( tmpPlatformIni, "name:string" )
		local platformStartTime = GetDate( tmpPlatformIni, "startdate:time" )
		--local platformEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--проверяем попадание врмененного интервала платформы в текущее время
		if platformStartTime <= curTime then
			--платформы нет на рынке, значит надо объявить о выход новой платформы
			local loadPl = applic:LoadPlatform( tmpPlatformIni )
			if loadPl and showPdaForNewPlatform then	
				--надо показать игроку что появилась новая платформа					
				base.pda.Show( "На рынке появилась новая платформа "..plName )
			end 
		end
	end --for
end

function CheckLanguages()
	local iniFile = base.CLuaIniFile( nil, fileLanguages )
	
	local lgNumber = iniFile:ReadInteger( "options", "languageNumber", 0 ) 
	Log( "Open config file "..fileLanguages.." with languageNumber="..lgNumber )

	--пройдемся по списку языков, которые вообще доступны
	for i=1, lgNumber do
		--запоминаем имя файла с описанием языка
		local tmpLangIni = iniFile:ReadString( "options", "language"..(i-1), "" )
		Log( "language"..(i-1).."="..tmpLangIni )
		
		--прочитаем параметры платформы
		local langName = GetString( tmpLangIni, "internalname:string" )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		local tech = base.CLuaTech( nil )
		tech:Create( 0 )
		tech.status = base.TS_READY
		tech:Load( tmpLangIni )
		applic:AddPublicTechnology( tech ) 
	end --for
end

function CheckGameBoxAddons( showPdaForNewPAddon )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )
	
	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 ) 
	Log( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием аддона
		local tmpAddonIni = iniFile:ReadString( "options", "addon"..(i-1), "" )
		Log( "addon"..(i-1).."="..tmpAddonIni )
		
		--прочитаем параметры платформы
		local addonName = GetString( tmpAddonIni, "internalname:string" )
		local addonStartTime = GetDate( tmpAddonIni, "startdate:time" )
		--local addonEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		if addonStartTime <= curTime then
			--попрoбуем загрузить новую аддон
			local mayShow = applic:LoadGameBoxAddon( tmpAddonIni )
			
			--надо показать игроку что появился новый аддон
			if mayShow and showPdaForNewPAddon then
				base.pda.Show( "На рынке появилась новое дополнение "..addonName )
			end 
		end
	end --for
end

function CheckDiskMachines( showPdaForNewDm )
	local iniFile = base.CLuaIniFile( nil, fileDiskMachines )
	
	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 ) 
	Log( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, dmNumber do
		--запоминаем имя файла с описанием аддона
		local dmIniFile = iniFile:ReadString( "options", "diskMachine"..(i-1), "" )
		Log( "diskMachine"..(i-1).."="..dmIniFile )
		
		--прочитаем параметры платформы
		local dmName = GetString( dmIniFile, "name:string" )
		local dmStartTime = GetDate( dmIniFile, "startdate:time" )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		if dmStartTime <= curTime then
			--попрoбуем загрузить новую аддон
			plant:LoadDiskMachine( dmIniFile )
			
			--надо показать игроку что появился новый аддон
			if showPdaForNewDm then
				base.pda.Show( "На рынке появилась новая платформа "..dmName )
			end 
		end
	end --for
end

function CheckRetailers( ptr )
	local iniFile = base.CLuaIniFile( nil, fileRetailers )

	local retlNumber = iniFile:ReadInteger( "options", "retailerNumber", 0 )
    local retlIniFile = ""
	
	Log( "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
	for i=1, retlNumber do
		local retailer = base.CLuaRetailer( nil )
		retailer:Create()

		retlIniFile = iniFile:ReadString( "options", "retailer"..(i-1), "" ) 
		retailer:Load( retlIniFile ) 

		if retailer:ValidTime() then
			if not retailer:IsLoaded() then
				applic:LoadRetailer( retlIniFile )
				Log( "Load retailer "..retailer.name.." from "..retlIniFile )
			end
		else
			if retailer:IsLoaded() then
				applic:RemoveRetiler( retailer.name )
				Log( "Remove retailer "..retailer.name )
			end
		end
		
		retailer:Remove()
	end
end

function CheckNewReklames( showPdaForNewReklame )
	local iniFile = base.CLuaIniFile( nil, fileReklames )
	
	local reklamesNumber = iniFile:ReadInteger( "options", "reklameNumber", 0 ) 
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку реклам, которые вообще доступны
	for i=1, reklamesNumber do
		--запоминаем имя файла с описанием рекламы
		local tmpReklameIni = iniFile:ReadString( "options", "reklame"..(i-1), "" )
		
		--если уже можно показывать пользователю рекламу
		if GetDate( tmpReklameIni, "startdate:time" ) <= curTime then --1
			--попрoбуем загрузить рекламу в двигло...
			local rName = GetString( tmpReklameIni, "internalname:string" )
			local itNew = plant:LoadBaseReklame( rName, tmpReklameIni )
			
			if showPdaForNewReklame and itNew then
				--надо показать игроку что появилась новая возможность
				base.pda.Show( "На рынке доступен новый вид рекламы "..rName )
			end			
		end --1
	end --for
end

function CheckNewGames()
	base.LogDebug( "!!!!!!!! Start update games" )
	local iniFile = base.CLuaIniFile( nil, fileGames )

	local gameNumber = iniFile:ReadInteger( "options", "gameNumber", 0 )
	local gameIniFile = ""
	local currentDate = GetCurrentDate()
	
	for i=1, gameNumber do
		gameIniFile = iniFile:ReadString( "options", "game"..(i-1), "" )
		base.LogDebug( "open game "..gameIniFile )
		
		local gameName = GetString( gameIniFile, "internalname:string" )
		local startDate = GetDate( gameIniFile, "startdate:time" )
		
		if startDate <= currentDate then
			--если такая игра уже выпущена, то её не надо показывать
			--искать будем по внутреннему имени
			local game = applic:GetGame( gameName )
			
			Log( "find new game in "..gameIniFile )
			if game.empty then
				game:Create( gameIniFile )
				applic:AddGameToMarket( game )
				if base.pda then
					base.pda.Show( "На рынке появилась новая игра "..game.name )
				end
			else
				base.LogDebug("Игра "..game.name.." уже кем-то создана")
			end
		end 
	end
end

function CheckNewTechs()
	base.LogDebug( "!!!!!!!!Start update technologies" )
	local iniFile = base.CLuaIniFile( nil, fileTechs )

	local plNumber = iniFile:ReadInteger( "Options", "techNumber", 0 )
    local plIniFile = ""
    local currentDate = GetCurrentDate()
	
	for i=1, plNumber do
		plIniFile = iniFile:ReadString( "options", "tech"..(i-1), "" ) 
		
		base.LogDebug( "open tech "..plIniFile ) 
		
		local techName = base.CLuaIniFile( nil, plIniFile ):ReadString( "properties", "internalname:string", "error" )
		local startDate = GetDate( plIniFile, "startdate:time" )
		--local t = base.os.date( "*t", startDate )
		--base.LogScript( base.string.format( "readed date %d.%d.%d", t.year, t.month, t.day ) )

		if startDate <= currentDate then
			--такую технологию никто не изобрел
			local tech = applic:GetTech( techName )
			
			base.LogScript( "find new tech... is "..techName )
			if tech.empty then
				tech:Create( 0 )
				tech.status = base.TS_READY
				tech:Load( plIniFile )
	
				base.LogScript("!!!!!! LOAD TECH FROM "..plIniFile )
				--добавим технологию в игру
				applic:AddPublicTechnology( tech ) 
				
				if base.pda then
					base.pda.Show( "На рынке появилась новая технология "..tech.name )
				end
			else
				--технология уже есть в игре
				local techcmp = tech.company
				--это технология зарегестрирована на какую-то контору,
				-- надо её перевести в разряд общедоступных
				if not techcmp.empty then
					techcmp:RemoveTech( tech.name )
					tech.company = nil
					if base.pda then
						base.pda.Show( "Технология адаптирована для массового применения "..
										tech.name.." от компании "..tech.company.name )
					end
				else
					base.LogDebug("Технология уже в общественном пользовании "..tech.name )
				end
			end
		end
	end
end