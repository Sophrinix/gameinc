local base = _G

module( "userRecognition" )

local emp = nil
local mainWindow = nil
local guienv = base.guienv
local button = base.button

local rank = { "������ ��������", "������� �������", "������ ������", 
			   "�������� ���", "����������� ���������", "������� �������",
			   "������-����������", "��������", "��������" }

local function _Apply( sender )
	sender = base.CLuaButton( sender )
	--������� ����������� ������� ��������� � ��������,
	--������� ����������� � ������ �������� � ���������
	
    if not emp:HaveModificator( sender.Text ) then
		local addingValue = base.math.random( 0, 20 )
		local addingKoeff = 1
		
		if emp.qualityAward > addingValue then
			addingKoeff = addingValue / emp.qualityAward
			addingValue = addingValue * addingKoeff
		end
		   
		
		emp:AddModificator( "codeQuality", base.math.random( 5, 30 ),
		  					false, addingValue, sender.text )
		emp.qualityAward = emp.qualityAward + addingValue
	else
		guienv:MessageBox( "��� ���� ����� �����������", false, false, button.CloseParent, nil )
	end
	
	--������� ��������� ��������� � ��������
	local relation = emp:GetRelation( emp.company.name )
	relation.value = relation.value * 1.1
	--relation.value = realtion.value * ( 1 + emp:GetParam( "codeQualityLast" ) )
	
	--���� ������� ������������ ���������� � ������� ����
	--��� � �������������
end

function Show()
	emp = base.userManager.currentEmployer
	if emp ~= nil then
		mainWindow = guienv:AddWindow( "recognition.png", "25%", "25%", "50%+", "50%+", -1, guienv.root )
		mainWindow.closeButton.visible = false
		
		local layout = guienv:AddLayout( "5%", "5%", "95%", "95%", 3, -1, mainWindow ) 
		
		for i=1, #rank do
			button.LayoutButton( "", layout, -1, rank[ i ], _Apply )
		end
		
		button.Stretch( "30e", "30e", "0e", "0e", "button_down", mainWindow, -1, "", button.CloseParent )
	end	
end