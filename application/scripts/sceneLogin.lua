
NrpInitializeLoginScene()

IncludeScript( "LoginFunctions" )

--�������� �������� ����� �����

local label = guienv:AddLabel( "�������:" .. applic.profile, scrWidth / 2 - 150, "170e", "150+", "150e", -1, guienv.root )
label.color = toColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--�������� ����� �����				
local btnSelectProfile = guienv:AddButton( "55%", "170e", "80+",  "150e", guienv.root, -1, "�������")
--btnSelectProfile.action = slogin_SelectProfile

--�������� ������ ����� 
local layout = guienv:AddLayout( "12%", "120e", "88%", "60+", 4, -1, guienv.root )
guienv:AddButton( "����� ����", layout, LoginFunctions.NewGame )
guienv:AddButton( "����������", layout, LoginFunctions.Continue )
guienv:AddButton( "���������", layout, LoginFunctions.ShowOptions )
guienv:AddButton( "�����", layout, LoginFunctions.Quit )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

--local img = guienv:AddImage( 0, 0, 200, 200, guienv.root, -1, "" );
--img.scale = true
--local txs = driver:CreateBlur( "media/02.bmp" )
--img.texture = txs.path