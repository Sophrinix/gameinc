local base = _G

module( "updates" )

local Log = base.LogScript
local SCRIPT = base.SCRIPT
local ODS = base.ODS
local CON = base.CON
local applic = base.applic
local plant = base.applic.plant
local currentYear = nil
local currentYearNumber = 0

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

local internalName = "internalname:string"
local ownerName = "owner:string"
local startDate = "startdate:time"
local section_opts = "options"

local function GetString( ini, key ) 
	return ini:ReadString( "properties", key, "error" )
end

local function GetDate( ini, keyName )
	local cYear, cMonth, cDay = ini:ReadTime( "properties", keyName )
	
	return base.os.time( {year=cYear, month=cMonth, day=cDay} )
end

local function GetCurrentDate()
	local cYear, cMonth, cDay = applic:GetGameTime()
	return base.os.time( {year=cYear, month=cMonth, day=cDay} )
end

function FindInventionLoadFile( inventionName )
	
	local iniFile = base.CLuaIniFile( nil, fileTechs )
	local techNumber = iniFile:ReadInteger( section_opts, "techNumber", 0 ) 
	
	for i=1, techNumber do
		if iniFile:ReadString( section_opts, "name"..(i-1), "" ) == inventionName then
			return 	iniFile:ReadString( section_opts, "tech"..(i-1), "" )	
		end
	end
end

function LoadLinks( loadExist )
	if loadExist then
	    --проверим может уже есть файл с линками
		local f=base.io.open( "save/"..applic.profile.."/links.list", "r" )
		if f~=nil then 
			io.close(f) 
			return 
		end
	end
	
	applic:LoadLinks( fileDiskMachines, "diskMachine" )
	applic:LoadLinks( fileIniAddons, "addon" )
	applic:LoadLinks( fileReklames,	"reklame" )
	applic:LoadLinks( fileRetailers, "retailer" )
	applic:LoadLinks( fileEngines, "engine" )
	applic:LoadLinks( fileTechs, "tech" )
	applic:LoadLinks( fileGames, "game" )									
	applic:LoadLinks( fileLanguages, "language" )		
	applic:LoadLinks( filePlatforms, "platform" )	
end

function UpdatePlatformSales()
	for i=1, applic.platformNumber do
		local platform = applic:GetPlatform( i-1 )
		platform:CreateRandomSales()
	end
end

function CheckPlatforms( showPdaForNewPlatform )
	local iniFile = base.CLuaIniFile( nil, filePlatforms )
	
	local plNumber = iniFile:ReadInteger( section_opts, "platformNumber", 0 ) 
	Log( "Open config file "..filePlatforms.." with platformNumber="..plNumber )

	--получим текущую дату

	local curTime = GetCurrentDate()
	--пройдемся по списку платформ, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием платформы
		local tmpPlatformIni = iniFile:ReadString( section_opts, "platform"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpPlatformIni )

		Log( "platform"..(i-1).."="..tmpPlatformIni )
		
		--прочитаем параметры платформы
		local plName = GetString( ini, "name:string" )
		local platformStartTime = GetDate( ini, startDate )
		
		--проверяем попадание врмененного интервала платформы в текущее время
		if platformStartTime <= curTime then
			--платформы нет на рынке, значит надо объявить о выход новой платформы
			local loadPl = applic:LoadPlatform( tmpPlatformIni )
			--все приставки в игре не могут стоить больше 1000$
			loadPl.price = base.math.random( 100, 2000 );
			loadPl:CreateSalesHistory()
			
			if loadPl and showPdaForNewPlatform then	
				--надо показать игроку что появилась новая платформа					
				base.pda.Show( "На рынке появилась новая платформа "..plName )
			end 
		end
	end --for
end

function CheckLanguages()
	local iniFile = base.CLuaIniFile( nil, fileLanguages )
	
	local lgNumber = iniFile:ReadInteger( section_opts, "languageNumber", 0 ) 
	Log( "Open config file "..fileLanguages.." with languageNumber="..lgNumber )

	--пройдемся по списку языков, которые вообще доступны
	for i=1, lgNumber do
		--запоминаем имя файла с описанием языка
		local tmpLangIni = iniFile:ReadString( section_opts, "language"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpLangIni )
		Log( "language"..(i-1).."="..tmpLangIni )
		
		--прочитаем параметры платформы
		local langName = GetString( ini, internalName )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		local tech = base.CLuaTech( nil ):Create( tmpLangIni )
		applic:AddPublicTechnology( tech ) 
	end --for
end

