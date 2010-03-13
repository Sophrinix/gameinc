require("elmid")
require("logdefs")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local driver = CLuaDriver( NrpGetVideoDriver() )

local scrWidth = 0
local scrHeight = 0 

NrpInitializeLoginScene()

scrWidth, scrHeight = driver:GetScreenSize()

--�������� �������� ����� �����

local label = CLuaLabel( guienv:AddLabel( "���", scrWidth / 2 - 150, scrHeight - 170,
										  scrWidth / 2 - 150 + 80, scrHeight - 170 + 20, -1, nil ) )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
						
label:SetObject( guienv:AddLabel( "������", scrWidth / 2 - 150, scrHeight - 140,
								  scrWidth / 2 - 150 + 80, scrHeight - 140 + 20, -1, nil ) )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
				
--�������� ����� �����				
local offsetx = scrWidth / 2 - 150 + 80
local offsety = scrHeight - 170
			
guienv:AddEdit(  "", offsetx, offsety, scrWidth / 2 + 150, offsety + 20, -1, nil )
				
offsety = offsety + 30
local edit = CLuaEdit( guienv:AddEdit(  "", offsetx, offsety, scrWidth / 2 + 150, offsety + 20, -1, nil ) )
edit:SetPasswordBox( true )

--�������� ������ ����� 

local button = CLuaButton( guienv:AddButton( scrWidth / 2 - 150, scrHeight - 120, scrWidth / 2 + 150, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "����") )
button:SetAction( "slogin_RegisterUser" )

--