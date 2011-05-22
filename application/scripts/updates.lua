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
fileCompanies		= "xtras/companies.list"

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
	applic:LoadLinks( fileCompanies, "company" )
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

	--������� ������� ����

	local curTime = GetCurrentDate()
	--��������� �� ������ ��������, ������� ������ ��������
	for i=1, plNumber do
		--���������� ��� ����� � ��������� ���������
		local tmpPlatformIni = iniFile:ReadString( "options", "platform"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpPlatformIni )

		Log( "platform"..(i-1).."="..tmpPlatformIni )
		
		--��������� ��������� ���������
		local plName = GetString( ini, "name:string" )
		local platformStartTime = GetDate( ini, "startdate:time" )
		
		--��������� ��������� ����������� ��������� ��������� � ������� �����
		if platformStartTime <= curTime then
			--��������� ��� �� �����, ������ ���� �������� � ����� ����� ���������
			local loadPl = applic:LoadPlatform( tmpPlatformIni )
			if loadPl and showPdaForNewPlatform then	
				--���� �������� ������ ��� ��������� ����� ���������					
				base.pda.Show( "�� ����� ��������� ����� ��������� "..plName )
			end 
		end
	end --for
end

function CheckLanguages()
	local iniFile = base.CLuaIniFile( nil, fileLanguages )
	
	local lgNumber = iniFile:ReadInteger( "options", "languageNumber", 0 ) 
	Log( "Open config file "..fileLanguages.." with languageNumber="..lgNumber )

	--��������� �� ������ ������, ������� ������ ��������
	for i=1, lgNumber do
		--���������� ��� ����� � ��������� �����
		local tmpLangIni = iniFile:ReadString( "options", "language"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpLangIni )
		Log( "language"..(i-1).."="..tmpLangIni )
		
		--��������� ��������� ���������
		local langName = GetString( ini, "internalname:string" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		local tech = base.CLuaTech( nil ):Create( tmpLangIni )
		applic:AddPublicTechnology( tech ) 
	end --for
end

function CheckGameBoxAddons( showPdaForNewPAddon )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )
	
	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 ) 
	Log( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	--������� ������� ����
	local curTime = GetCurrentDate()
	
	--��������� �� ������ �������, ������� ������ ��������
	for i=1, plNumber do
		--���������� ��� ����� � ��������� ������
		local tmpAddonIni = iniFile:ReadString( "options", "addon"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpAddonIni )
		Log( "addon"..(i-1).."="..tmpAddonIni )
		
		--��������� ��������� ���������
		local addonName = GetString( ini, "internalname:string" )
		local addonStartTime = GetDate( ini, "startdate:time" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		if addonStartTime <= curTime then
			--����o���� ��������� ����� �����
			local mayShow = applic:LoadGameBoxAddon( tmpAddonIni )
			
			--���� �������� ������ ��� �������� ����� �����
			if mayShow and showPdaForNewPAddon then
				base.pda.Show( "�� ����� ��������� ����� ���������� "..addonName )
			end 
		end
	end --for
end

function CheckDiskMachines( showPdaForNewDm )
	local iniFile = base.CLuaIniFile( nil, fileDiskMachines )
	
	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 ) 
	Log( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	--������� ������� ����
	local curTime = GetCurrentDate()
	
	--��������� �� ������ �������, ������� ������ ��������
	for i=1, dmNumber do
		--���������� ��� ����� � ��������� ������
		local dmIniFile = iniFile:ReadString( "options", "diskMachine"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, dmIniFile )
		Log( "diskMachine"..(i-1).."="..dmIniFile )
		
		--��������� ��������� ���������
		local dmName = GetString( ini, "internalname:string" )
		local dmStartTime = GetDate( ini, "startdate:time" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		if dmStartTime <= curTime then
			--����o���� ��������� ����� �����
			plant:LoadDiskMachine( dmIniFile )
			
			--���� �������� ������ ��� �������� ����� �����
			if showPdaForNewDm then
				base.pda.Show( "�� ����� ��������� ����� ��������� "..dmName )
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
	end
end

function CheckNewReklames( showPdaForNewReklame )
	local iniFile = base.CLuaIniFile( nil, fileReklames )
	
	local reklamesNumber = iniFile:ReadInteger( "options", "reklameNumber", 0 ) 
	--������� ������� ����
	local curTime = GetCurrentDate()
	
	--��������� �� ������ ������, ������� ������ ��������
	for i=1, reklamesNumber do
		--���������� ��� ����� � ��������� �������
		local tmpReklameIni = iniFile:ReadString( "options", "reklame"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, tmpReklameIni )
		
		--���� ��� ����� ���������� ������������ �������
		if GetDate( ini, "startdate:time" ) <= curTime then --1
			--����o���� ��������� ������� � ������...
			local rName = GetString( ini, "internalname:string" )
			local itNew = plant:LoadBaseReklame( rName, tmpReklameIni )
			
			if showPdaForNewReklame and itNew then
				--���� �������� ������ ��� ��������� ����� �����������
				base.pda.Show( "�� ����� �������� ����� ��� ������� "..rName )
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
		local ini = base.CLuaIniFile( nil, gameIniFile )
		base.LogDebug( "open game "..gameIniFile )
		
		local gameName = GetString( ini, "internalname:string" )
		local startDate = GetDate( ini, "startdate:time" )
		
		if startDate <= currentDate then
			--���� ����� ���� ��� ��������, �� � �� ���� ����������
			--������ ����� �� ����������� �����
			local game = applic:GetGame( gameName )
			
			Log( "find new game in "..gameIniFile )
			if game == nil or game.empty then
				game = base.CLuaGame():Create( gameIniFile )
				applic:AddGameToMarket( game )
			else
				base.LogDebug("���� "..game.name.." ��� ���-�� �������")
			end
		end 
	end
end

function CheckNewCompanies()
	base.LogDebug( "!!!!!!!! Start update companies" )
	local iniFile = base.CLuaIniFile( nil, fileCompanies )

	local companyNumber = iniFile:ReadInteger( "options", "companyNumber", 0 )
	local companyIniFile = ""
	local currentDate = GetCurrentDate()
	
	for i=1, companyNumber do
		companyIniFile = iniFile:ReadString( "options", "company"..(i-1), "" )
		local ini = base.CLuaIniFile( nil, companyIniFile )
		base.LogDebug( "open company file "..companyIniFile )
		
		local companyName = GetString( ini, "internalname:string" )
		local startDate = GetDate( ini, "startdate:time" )
		
		if startDate <= currentDate then
			--���� ����� �������� ��� ���� �� �����, �� � �� ���� ����������
			--������ ����� �� ����������� �����
			local company = applic:GetCompanyByName( companyName )
			
			base.LogDebug( "find new company in "..companyIniFile )
			
			if company == nil or company.empty then
				company = applic:SinceCompany( companyIniFile )
				
				if base.pda then
					base.pda.Show( "�� ����� ��������� ����� �������� "..company.name )
				end
				
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
		local ini = base.CLuaIniFile( nil, plIniFile )
		base.LogDebug( "open tech "..plIniFile ) 
		
		local techName = GetString( ini, "internalname:string" )
		local startDate = GetDate( ini, "startdate:time" )
		--local t = base.os.date( "*t", startDate )
		--base.LogScript( base.string.format( "readed date %d.%d.%d", t.year, t.month, t.day ) )

		if startDate <= currentDate then
			--����� ���������� ����� �� �������
			local tech = applic:GetTech( techName )
			
			base.LogScript( "find new tech... is "..techName )
			if tech == nil or tech.empty then
				tech = base.CLuaTech():Create( plIniFile )
	
				base.LogScript("!!!!!! LOAD TECH FROM "..plIniFile )
				--������� ���������� � ����
				applic:AddPublicTechnology( tech ) 
				
				if base.pda then
					base.pda.Show( "�� ����� ��������� ����� ���������� "..tech.name )
				end
			else
				--���������� ��� ���� � ����
				local techcmp = tech.company
				--��� ���������� ���������������� �� �����-�� �������,
				-- ���� � ��������� � ������ �������������
				if not techcmp.empty then
					techcmp:RemoveTech( tech.name )
					tech.company = nil
					if base.pda then
						base.pda.Show( "���������� ������������ ��� ��������� ���������� "..
										tech.name.." �� �������� "..tech.company.name )
					end
				else
					base.LogDebug("���������� ��� � ������������ ����������� "..tech.name )
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