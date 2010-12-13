local base = _G

module( "updates" )

local Log = base.LogScript
local SCRIPT = base.SCRIPT
local ODS = base.ODS
local CON = base.CON
local applic = base.applic
local plant = base.NrpGetPlant()

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
		Log( "platform"..(i-1).."="..tmpPlatformIni )
		
		--��������� ��������� ���������
		local plName = GetString( tmpPlatformIni, "name:string" )
		local platformStartTime = GetDate( tmpPlatformIni, "startdate:time" )
		--local platformEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
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
		Log( "language"..(i-1).."="..tmpLangIni )
		
		--��������� ��������� ���������
		local langName = GetString( tmpLangIni, "internalname:string" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		local tech = base.CLuaTech( nil )
		tech:Create( 0 )
		tech:SetStatus( TS_READY )
		tech:Load( tmpLangIni )
		applic:AddPublicTechnology( tech:Self() ) 
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
		Log( "addon"..(i-1).."="..tmpAddonIni )
		
		--��������� ��������� ���������
		local addonName = GetString( tmpAddonIni, "internalname:string" )
		local addonStartTime = GetDate( tmpAddonIni, "startdate:time" )
		--local addonEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
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
		Log( "diskMachine"..(i-1).."="..dmIniFile )
		
		--��������� ��������� ���������
		local dmName = GetString( dmIniFile, "name:string" )
		local dmStartTime = GetDate( dmIniFile, "startdate:time" )
		
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
		local retailer = base.CLuaRetailer( nil )
		retailer:Create()

		retlIniFile = iniFile:ReadString( "options", "retailer"..(i-1), "" ) 
		retailer:Load( retlIniFile ) 

		if retailer:ValidTime() then
			if not retailer:IsLoaded() then
				applic:LoadRetailer( retlIniFile )
				Log( "Load retailer "..retailer:GetName().." from "..retlIniFile )
			end
		else
			if retailer:IsLoaded() then
				applic:RemoveRetiler( retailer:GetName() )
				Log( "Remove retailer "..retailer:GetName() )
			end
		end
		
		retailer:Remove()
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
		
		--���� ��� ����� ���������� ������������ �������
		if GetDate( tmpReklameIni, "startdate:time" ) <= curTime then --1
			--����o���� ��������� ������� � ������...
			local rName = GetString( tmpReklameIni, "reklametype:string" )
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
		base.LogDebug( "open game "..gameIniFile )
		
		local gameName = GetString( gameIniFile, "internalname:string" )
		local startDate = GetDate( gameIniFile, "startdate:time" )
		
		if startDate <= currentDate then
			--���� ����� ���� ��� ��������, �� � �� ���� ����������
			--������ ����� �� ����������� �����
			local game = applic:GetGame( gameName )
			
			Log( "find new game in "..gameIniFile )
			if game:Empty() == 1 then
				game:Create( gameIniFile )
				applic:AddGameToMarket( game:Self() )
				base.pda.Show( "�� ����� ��������� ����� ���� "..game:GetName() )
			else
				base.LogDebug("���� "..game:GetName().." ��� ���-�� �������")
			end
		end 
	end
end

function CheckNewTechs()
	base.LogDebug( "!!!!!!!!Start update technologies" )
	local iniFile = base.CLuaIniFile( nil, fileTechs )

	local plNumber = iniFile:ReadInteger( "options", "techNumber", 0 )
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
			--����� ���������� ����� �� �������
			local tech = applic:GetTech( techName )
			
			base.LogScript( "find new tech... is "..techName )
			if tech:Empty() == 1 then
				tech:Create( 0 )
				tech:SetStatus( TS_READY )
				tech:Load( plIniFile )
	
				base.LogScript("!!!!!! LOAD TECH FROM "..plIniFile )
				--������� ���������� � ����
				applic:AddPublicTechnology( tech:Self() ) 
				
				if base.pda then
					base.pda.Show( "�� ����� ��������� ����� ���������� "..tech:GetName() )
				end
			else
				--���������� ��� ���� � ����
				local techcmp = tech:GetCompany()
				--��� ���������� ���������������� �� �����-�� �������,
				-- ���� � ��������� � ������ �������������
				if techcmp:Empty() == 0 then
					techcmp:RemoveTech( tech:GetName() )
					tech:SetCompany( nil )
					if base.pda then
						base.pda.Show( "���������� ������������ ��� ��������� ���������� "..
										tech:GetName().." �� �������� "..tech:GetCompany():GetName() )
					end
				else
					base.LogDebug("���������� ��� � ������������ ����������� "..tech:GetName() )
				end
			end
		end
	end
end