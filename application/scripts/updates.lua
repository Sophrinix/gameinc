local base = _G

module( "updates" )

local Log = base.Log
local SCRIPT = base.SCRIPT
local ODS = base.ODS
local CON = base.CON
local applic = base.applic
local plant = base.NrpGetPlant()

fileIniPlatforms	= "xtras/platforms.list"
fileIniAddons		= "xtras/gameboxaddons.list"
fileDiskMachines	= "xtras/diskmachines.list"
fileRetailers		= "xtras/retailers.list"
fileImages			= "xtras/screenshots.list"
fileTechs			= "xtras/technologies.list"
fileReklames		= "xtras/reklames.list"
fileScreenshots		= "xtras/screenshots.list"
fileEngines			= "xtras/engines.list"
fileGames			= "xtras/games.list"

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

function CheckGamePlatforms( showPdaForNewPlatform )
	local iniFile = base.CLuaIniFile( nil, fileIniPlatforms )
	
	local plNumber = iniFile:ReadInteger( "options", "platformNumber", 0 ) 
	--������� ������� ����

	local curTime = GetCurrentDate()
	--��������� �� ������ ��������, ������� ������ ��������
	for i=1, plNumber do
		--���������� ��� ����� � ��������� ���������
		local tmpPlatformIni = iniFile:ReadString( "options", "platform"..(i-1), "" )
		Log({src=SCRIPT, dev=ODS|CON}, "platform"..(i-1).."="..tmpPlatformIni )
		
		--��������� ��������� ���������
		local plName = GetString( tmpPlatformIni, "name:string" )
		local platformStartTime = GetDate( tmpPlatformIni, "startdate:time" )
		--local platformEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--��������� ��������� ����������� ��������� ��������� � ������� �����
		if platformStartTime <= curTime then
			--��������� ��� �� �����, ������ ���� �������� � ����� ����� ���������
			if not applic:IsPlatformAvaible( plName ) then
				--����o���� ��������� ����� ���������
				local platform = base.CLuaPlatform( nil )
				platform:Create()
				platform:Load( tmpPlatformIni )
				applic:AddPlatform( platform:Self() )
			
				--���� �������� ������ ��� ��������� ����� ���������					
				if showPdaForNewPlatform then
					base.pda.Show( "�� ����� ��������� ����� ��������� "..plName )
				end
			end 
		end
	end --for
end

function CheckGameBoxAddons( showPdaForNewPAddon )
	local iniFile = base.CLuaIniFile( nil, fileIniAddons )
	
	local plNumber = iniFile:ReadInteger( "options", "addonNumber", 0 ) 
	base.LogScript( "Open config file "..fileIniAddons.." with addonNumber="..plNumber )
	--������� ������� ����
	local curTime = GetCurrentDate()
	
	--��������� �� ������ �������, ������� ������ ��������
	for i=1, plNumber do
		--���������� ��� ����� � ��������� ������
		local tmpAddonIni = iniFile:ReadString( "options", "addon"..(i-1), "" )
		base.LogScript( "addon"..(i-1).."="..tmpAddonIni )
		
		--��������� ��������� ���������
		local addonName = GetString( tmpAddonIni, "name:string" )
		local addonStartTime = GetDate( tmpAddonIni, "startdate:time" )
		--local addonEndDate = GetDate( tmpPlatformIni, "enddate:time" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		if addonStartTime <= curTime then
			--����o���� ��������� ����� �����
			applic:LoadGameBoxAddon( tmpAddonIni )
			
			--���� �������� ������ ��� �������� ����� �����
			if showPdaForNewPAddon then
				base.pda.Show( "�� ����� ��������� ����� ��������� "..addonName )
			end 
		end
	end --for
end

function CheckDiskMachines( showPdaForNewDm )
	local iniFile = base.CLuaIniFile( nil, fileDiskMachines )
	
	local dmNumber = iniFile:ReadInteger( "options", "diskMachineNumber", 0 ) 
	base.LogScript( "Open config file "..fileDiskMachines.." with diskMachineNumber="..dmNumber )
	--������� ������� ����
	local curTime = GetCurrentDate()
	
	--��������� �� ������ �������, ������� ������ ��������
	for i=1, dmNumber do
		--���������� ��� ����� � ��������� ������
		local dmIniFile = iniFile:ReadString( "options", "diskMachine"..(i-1), "" )
		Log({src=SCRIPT, dev=ODS|CON}, "diskMachine"..(i-1).."="..dmIniFile )
		
		--��������� ��������� ���������
		local dmName = GetString( dmIniFile, "name:string" )
		local dmStartTime = GetDate( dmIniFile, "startdate:time" )
		
		--��������� ��������� ����������� ��������� ������ � ������� �����
		if dmStartTime <= curTime then
			--����o���� ��������� ����� �����
			applic:LoadDiskMachine( dmIniFile )
			
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
	
	base.LogScript( "Open config file "..fileRetailers.." with retailNumber="..retlNumber )
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
			local rName = GetString( tmpReklameIni, "techtype:string" )
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
			
			base.LogScript( "find new game in "..gameIniFile )
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
				
				base.pda.Show( "�� ����� ��������� ����� ���������� "..tech:GetName() )
			else
				--���������� ��� ���� � ����
				local techcmp = tech:GetCompany()
				--��� ���������� ���������������� �� �����-�� �������,
				-- ���� � ��������� � ������ �������������
				if techcmp:Empty() == 0 then
					techcmp:RemoveTech( tech:GetName() )
					base.pda.Show( "���������� ������������ ��� ��������� ���������� "..tech:GetName().." �� �������� "..tech:GetCompany():GetName() )
					tech:SetCompany( nil )
				else
					base.LogDebug("���������� ��� � ������������ ����������� "..tech:GetName() )
				end
			end
		end
	end
end