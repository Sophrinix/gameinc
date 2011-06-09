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
local sceneManager = base.sceneManager
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

function About()
	base.browser:Show()
	base.browser:Navigate( "media/html/intro.htm" )
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
	sceneManager:DrawProgress( 5, "���� �����������" )

	updates.CheckNewTechs()
	sceneManager:DrawProgress( 10, "��������� ����������" )

	updates.CheckLanguages()
	sceneManager:DrawProgress( 15, "��������� �����" )

	updates.CheckGameBoxAddons()
	sceneManager:DrawProgress( 20, "���� ������ � ��������" )

	updates.CheckPlatforms( false )
	sceneManager:DrawProgress( 26, "���� ����� ���������" )

	updates.CheckDiskMachines()
	sceneManager:DrawProgress( 30, "��������� ������ � �����������" )

	updates.CheckNewReklames()
	sceneManager:DrawProgress( 35, "���� �������" )

	updates.CheckNewGames()
	sceneManager:DrawProgress( 40, "��������� ���� ������ �� �����" )

	updates.CheckNewCompanies()
	sceneManager:DrawProgress( 45, "������� ��������" )
	
	--�������� �������
	applic:CreateNewFreeUsers()
	sceneManager:DrawProgress( 50, "��������� ����� �������������" )
	
	--��������� ������� �������
	localPlantLoadBaseReklame( false )
	sceneManager:DrawProgress( 55, "������� ������� �������" )
	
	--��������� �� ��������� �����
	base.NrpSetNextScene( "sceneWork" )
end

function NewGame( ptr )
	wndNewGame = guienv:AddWindow( "", "25%", "33%", "50%+", "33%+", -1, guienv.root )
	wndNewGame.closeButton.visible = false
	
	editName = guienv:AddEdit(  "Player",  "10%", 40, "80%+", "20+", -1, wndNewGame )
	editCompany = guienv:AddEdit(  "CompanyName", "10%", 70, "80%+", "20+", -1, wndNewGame )				
	
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
		guienv:MessageBox( "��� ����������� ���", false, false, button.CloseParent, button.NoFunction )
		return 0
	end
	--������������� ������� ������
	applic:LoadGameTimeFromProfile( applic.profile )
	sceneManager:DrawProgress( 1, "���������� ������ �� ����������" )
	
	--��������� ��������� ��� ���
	applic:LoadScreenshots( updates.fileScreenshots )
	sceneManager:DrawProgress( 4, "��������� ������ ��� ����" )
	
	--��������� �����
	updates.LoadLinks()
	sceneManager:DrawProgress( 8, "������� �������" )
	
	--��������� ������� ������ ��� ������� ��� �������� �������
	updates.CheckGameBoxAddons()
	sceneManager:DrawProgress( 20, "���� ������ � ��������" )

	applic:LoadBoxAddonsPrice()
	sceneManager:DrawProgress( 30, "������������� ���� �� ������" )
	
	--��������� �������
	applic:LoadProfile( applic.profile, applic:GetCurrentProfileCompany() )
	sceneManager:DrawProgress( 10, "������ ������� ������" )
	
	applic.pda:Load()
	sceneManager:DrawProgress( 40, "��������� �������" )
		
	--��������� �������� �� ������������ ������
	updates.CheckDiskMachines()
	sceneManager:DrawProgress( 50, "���� ����� �������� ������������" )
	
	--��������� ������������ � �������
	plant:Load()
	sceneManager:DrawProgress( 60, "��������� ������ ������������" )

	--��������� ������� �������
	localPlantLoadBaseReklame( false )
	sceneManager:DrawProgress( 70, "��������� ������� �������" )
	
	guienv:FadeAction( 1000, FADE_OUT, REMOVE_ON_END )
	guienv:AddTimer( 1010, LoadCity, nil );
end

function Quit()
	base.NrpApplicationClose()
end