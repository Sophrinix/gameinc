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
local profiles = "save/profiles.list"
local names = nil

--������� ����
local width = 200
local height = 100

local editName = nil
local editCompany = nil
local wndNewGame = nil

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
	
	button.Stretch( "12%", "70e", "49%", "30e", "ok", wndNewGame, -1, "������", CreateNewProfileAndStartGame )
	button.Stretch( "51%", "70e", "88%", "30e", "cancel", wndNewGame, -1, "������", button.CloseParent )
end

function LoadCity()
	base.NrpSetNextScene( "sceneWork" )
end

local function _LoadSaveGame( index )
	local prof = names[ index + 1 ]
	
	if prof.name == nil then
		guienv:MessageBox( "�� ������ �������", false, false, button.CloseParent, button.NoFunction )
		return 
	end
	--������������� ������� ������
	applic:LoadGameTimeFromProfile( prof.name )
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
	applic:LoadProfile( prof.name, prof.company )
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

function Continue()
	applic:CreateDirectorySnapshot( "save", profiles, "profile", "profile.ini" )
		
	local iniFile = base.CLuaIniFile( nil, profiles )
	local profilesNumber = iniFile:ReadInteger( "options", "profileNumber", 0 ) 
	
	local profileWindow = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, guienv.root )
	profileWindow.closeButton.visible = false
	profileWindow.draggable = false
	profileWindow.modal = true
	local lbx = guienv:AddListBox( 30, 45, "30e", "80e", -1, profileWindow )
	names = nil
	names = {}
	
	if profilesNumber == 0 then
		guienv:MessageBox( "��� ����������� ���", false, false, button.CloseParent, button.NoFunction )
		return
	end

	for i=1, profilesNumber do
	 	local configPath = iniFile:ReadString( "options", "profile"..(i-1), "" )	
	 	
	 	base.LogScript( "path to config="..configPath )

	 	local config = base.CLuaIniFile( nil, configPath )
	 	local profileName = config:ReadString( "properties", "profilename:string", "UnknownName" )
	 	local profileCompany = config:ReadString( "properties", "profilecompany:string", "UnknownCompany" )
	 	local cYear, cMonth, cDay = config:ReadTime( "properties", "currentTime:time" )
	 	
	 	local str = base.string.format( "%s(%s) [%04d-%02d-%02d]", profileName, profileCompany, cYear, cMonth, cDay ) 
	 	lbx:AddItem( str, nil )
	 	
	 	base.table.insert( names, { name=profileName, company=profileCompany } )
	end
 
    lbx.itemIndex = 0
		
	button.Stretch( "12%", "70e", "49%", "30e", "ok", profileWindow, -1, "���������", function() _LoadSaveGame( lbx.itemIndex ) end )
	button.Stretch( "51%", "70e", "88%", "30e", "cancel", profileWindow, -1, "������", button.CloseParent )
end

function Quit()
	base.NrpApplicationClose()
end