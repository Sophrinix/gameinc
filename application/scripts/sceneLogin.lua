
NrpInitializeLoginScene()

IncludeScript( "LoginFunctions" )

--�������� �������� ����� �����

local label = guienv:AddLabel( "�������:" .. applic.profile, scrWidth / 2 - 150, "170e", "150+", "150e", -1, guienv:GetRootGUIElement() )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--�������� ����� �����				
local btnSelectProfile = guienv:AddButton( "55%", "170e", "80+",  "150e", guienv:GetRootGUIElement(), -1, "�������")
--btnSelectProfile.action = slogin_SelectProfile

--�������� ������ ����� 
local layout = guienv:AddLayout( "12%", "120e", "88%", "60+", 4, -1, guienv:GetRootGUIElement() )
guienv:AddButton( "����� ����", layout, LoginFunctions.NewGame )
guienv:AddButton( "����������", layout, LoginFunctions.Continue )
guienv:AddButton( "���������", layout, LoginFunctions.ShowOptions )
guienv:AddButton( "�����", layout, LoginFunctions.Quit )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

--guienv:AddLigthing( btnNewGame:Self(), btnExitGame:Self(), "media/textures/larrow.png", 10 )
--