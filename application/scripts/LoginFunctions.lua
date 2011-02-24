--���������� ������ �������� ��������� ������� ����� ����
IncludeScript("newGame")
IncludeScript("updates")
IncludeScript("button")

local base = _G

module( "LoginFunctions" )

local guienv = base.guienv
local applic = base.applic
local button = base.button
local updates = base.updates
--���������� ��� ������
local plant = base.applic.plant

--������� ����
local width = 200
local height = 100

local editName = nil
local editCompany = nil
local wndNewGame = nil

local function CloseNewGame( ptr )
	wndNewGame:Remove()
end

function ShowOptions()

end

local function localPlantLoadBaseReklame()
	base.updates.CheckNewReklames()
	plant:LoadReklamePrice( applic.profile )
end

--�������� ����� ����
function CreateNewProfileAndStartGame( ptr )
	--������� ����� �������
	applic:CreateProfile( editName.text, editCompany.text )
	
	--������� ������ �� ������
	base.newGame.ResetData()
	
	applic:LoadScreenshots( updates.fileScreenshots )
	updates.CheckNewTechs()
	updates.CheckLanguages()
	updates.CheckGameBoxAddons()
	updates.CheckPlatforms( false )
	updates.CheckDiskMachines()
	updates.CheckNewReklames()
	updates.CheckNewGames()
	
	--�������� �������
	applic:CreateNewFreeUsers()
	
	--��������� ������� �������
	localPlantLoadBaseReklame( false )
	
	--��������� �� ��������� �����
	base.NrpSetNextScene( "sceneWork" )
end

function NewGame( ptr )
	wndNewGame = guienv:AddWindow( "", "25%", "33%", "50%+", "33%+", -1, guienv.root )
	wndNewGame.closeButton.visible = false
	
	editName = guienv:AddEdit(  "dalerank",  "10%", 40, "80%+", "20+", -1, wndNewGame )
	editCompany = guienv:AddEdit(  "daleteam", "10%", 70, "80%+", "20+", -1, wndNewGame )				
	
	button.Stretch( "12%", "60e", "49%", "10e", "ok", wndNewGame, -1, "������", CreateNewProfileAndStartGame )
	button.Stretch( "51%", "60e", "88%", "10e", "cancel", wndNewGame, -1, "������", CloseNewGame )
end

function LoadCity()
	base.NrpSetNextScene( "sceneWork" )
end

function Continue( ptr )
	local lastProfile = applic.profile
	
	local dd = base.io.open( "save/"..lastProfile.."/profile.ini", "r" )
	if dd == nil then
		guienv:MessageBox( "��� ����������� ���", false, false, "", "" )
		return 0
	end
	--������������� ������� ������
	applic:LoadGameTimeFromProfile( applic.profile )
	
	--��������� ��������� ��� ���
	applic:LoadScreenshots( updates.fileScreenshots )
	
	--��������� �����
	updates.LoadLinks()
	
	--��������� �������
	applic:LoadProfile( applic.profile, applic:GetCurrentProfileCompany() )
	
	--��������� ������� ������ ��� ������� ��� �������� �������
	updates.CheckGameBoxAddons()
	applic:LoadBoxAddonsPrice()
	
	applic.pda:Load()
		
	--��������� �������� �� ������������ ������
	updates.CheckDiskMachines()
	
	--��������� ������������ � �������
	plant:Load()
		
	--��������� ������� �������
	localPlantLoadBaseReklame( false )
	
	guienv:FadeAction( 1000, FADE_OUT, REMOVE_ON_END )
	guienv:AddTimer( 1010, LoadCity );
end

function Quit()
	base.NrpApplicationClose()
end