function CheckGameBoxAddons( showPdaForNewPAddon )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )
	
	local plNumber = iniFile:ReadInteger( section_opts, "addonNumber", 0 ) 
	Log( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием аддона
		local tmpAddonIni = iniFile:ReadString( section_opts, "addon"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpAddonIni )
		Log( "addon"..(i-1).."="..tmpAddonIni )
		
		--прочитаем параметры платформы
		local addonName = GetString( ini, internalName )
		local addonStartTime = GetDate( ini, startDate )
		
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
	
	local dmNumber = iniFile:ReadInteger( section_opts, "diskMachineNumber", 0 ) 
	Log( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, dmNumber do
		--запоминаем имя файла с описанием аддона
		local dmIniFile = iniFile:ReadString( section_opts, "diskMachine"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, dmIniFile )
		Log( "diskMachine"..(i-1).."="..dmIniFile )
		
		--прочитаем параметры платформы
		local dmName = GetString( ini, internalName )
		local dmStartTime = GetDate( ini, startDate )
		
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

	local retlNumber = iniFile:ReadInteger( section_opts, "retailerNumber", 0 )
    local retlIniFile = ""
	
	Log( "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
	for i=1, retlNumber do
	end
end

function CheckNewReklames( showPdaForNewReklame )
	local iniFile = base.CLuaIniFile( nil, fileReklames )
	
	local reklamesNumber = iniFile:ReadInteger( section_opts, "reklameNumber", 0 ) 
	--получим текущую дату
	local curTime = GetCurrentDate()
	
	--пройдемся по списку реклам, которые вообще доступны
	for i=1, reklamesNumber do
		--запоминаем имя файла с описанием рекламы
		local tmpReklameIni = iniFile:ReadString( section_opts, "reklame"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpReklameIni )
		
		--если уже можно показывать пользователю рекламу
		if GetDate( ini, startDate ) <= curTime then --1
			--попрoбуем загрузить рекламу в двигло...
			local rName = GetString( ini, internalName )
			local itNew = plant:LoadBaseReklame( rName, tmpReklameIni )
			
			if showPdaForNewReklame and itNew then
				--надо показать игроку что появилась новая возможность
				base.pda.Show( "На рынке доступен новый вид рекламы "..rName )
			end			
		end --1
	end --for
end

local function _CreateCompanyByGame( cmpName )
	--если нет компании, то по умолчанию ставим имя игры
	if cmpName == "" then
		cmpName = game.name..", Ltd."
	end

	local company = applic:GetCompanyByName( cmpName )
	
	base.LogDebug( "find new company "..cmpName )
	
	if company == nil or company.empty then
		local balance = base.math.random( 0, 1000000 )
		local selfPie = 5000;
		local allPie = 11000;
		company = applic:SinceCompany( cmpName, balance, selfPie, allPie )
		company.texture = "media/companies/"..cmpName..".jpg"
		
		if base.pda then
			base.pda.Show( "На рынке появилась новая компания "..company.name )
		end
	end
	
	base.LogScript( "Since company "..company.name )
	return company
end

function CheckNewGames()
	base.LogDebug( "!!!!!!!! Start update games" )
	local iniFile = base.CLuaIniFile( nil, fileGames )

	local gameNumber = iniFile:ReadInteger( section_opts, "gameNumber", 0 )
	local gameIniFile = ""
	local currentDate = GetCurrentDate()
	
	for i=1, gameNumber do
		gameIniFile = iniFile:ReadString( section_opts, "game"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, gameIniFile )
		
		local gameName = GetString( ini, internalName )
		local startDate = GetDate( ini, startDate )
		local cmpName = GetString( ini, ownerName )
		
		if startDate <= currentDate then
			--если такая игра уже выпущена, то её не надо показывать
			--искать будем по внутреннему имени
			local game = applic:GetGame( gameName )
			
			if game == nil or game.empty then
				Log( "find new game in "..gameIniFile )
			    _CreateCompanyByGame( cmpName )

				game = base.CLuaGame():Create( gameIniFile )
				game:CreateHistory()

				applic:AddGameToMarket( game )
			else
				base.LogScript("Игра "..game.name.." уже кем-то создана")
			end
		end 
	end
end

function CheckNewTechs()
	base.LogDebug( "!!!!!!!!Start update technologies" )
	local iniFile = base.CLuaIniFile( nil, fileTechs )

	local plNumber = iniFile:ReadInteger( section_opts, "techNumber", 0 )
    local plIniFile = ""
    local currentDate = GetCurrentDate()
	
	for i=1, plNumber do
		plIniFile = iniFile:ReadString( section_opts, "tech"..(i-1), "" ) 
		local ini = base.CLuaIniFile( nil, plIniFile )
		base.LogDebug( "open tech "..plIniFile ) 
		
		local techName = GetString( ini, internalName )
		local startDate = GetDate( ini, startDate )
		--local t = base.os.date( "*t", startDate )
		--base.LogScript( base.string.format( "readed date %d.%d.%d", t.year, t.month, t.day ) )

		if startDate <= currentDate then
			--такую технологию никто не изобрел
			local tech = applic:GetTech( techName )
			
			base.LogScript( "find new tech... is "..techName )
			if tech == nil or tech.empty then
				tech = base.CLuaTech():Create( plIniFile )
	
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

function CheckNewEvents()
	local year, month, day, _, _ = applic:GetGameTime()
	
	if currentYear == nil or currentYearNumber ~= year then
		local fileName = "year"..year
		base.IncludeScript( fileName )
		currentYear = base.require( fileName )
		currentYearNumber = year
	end
	
	if currentYear then
		currentYear.FindEvent( month, day )
	end
end