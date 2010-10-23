
NrpInitializeLoginScene()

--�������� �������� ����� �����

local label = guienv:AddLabel( "�������:" .. applic:GetCurrentProfile(), scrWidth / 2 - 150, scrHeight - 170,
										  scrWidth / 2 - 150 + 150, scrHeight - 150, -1, nil )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--�������� ����� �����				
local btnSelectProfile = guienv:AddButton( scrWidth / 2 + 150, scrHeight - 170, scrWidth / 2 + 150 + 60,  scrHeight - 150, 
								 	       guienv:GetRootGUIElement(), -1, "������")
btnSelectProfile:SetAction( "slogin_SelectProfile" )

--�������� ������ ����� 

local btnNewGame = guienv:AddButton( scrWidth / 2 - 350, scrHeight - 120, scrWidth / 2 - 200, scrHeight - 80, 
									 guienv:GetRootGUIElement(), -1, "����� ����")
btnNewGame:SetAction( "slogin_CreateNewGame" )


local btnContinueGame = guienv:AddButton( scrWidth / 2 - 200, scrHeight - 120, scrWidth / 2 - 50, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "����������")
btnContinueGame:SetAction( "slogin_ContinueLastGame" )

local btnOptionsGame = guienv:AddButton( scrWidth / 2 + 50, scrHeight - 120, scrWidth / 2 + 200, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "���������")
btnOptionsGame:SetAction( "slogin_OptionsGame" )


local btnExitGame = guienv:AddButton( scrWidth / 2 + 200, scrHeight - 120, scrWidth / 2 + 350, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "�����")
btnExitGame:SetAction( "slogin_CloseApp" )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

IncludeScript( "functions_sceneLogin" )

--guienv:AddLigthing( btnNewGame:Self(), btnExitGame:Self(), "media/textures/larrow.png", 10 )
--