local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local driver = CLuaDriver( NrpGetVideoDriver() )
local applic = CLuaApplication( NrpGetApplication() ) 

local scrWidth = 0
local scrHeight = 0 

NrpInitializeLoginScene()

scrWidth, scrHeight = driver:GetScreenSize()

--�������� �������� ����� �����

local label = CLuaLabel( guienv:AddLabel( "�������:" .. applic:GetCurrentProfile(), scrWidth / 2 - 150, scrHeight - 170,
										  scrWidth / 2 - 150 + 150, scrHeight - 150, -1, nil ) )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--�������� ����� �����				
local btnSelectProfile = CLuaButton( guienv:AddButton( scrWidth / 2 + 150, scrHeight - 170, scrWidth / 2 + 150 + 60,  scrHeight - 150, 
								 	 guienv:GetRootGUIElement(), -1, "������") )
btnSelectProfile:SetAction( "slogin_SelectProfile" )

--�������� ������ ����� 

local btnNewGame = CLuaButton( guienv:AddButton( scrWidth / 2 - 350, scrHeight - 120, scrWidth / 2 - 200, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "����� ����") )
btnNewGame:SetAction( "slogin_CreateNewGame" )


local btnContinueGame = CLuaButton( guienv:AddButton( scrWidth / 2 - 200, scrHeight - 120, scrWidth / 2 - 50, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "����������") )
btnContinueGame:SetAction( "slogin_CreateContinueGame" )

local btnOptionsGame = CLuaButton( guienv:AddButton( scrWidth / 2 + 50, scrHeight - 120, scrWidth / 2 + 200, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "���������") )
btnOptionsGame:SetAction( "slogin_OptionsGame" )


local btnExitGame = CLuaButton( guienv:AddButton( scrWidth / 2 + 200, scrHeight - 120, scrWidth / 2 + 350, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "�����") )
btnExitGame:SetAction( "slogin_ExitGame" )
--