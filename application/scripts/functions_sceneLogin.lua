--���������� ������ �������� ��������� ������� ����� ����
IncludeScript("swork_startDefinition")
IncludeScript("swork_UpdateFunctions")

--���������� ��� ������
local plant = NrpGetPlant()

--��������� ����� ��������� ���
local EDIT_NEWPROFILE_NAME = "_WindowNewGameEditProfile_"
local EDIT_NEWCOMPANY_NAME = "_WindowNewGameEditCompany_"

--������� ����
local width = 200
local height = 100

--�������� ����� ����
function sloginCreateNewProfileAndStartGame( ptr )
	local editName = CLuaEdit( guienv:GetElementByName( EDIT_NEWPROFILE_NAME ) )
	local editCompany = CLuaEdit( guienv:GetElementByName( EDIT_NEWCOMPANY_NAME ) )
	--������� ����� �������
	applic:CreateProfile( editName:GetText(), editCompany:GetText() )
	
	--������� ������ �� ������
	sloginResetDataForNewGame()
	
	--��������� ��������� �������
	sloginAddStartPlayerDef()
	sloginAddStartCompanyTechnology()
	sloginAddStartVideoContentTechnology()
	sloginAddStartSoundContentTechnology()
	sloginAddStartScenarioContentTechnology()
	DebugFunctionCall( sloginAddStartPublicTechnology )
	
	--��������� �� ��������� �����
	NrpSetNextScene( "sceneWork" )
end

function sloginCloseWindowCreateNewProfile( ptr )
	local btn = CLuaButton( ptr )
	local windowg = CLuaWindow( btn:GetParent() )
	windowg:Remove()
end

function slogin_CreateNewGame( ptr )

	local scrW, scrH = driver:GetScreenSize()
	local windowg = CLuaWindow( guienv:AddWindow(  "", scrW / 2 - width, scrH / 2 - height, scrW / 2 + width, scrH / 2 + height, -1, guienv:GetRootGUIElement() ) )
	local btnClose = CLuaButton( windowg:GetCloseButton() )
	btnClose:SetVisible( false )

	scrW, scrH = windowg:GetSize()
	local editName = CLuaEdit( guienv:AddEdit(  "dalerank",
				 						    scrW / 2 - 100, 40, scrW / 2 + 100, 60,
											-1,
											windowg:Self() ) )
	editName:SetName( EDIT_NEWPROFILE_NAME )
	
	local editCompany = CLuaEdit( guienv:AddEdit(  "daleteam",
				 						    scrW / 2 - 100, 70, scrW / 2 + 100, 90,
											-1,
											windowg:Self() ) )
	editCompany:SetName( EDIT_NEWCOMPANY_NAME )
	
	local btnOk = CLuaButton( guienv:AddButton( scrW / 2 - 150,scrH - 30, scrW / 2 - 50, scrH - 10, 
											    windowg:Self(), -1, "������" ) )
	btnOk:SetAction( "sloginCreateNewProfileAndStartGame" )
	
	local btnCancel = CLuaButton( guienv:AddButton( scrW / 2 + 50,scrH - 30, scrW / 2 + 150, scrH - 10, 
											    windowg:Self(), -1, "������" ) )
	btnCancel:SetAction( "sloginCloseWindowCreateNewProfile" )
	
end

local function localPlantLoadBaseReklame()
	plant:LoadBaseReklame( "xtras/reklames/magazine.reklame" )
	plant:LoadBaseReklame( "xtras/reklames/tv.reklame" )
	plant:LoadBaseReklame( "xtras/reklames/radio.reklame" )
	plant:LoadBaseReklame( "xtras/reklames/paper.reklame" )
	
	plant:LoadReklamePrice( applic:GetCurrentProfile() )
end

function slogin_ContinueLastGame( ptr )
	--������������� ������� ������
	applic:LoadGameTimeFromProfile( applic:GetCurrentProfile() )
	
	--��������� ������� ������ ��� ������� ��� �������� �������
	ApplicationUpdateGameBoxAddons()
	applic:LoadBoxAddonsPrice()

	--��������� �������
	applic:LoadProfile( applic:GetCurrentProfile(), applic:GetCurrentProfileCompany() )
		
	--��������� �������� �� ������������ ������
	ApplicationUpdateDiskMachines()
	
	--��������� ��������� ��� ���
	ApplicationUpdateScreenshots()
	
	--��������� ������������
	plant:Load( applic:GetCurrentProfile() )
	
	--��������� ������� �������
	localPlantLoadBaseReklame()
	
	NrpSetNextScene( "sceneWork" )
end