
NrpInitializeLoginScene()

--�������� �������� ����� �����

local label = guienv:AddLabel( "�������:" .. applic.profile, scrWidth / 2 - 150, "170e", "150+", "150e", -1, guienv:GetRootGUIElement() )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--�������� ����� �����				
local btnSelectProfile = guienv:AddButton( scrWidth / 2 + 150, "170e", "60+",  "150e", guienv:GetRootGUIElement(), -1, "������")
btnSelectProfile:SetAction( "slogin_SelectProfile" )

--�������� ������ ����� 

local btnNewGame = guienv:AddButton( scrWidth / 2 - 350, "120e", "150+", "80e", guienv:GetRootGUIElement(), -1, "����� ����")
btnNewGame:SetAction( "slogin_CreateNewGame" )


local btnContinueGame = guienv:AddButton( scrWidth / 2 - 200, "120e", "150+", "80e", guienv:GetRootGUIElement(), -1, "����������")
btnContinueGame:SetAction( "slogin_ContinueLastGame" )

local btnOptionsGame = guienv:AddButton( scrWidth / 2 + 50, "120e", "150+", "80e", guienv:GetRootGUIElement(), -1, "���������")
btnOptionsGame:SetAction( "slogin_OptionsGame" )


local btnExitGame = guienv:AddButton( scrWidth / 2 + 200, "120e", "150+", "80e", guienv:GetRootGUIElement(), -1, "�����")
btnExitGame:SetAction( "slogin_CloseApp" )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

IncludeScript( "functions_sceneLogin" )

--guienv:AddLigthing( btnNewGame:Self(), btnExitGame:Self(), "media/textures/larrow.png", 10 )
--