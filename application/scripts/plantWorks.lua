local base = _G

module( "plantWorks" )

local guienv = base.guienv
local button = base.button
local plantr = base.applic.plant

local _works = nil
local _mainWindow = nil
local _infoWindow = nil
local _parentWindow = nil

function _ShowWorkInfo( sender )
	local elm = base.CLuaElement( sender )
	
	local plWork = _works[ elm.id ].work
	
	if plWork then
		_infoWindow = guienv:AddWindow( "", "25%", 80, "50%+", "50e", -1, _parentWindow )
		_infoWindow.closeButton.visible = false
		_infoWindow.modal = true
		
		local listBox = guienv:AddListBox( "30", "60", "30e", "30e", -1, _infoWindow )
		
		local machines = plWork.numberMachine
		
		listBox:AddItem( "��������:"..plWork.game.name, nil )
		listBox:AddItem( base.string.format( "���� �����: $%.2f", plWork.diskPrice ), nil )	
		listBox:AddItem( "����� ����������� "..plWork.numberDisk.." �������", nil )
		listBox:AddItem( "� ���� ������������ "..plWork.diskInDay.." �������", nil )		
		listBox:AddItem( "������������������ ����� ����� :"..plWork.hourPerfomance.." (��/���)", nil ) 
		listBox:AddItem( "����� ������������������ : "..plWork.hourPerfomance * machines .. " (��/���)", nil ) 

		listBox:AddItem( "��������� ������� ����� ����� �������� $"..plWork.rentPrice, nil )
		listBox:AddItem( "��������� ������� ������������ �������� $"..plWork.rentPrice * machines, nil)
		listBox:AddItem( "��������� ������ ($/���):"..plWork.hourPrice * machines, nil )
		listBox:AddItem( base.string.format( "��������� �������������� �����: $ %.2f", plWork.addonPrice ), nil )
			
		listBox:AddItem( "��������� ������:"..plWork.hourPrice.." ($/���)", nil )
		--listBox:AddItem( "������ �������������: "..base.string.format( "%d $", diskManager.GetFinalDiscount() * 100 ), nil )
		
		listBox:AddItem( "����� ������������: "..plWork.numberMachine, nil )
		listBox:AddItem( "������������ ������������: "..plWork.numberDay.." (��)", nil )
		
		listBox:AddItem( base.string.format( "������ ��������� $ %d", plWork.price ), nil )
		
		button.Stretch( "25%", "70e", "50%+", "40+", "", _infoWindow, -1, "�������", button.CloseParent )
	else
		base.LogScript( "Plant work on index ".. elm.id .. " is null" )
		guienv:MessageBox( "�� ���� ������� ������ ������ CLuaPlantWork. id="..elm.id, false, false, button.CloseParent, button.NoFunction ) 
	end
end

function Show( parent )
	_parentWindow = parent
	_infoWindow = nil
	_mainWindow = guienv:AddWindow( "", "10%", "30%", "90%", "90%", -1, parent )
	_mainWindow.closeButton.visible = false
	_mainWindow.draggable = false
	_mainWindow.drawBody = false

	local flick = guienv:AddFlick( "5%", "5%", "95%", "95%", 5, -1, _mainWindow )
	
	_works = {}
	for i=1, plantr.worksNumber do
		local plWork = plantr:GetProduceWork( i-1 )
		
		local btnWork = button.Stretch( 0, 0, "115+", "120+", "palette", flick, i, plWork.game.name, _ShowWorkInfo )  
		
		base.table.insert( _works, { btn=btnWork, work=plWork } )
	end
	
	return _mainWindow
end
