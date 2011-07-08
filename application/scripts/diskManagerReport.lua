local base = _G

module( "diskManagerReport" )

local guienv = base.guienv
local window = base.window 
local button = base.button

local price = 0
local _mainWindow = nil
local diskManager = nil

local function _AddWork()	
	if base.applic.playerCompany.balance < price then
		guienv:MessageBox( "�� ������� ������� ��� ���������� ������������", false, false, button.CloseParent, button.NoFunction )
		return
	end
	--������� ��������� ������������� � ������ �� ���������� ������
	diskManager.ChangeProducerDiscount()
	
	diskManager.company:AddBalance( "������ ������������", -price )
	base.applic.plant:AddProduceWork( diskManager.produceDiskWork )
	
	diskManager.produceDiskWork:Remove()
	window.Hide( _mainWindow )
end

function Show()
	diskManager = base.diskManager
	price = diskManager.GetFinalPrice()
	
	local produceDiskWork = diskManager.produceDiskWork
	local machines = produceDiskWork.numberMachine
	
	_mainWindow = guienv:AddWindow( "", "25%", 80, "50%+", "50e", -1, diskManager.wndDPP )
	_mainWindow.closeButton.visible = false
	_mainWindow.modal = true
	
	local listBox = guienv:AddListBox( "30", "60", "30e", "30e", -1, _mainWindow )
	
	listBox:AddItem( "��������:"..produceDiskWork.game.name, nil )
	listBox:AddItem( base.string.format( "���� �����: $%.2f", produceDiskWork.diskPrice ), nil )	
	listBox:AddItem( "����� ����������� "..produceDiskWork.numberDisk.." �������", nil )
	listBox:AddItem( "� ���� ������������ "..produceDiskWork.diskInDay.." �������", nil )		
	listBox:AddItem( "������������������ ����� ����� :"..produceDiskWork.hourPerfomance.." (��/���)", nil ) 
	listBox:AddItem( "����� ������������������ : "..produceDiskWork.hourPerfomance * machines .. " (��/���)", nil ) 

	listBox:AddItem( "��������� ������� ����� ����� �������� $"..produceDiskWork.rentPrice, nil )
	listBox:AddItem( "��������� ������� ������������ �������� $"..produceDiskWork.rentPrice * machines, nil)
	listBox:AddItem( "��������� ������ ($/���):"..produceDiskWork.hourPrice * machines, nil )
	listBox:AddItem( base.string.format( "��������� �������������� �����: $ %.2f", produceDiskWork.addonPrice ), nil )
		
	listBox:AddItem( "��������� ������:"..produceDiskWork.hourPrice.." ($/���)", nil )
	listBox:AddItem( "������ �������������: "..base.string.format( "%d $", diskManager.GetFinalDiscount() * 100 ), nil )
	
	listBox:AddItem( "����� ������������: "..produceDiskWork.numberMachine, nil )
	listBox:AddItem( "������������ ������������: "..produceDiskWork.numberDay.." (��)", nil )
	
	listBox:AddItem( base.string.format( "������ ��������� $ %d", price ), nil )
	
	button.Stretch( "12%", "70e", "49%", "40+", "", _mainWindow, -1, base.string.format( "�������� $ %d", price ), _AddWork )
	button.Stretch( "51%", "70e", "88%", "40+", "", _mainWindow, -1, "����������", button.CloseParent )
end
