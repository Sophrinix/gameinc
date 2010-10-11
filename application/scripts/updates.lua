local base = _G

module( "updates" )

local LogScript = base.LogScript
local applic = base.applic

fileIniPlatforms = "xtras/platforms.list"
fileIniAddons	= "xtras/gameboxaddons.list"
fileDiskMachines = "xtras/diskmachines.list"
fileRetailers = "xtras/retailers.list"
fileImages = "xtras/screenshots.list"
fileTechs = "xtras/technologies.list"
fileReklames = "xtras/reklames.list"
fileScreenshots = "xtras/screenshots.list"
fileEngines	= "xtras/engines.list"

function CheckGamePlatforms( ptr )
	
	local iniFile = base.CLuaIniFile( nil, fileIniPlatforms )
	
	local plNumber = iniFile:ReadInteger( "options", "platformNumber", 0 )
    local plIniFile = ""
	for i=1, plNumber do
		local platform = base.CLuaPlatform( applic:CreatePlatform( "none" ) )
		
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

function CheckGameBoxAddons( ptr )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )

	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 )
    local plIniFile = ""
	
	LogScript( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	for i=1, plNumber do
		local tech = CLuaTech( nil )
		tech:Create( 0 )

		plIniFile = iniFile:ReadString( "options", "addon_"..(i-1), "" ) 
		tech:Load( plIniFile ) 

		if tech:ValidTime() then
			if not tech:IsLoaded() then
				applic:LoadGameBoxAddon( plIniFile )
				LogScript( "Load addon "..tech:GetName().." from "..plIniFile )
			end
		else
			if tech:IsLoaded() then
				applic:RemoveGameBoxAddon( tech:GetName() )
				LogScript( "Remove addon "..tech:GetName() )
			end
		end
		
		tech:Remove()
	end
end

function CheckDiskMachines( ptr )
	local iniFile = base.CLuaIniFile( nil, fileDiskMachines )

	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 )
    local dmIniFile = ""
	
	LogScript( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	for i=1, dmNumber do
		local dm = base.CLuaDiskMachine( nil )
		dm:Create()

		dmIniFile = iniFile:ReadString( "options", "diskMachine_"..(i-1), "" ) 
		dm:Load( dmIniFile ) 

		if dm:ValidTime() then
			if not dm:IsLoaded() then
				applic:LoadDiskMachine( dmIniFile )
				LogScript( "Load diskMachine "..dm:GetName().." from "..dmIniFile )
			end
		else
			if dm:IsLoaded() then
				applic:RemoveDiskMachine( dm:GetName() )
				LogScript( "Remove diskMachine "..dm:GetName() )
			end
		end
		
		dm:Remove()
	end
end

function CheckRetailers( ptr )
	local iniFile = base.CLuaIniFile( nil, fileRetailers )

	local retlNumber = iniFile:ReadInteger( "options", "retailerNumber", 0 )
    local retlIniFile = ""
	
	LogScript( "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
	for i=1, retlNumber do
		local retailer = base.CLuaRetailer( nil )
		retailer:Create()

		retlIniFile = iniFile:ReadString( "options", "retailer_"..(i-1), "" ) 
		retailer:Load( retlIniFile ) 

		if retailer:ValidTime() then
			if not retailer:IsLoaded() then
				applic:LoadRetailer( retlIniFile )
				LogScript( "Load retailer "..retailer:GetName().." from "..retlIniFile )
			end
		else
			if retailer:IsLoaded() then
				applic:RemoveRetiler( retailer:GetName() )
				LogScript( "Remove retailer "..retailer:GetName() )
			end
		end
		
		retailer:Remove()
	end
end

function CheckScreenshots( ptr )
	local iniFile = base.CLuaIniFile( nil, fileImages )

	local maxYear = applic:GetGameTime()
	
	local descNumber = iniFile:ReadInteger( maxYear, "DescriptionNumber", 0 )
    local descIniFile = ""
    applic:ClearImageList()
	
	LogScript( "Open config file "..fileImages.." with DescriptionNumber="..descNumber )
	for year=1983, maxYear do
		for i=1, descNumber do
			descIniFile = iniFile:ReadString( maxYear, "description_"..(i-1), "" ) 
			applic:LoadImageList( descIniFile )	
		end
	end
end

function CheckNewReklames( showPdaForNewReklame )
	local iniFile = base.CLuaIniFile( nil, fileReklames )
	
	local reklamesNumber = iniFile:ReadInteger( "options", "reklameNumber", 0 ) 
	--получим текущую дату
	local cYear, cMonth, cDay = applic:GetGameTime()
	local curTime = os.time( {year=cYear, month=cMonth, day=cDay} )
	
	--пройдемся по списку реклам, которые вообще доступны
	for i=1, reklamesNumber do
		--запоминаем имя файла с описанием рекламы
		local tmpReklameIni = iniFile:ReadString( "options", "reklame_"..(i-1), "" )
		
		--прочитаем время, когда этот вид рекламы появился на рынке
		local lYear, lMonth, lDay = base.CLuaIniFile( nil, tmpReklameIni ):ReadTime( "options", "startdate:time" )
		
		--если уже можно показывать пользователю рекламу
		if os.time( {year=lYear, month=lMonth, day=lDay} ) <= curTime then --1
			--попрoбуем загрузить рекламу в двигло...
			local itNew = plant:LoadBaseReklame( tmpReklameIni )
			
			if showPdaForNewReklame and itNew then
				--надо показать игроку что появилась новая возможность
				local rName = base.CLuaIniFile( nil, tmpReklameIni ):ReadString( "options", "name:string", "" )
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
    local currentDate = os.time{year=cYear, month=cMonth, day=cDay }
	
	base.LogScript( "Open config file "..fileIniAddons.." with tech="..plNumber )
	for i=1, plNumber do
		plIniFile = iniFile:ReadString( "options", "tech_"..(i-1), "" ) 
		
		local techIni = base.CLuaIniFile( nil, plIniFile )
		local tYear, tMonth, tDay = techIni:ReadTime( "options", "startdate:time", "y=0000 m=00 d=00 h=00 mi=00 s=0" )
		local techName = techIni:ReadString( "options", "name:string", "" )

		if os.time{year=tYear, month=tMonth, day=tDay} <= currentDate then
			--такую технологию никто не изобрел
			local tech = applic:GetTech( techName )
			if tech:Empty() == 1 then
				tech:Create( 0 )
				tech:SetStatus( TS_READY )
				tech:Load( techIni )
	
				LogScript({src=SCRIPT, dev=ODS|CON}, "!!!!!! LOAD TECH FROM "..techIni )
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