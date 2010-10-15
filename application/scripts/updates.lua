local base = _G

module( "updates" )

local Log = base.Log
local SCRIPT = base.SCRIPT
local ODS = base.ODS
local CON = base.CON
local applic = base.applic
local plant = base.NrpGetPlant()

fileIniPlatforms = "xtras/platforms.list"
fileIniAddons	= "xtras/gameboxaddons.list"
fileDiskMachines = "xtras/diskmachines.list"
fileRetailers = "xtras/retailers.list"
fileImages = "xtras/screenshots.list"
fileTechs = "xtras/technologies.list"
fileReklames = "xtras/reklames.list"
fileScreenshots = "xtras/screenshots.list"
fileEngines	= "xtras/engines.list"

local function GetDate( fileName, keyName )
	local ini = base.CLuaIniFile( nil, fileName )
	local cYear, cMonth, cDay = ini:ReadTime( "properties", keyName )
	
	return base.os.time( {year=cYear, month=cMonth, day=cDay} )
end

function CheckGamePlatforms( showPdaForNewPlatform )
	local iniFile = base.CLuaIniFile( nil, fileIniPlatforms )
	
	local plNumber = iniFile:ReadInteger( "options", "platformNumber", 0 ) 
	--получим текущую дату
	local cYear, cMonth, cDay = applic:GetGameTime()
	local curTime = base.os.time( {year=cYear, month=cMonth, day=cDay} )
	
	--пройдемся по списку платформ, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием платформы
		local tmpPlatformIni = iniFile:ReadString( "options", "platform"..(i-1), "" )
		Log({src=SCRIPT, dev=ODS|CON}, "platform"..(i-1).."="..tmpPlatformIni )
		
		--прочитаем параметры платформы
		local plName = base.CLuaIniFile( nil, tmpPlatformIni ):ReadString( "properties", "name:string", "error" )
		local platformStartTime = GetDate( tmpPlatformIni, "startdate:time" )
		--local platformEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--проверяем попадание врмененного интервала платформы в текущее время
		if platformStartTime <= curTime then
			--платформы нет на рынке, значит надо объявить о выход новой платформы
			if not applic:IsPlatformAvaible( plName ) then
				--попрoбуем загрузить новую платформу
				local platform = base.CLuaPlatform( nil )
				platform:Create()
				platform:Load( tmpPlatformIni )
				applic:AddPlatform( platform:Self() )
			
				--надо показать игроку что появилась новая платформа					
				if showPdaForNewPlatform then
					base.pda.Show( "На рынке появилась новая платформа "..plName )
				end
			end 
		end
	end --for
end

function CheckGameBoxAddons( showPdaForNewPAddon )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )
	
	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 ) 
	Log( {src=SCRIPT, dev=ODS|CON}, "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	--получим текущую дату
	local cYear, cMonth, cDay = applic:GetGameTime()
	local curTime = base.os.time( {year=cYear, month=cMonth, day=cDay} )
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, plNumber do
		--запоминаем имя файла с описанием аддона
		local tmpAddonIni = iniFile:ReadString( "options", "addon"..(i-1), "" )
		Log({src=SCRIPT, dev=ODS|CON}, "addon"..(i-1).."="..tmpAddonIni )
		
		--прочитаем параметры платформы
		local addonName = base.CLuaIniFile( nil, tmpAddonIni ):ReadString( "properties", "name:string", "error" )
		local addonStartTime = GetDate( tmpAddonIni, "startdate:time" )
		--local addonEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		if addonStartTime <= curTime then
			--попрoбуем загрузить новую аддон
			applic:LoadGameBoxAddon( tmpAddonIni )
			
			--надо показать игроку что появился новый аддон
			if showPdaForNewPAddon then
				base.pda.Show( "На рынке появилась новая платформа "..addonName )
			end 
		end
	end --for
end

function CheckDiskMachines( showPdaForNewDm )
	local iniFile = base.CLuaIniFile( nil, fileDiskMachines )
	
	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 ) 
	Log( {src=SCRIPT, dev=ODS|CON}, "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	--получим текущую дату
	local cYear, cMonth, cDay = applic:GetGameTime()
	local curTime = base.os.time( {year=cYear, month=cMonth, day=cDay} )
	
	--пройдемся по списку аддонов, которые вообще доступны
	for i=1, dmNumber do
		--запоминаем имя файла с описанием аддона
		local dmIniFile = iniFile:ReadString( "options", "diskMachine"..(i-1), "" )
		Log({src=SCRIPT, dev=ODS|CON}, "diskMachine"..(i-1).."="..dmIniFile )
		
		--прочитаем параметры платформы
		local dmName = base.CLuaIniFile( nil, dmIniFile ):ReadString( "properties", "name:string", "error" )
		local dmStartTime = GetDate( dmIniFile, "startdate:time" )
		
		--проверяем попадание врмененного интервала аддона в текущее время
		if dmStartTime <= curTime then
			--попрoбуем загрузить новую аддон
			applic:LoadDiskMachine( dmIniFile )
			
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
	
	Log( {src=SCRIPT, dev=ODS|CON}, "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
	for i=1, retlNumber do
		local retailer = base.CLuaRetailer( nil )
		retailer:Create()

		retlIniFile = iniFile:ReadString( "options", "retailer"..(i-1), "" ) 
		retailer:Load( retlIniFile ) 

		if retailer:ValidTime() then
			if not retailer:IsLoaded() then
				applic:LoadRetailer( retlIniFile )
				Log( {src=SCRIPT, dev=ODS|CON}, "Load retailer "..retailer:GetName().." from "..retlIniFile )
			end
		else
			if retailer:IsLoaded() then
				applic:RemoveRetiler( retailer:GetName() )
				Log( {src=SCRIPT, dev=ODS|CON}, "Remove retailer "..retailer:GetName() )
			end
		end
		
		retailer:Remove()
	end
end

function CheckScreenshots( ptr )
	local iniFile = base.CLuaIniFile( nil, fileImages )

	local maxYear = applic:GetGameTime()
	
	local descNumber = iniFile:ReadInteger( maxYear, "screenshotNumber", 0 )
    local descIniFile = ""
    applic:ClearImageList()
	
	Log( {src=SCRIPT, dev=ODS|CON}, "Open config file "..fileImages.." with DescriptionNumber="..descNumber )
	for year=1983, maxYear do
		for i=1, descNumber do
			descIniFile = iniFile:ReadString( maxYear, "description"..(i-1), "" ) 
			applic:LoadImageList( descIniFile )	
		end
	end
end

function CheckNewReklames( showPdaForNewReklame )
	local iniFile = base.CLuaIniFile( nil, fileReklames )
	
	local reklamesNumber = iniFile:ReadInteger( "options", "reklameNumber", 0 ) 
	--получим текущую дату
	local cYear, cMonth, cDay = applic:GetGameTime()
	local curTime = base.os.time( {year=cYear, month=cMonth, day=cDay} )
	
	--пройдемся по списку реклам, которые вообще доступны
	for i=1, reklamesNumber do
		--запоминаем имя файла с описанием рекламы
		local tmpReklameIni = iniFile:ReadString( "options", "reklame"..(i-1), "" )
		
		--прочитаем время, когда этот вид рекламы появился на рынке		
		local rTime = GetDate( tmpReklameIni, "startdate:time" )
		
		Log({src=SCRIPT, dev=ODS|CON}, "reklame"..(i-1).."="..tmpReklameIni.."   rTime="..rTime.."   curTime="..curTime )
		--если уже можно показывать пользователю рекламу
		if rTime <= curTime then --1
			--попрoбуем загрузить рекламу в двигло...
			local itNew = plant:LoadBaseReklame( tmpReklameIni )
			
			if showPdaForNewReklame and itNew then
				--надо показать игроку что появилась новая возможность
				local rName = base.CLuaIniFile( nil, tmpReklameIni ):ReadString( "properties", "name:string", "error" )
				base.pda.Show( "На рынке доступен новый вид рекламы "..rName )
			end
			
		end --1
	end --for
end

function CheckNewTechs()
	local iniFile = base.CLuaIniFile( nil, fileTechs )

	local plNumber = iniFile:ReadInteger( "options", "techNumber", 0 )
    local plIniFile = ""
    local cYear, cMonth, cDay = applic.GetGameTime()
    local currentDate = os.time( {year=cYear, month=cMonth, day=cDay } )
	
	for i=1, plNumber do
		plIniFile = iniFile:ReadString( "options", "tech"..(i-1), "" ) 
		
		local techIni = base.CLuaIniFile( nil, plIniFile )
		local startDate = GetDate( plIniFile, "startdate:time" )
		local techName = techIni:ReadString( "properties", "name:string", "error" )

		if startDate <= currentDate then
			--такую технологию никто не изобрел
			local tech = applic:GetTech( techName )
			if tech:Empty() == 1 then
				tech:Create( 0 )
				tech:SetStatus( TS_READY )
				tech:Load( techIni )
	
				Log({src=SCRIPT, dev=ODS|CON}, "!!!!!! LOAD TECH FROM "..techIni )
				--добавим технологию в игру
				applic:AddPublicTechnology( tech:Self() ) 
				
				base.pda.Show( "На рынке появилась новая технология "..tech:GetName() )
			else
				--технология уже есть в игре
				local techcmp = tech:GetCompany()
				--это технология зарегестрирована на какую-то контору,
				-- надо её перевести в разряд общедоступных
				if techcmp:IsEmpty() == 0 then
					techcmp:RemoveTech( tech:GetName() )
					base.pda.Show( "Технология адаптирована для массового применения "..tech:GetName().." от компании "..tech:GetCompany():GetName() )
					tech:SetCompany( nil )
				end
			end
		end
	end
end