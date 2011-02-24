local base = _G

module( "diskManager" )

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial
local plant = nil
local guienv = base.guienv
local company = nil

local wndDPP = nil
local produceDiskWork = nil

local dayOfProduce = 0
local numberMachine = 0
local labelPricePrint = nil
local currentDiskMachine = nil
local currentGame = nil

local labelPerfomance = nil
local labelPriceHour = nil
local labelNumberMachine = nil
local labelNumberDay = nil
local labelDiskNumber = nil
local labelFinalPrice = nil
local labelDiskPrice = nil
local labelAdvPrice = nil
local labelDiskInDay = nil
local cmbxProduceType = nil
local labelRelationWithPlant = nil

local labelPricePrintAll = nil
local labelPerfomanceAll = nil
local labelPriceHourAll = nil
local labelDiskTrash = nil
local labelGameName = nil 

local windowSelectMachine = nil
local windowSelectGame = nil
local btnGame = nil
local btnMachine = nil
local windowQueryWork = nil

local cmbxGames = nil

local addons = { }

local function localGetAddinMachine()
	if numberMachine < 10 then return 1 
	elseif numberMachine >= 10 and numberMachine < 30 then return 5 
	elseif numberMachine >= 30 and numberMachine < 100 then return 10 
	elseif numberMachine >= 100 and numberMachine < 500 then return 50 
	elseif numberMachine >= 500 then return 100 end
end

local function localGetFinalDiscount()
	local discount = currentDiskMachine.discount + currentDiskMachine.lineDiscount * produceDiskWork.numberMachine
	if discount > currentDiskMachine.maxDiscount then
		discount = currentDiskMachine.maxDiscount
	end
	
	return discount
end

local function localGetFinalPrice()
	return produceDiskWork.price * ( 1 - _GetFinalDiscount() )
end

local function UpdateLabels()
	local machines = produceDiskWork.numberMachine
	
	labelGameName.text = currentGame.name
	
	--labelPricePrint:SetText( "����� �� ����������: $"..produceDiskWork:GetRentPrice() )
	--labelPerfomance:SetText( "������������������:"..produceDiskWork:GetHourPerfomance().." (��/���)" )
	--labelPriceHour:SetText( "��������� ������:"..produceDiskWork:GetHourPrice().." ($/���)" )
	
	labelNumberMachine.text = produceDiskWork.numberMachine
	labelNumberDay.text = produceDiskWork.numberDay.." (��)"
	labelDiskNumber.text = produceDiskWork.numberDisk.." ��" 
	labelDiskPrice.text = base.string.format( "%.2f", produceDiskWork.diskPrice )
	labelFinalPrice.text = "$"..base.string.format( "%d", _GetFinalPrice() )
	
	--labelAdvPrice:SetText( "���� �������������� �����: $"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ) )
	--labelDiskInDay:SetText( "����� ������ �� ����:"..produceDiskWork:GetDiskInDay().." ��" )
	--labelPricePrintAll:SetText( "��������� ������: $"..produceDiskWork:GetRentPrice() * machines )
	--labelPerfomanceAll:SetText( "������������������ (��\���):"..produceDiskWork:GetHourPerfomance() * machines ) 
	--labelPriceHourAll:SetText( "��������� ������ ($\���):"..produceDiskWork:GetHourPrice() * machines )
	
	--labelRelationWithPlant:SetText( "������ �������������: "..base.string.format( "%d", localGetFinalDiscount() * 100 ).."%" )
end

local function localAddLabel( text, xpos, ypos, ww, hh, parent )
	local lb = guienv:AddLabel( text, xpos, ypos, ww, hh, -1, parent )
	lb.color = base.toColor( 0xff, 0xff, 0xff, 0xff )	
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	return lb, ypos + 40
end

function SelectGame()
	guienv:BringToFront( windowSelectGame )
	windowSelectGame.visible = true
end

function SelectGameClose()
	windowSelectGame.visible = false
end

function GameSelected()
	currentGame = base.CLuaGame( cmbxGames.selectedObject )
	produceDiskWork:SetGame( currentGame )
	UpdateLabels()		
	
	windowSelectGame.visible = false
	btnGame:SetImage( 0, 0, 0, 0, currentGame.viewImage )
end

local function localAddGames()
	btnGame = button.Stretch( 100, 400, "150+", "230+", "pure_button", wndDPP, -1, "�������\n ����", SelectGame )

	windowSelectGame = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectGame.visible = false
	windowSelectGame.draggable = false
	windowSelectGame.closeButton.visible = false
	windowSelectGame:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, GameSelected )
	
	cmbxGames = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectGame )
	cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	cmbxGames.drawBorder = false
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectGame, -1, "�������", SelectGame )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectGame, -1, "�������", SelectGameClose )
		
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		if game.haveBox then 
			cmbxGames:AddItem( game.viewImage, game.name, game.object )
		end	
	end
end

function SelectMachine()
	guienv:BringToFront( windowSelectMachine )
	windowSelectMachine.visible = true
end

function SelectMachineClose()
	windowSelectMachine.visible = false
end

function MachineSelected()
	currentDiskMachine = base.CLuaDiskMachine( cmbxProduceType.selectedObject )
	produceDiskWork.produceType = currentDiskMachine.object
	UpdateLabels()
	btnMachine:SetImage( 0, 0, 0, 0, currentDiskMachine.texture )
end

local function  localAddMachines()
	btnMachine = button.Stretch( 130, 170, 250, 265, "pure_button", wndDPP, -1, "�������\n ������������", _SelectMachine )
	
	windowSelectMachine = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectMachine.visible = false
	windowSelectMachine.draggable = false
	windowSelectMachine.closeButton.visible = false
	windowSelectMachine:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, _MachineSelected )
	
	cmbxProduceType = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectMachine )
	cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	cmbxProduceType.drawBorder = false
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectMachine, -1, "�������", _SelectMachine )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectMachine, -1, "�������", _SelectMachineClose )
	
	for i=1, plant.diskMachineNumber do
		local dm = plant:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm.texture, dm.name, dm.object )		
	end
end

local function localAddLabels()
	labelGameName, _ = localAddLabel( "GameName", 270, 60, "400+", "40+", wndDPP )
	--������� ����� ���� ������ �����
	labelDiskPrice, _ =  localAddLabel( "???", 340, 330, "140+", "40+", wndDPP )
	
	--������� ����� ����� ���� �� ������������
	labelDiskNumber, _ = localAddLabel( "???", 330, 490, "140+", "30+", wndDPP )	
	labelFinalPrice, _ = localAddLabel( "�����:", 330, 410, "140+", "30+", wndDPP )
	
	--������� ������ ��������� ���������� ��������� ��� ������������ ������
	labelNumberMachine, _ = localAddLabel( "???", 300, 230, "30+", "40+", wndDPP )
	local btn = guienv:AddButton( 260, 220, "40+", "30+", wndDPP, -1, "+" )
	btn.action = _IncMachineNumber
	
	btn = guienv:AddButton( 260, 250, "40+", "30+", wndDPP, -1, "-" )
	btn.action = _DecMachineNumber
	
	--������� ������ ��������� ���������� ���� ������������
	labelNumberDay, _ = localAddLabel( "numberday", 275, 180, "120+", "30+", wndDPP )
	btn = guienv:AddButton( 410, 160, "32+", "32+", wndDPP, -1, "+" )
	btn.action = _IncDayNumber
	btn = guienv:AddButton( 410, 192, "32+", "32+", wndDPP, -1, "-" )
	btn.action = _DecDayNumber
	
	btn = guienv:AddButton( 480, 220, "190+", "64+", wndDPP, -1, "-" )
	btn.action = _SetAntipiratForce
end

local function _AddWork()	
	--������� ��������� ������������� � ������ �� ���������� ������
	localChangeProducerDiscount()
	
	company:AddBalance( -localGetFinalPrice() )
	plant:AddProduceWork( produceDiskWork )
	
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

function Hide()
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic.playerCompany
	plant = base.pllic.plant
	produceDiskWork = base.CLuaPlantWork():Create( company.name )
	
	if wndDPP == nil then
		wndDPP = window.fsWindow( "media/textures/MachinePlant.png", Hide )
	else
		wndDPP.visible = true
	end
	
	--����, ������� ����� �����������
	localAddGames()
	
	--������� ������ ���������
	localAddMachines()	
	--������� ������� �� ����� � ����� ���� �� �������
	wndDPP:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.ItemSelected()" )
	
	localAddLabels()
	
	local btn = button.Stretch( 700, 385, "226+", "275+", "button_start", wndDPP, -1, "", QueryAddWork )
	btn:SetHoveredImage( 0, 0, 0, 0, "media/buttons/button_start_next.png" )
end

function IncDayNumber()
	dayOfProduce = dayOfProduce + 1
	produceDiskWork:SetNumberDay( dayOfProduce )
	
	UpdateLabels()
end

function DecDayNumber()
	if dayOfProduce > 1 then
		dayOfProduce = dayOfProduce - 1
		produceDiskWork.numberDay = dayOfProduce
	end
	UpdateLabels()
end

function IncMachineNumber()
	numberMachine = numberMachine + _GetAddinMachine()
	produceDiskWork.numberMachine = numberMachine
	
	UpdateLabels()
end

function DecMachineNumber( ptr )
	if numberMachine > 1 then
		numberMachine = numberMachine - localGetAddinMachine()
		produceDiskWork.numberMachine = numberMachine
	end
	
	UpdateLabels()
end

function CloseQueryAddWork()
	windowQueryWork:Remove()
end

function QueryAddWork()
	local machines = produceDiskWork.numberMachine
	windowQueryWork = guienv:AddWindow( "", "25%", 0, "50%+", "100%", -1, wndDPP )
	local listBox = guienv:AddListBox( "2%", "2%", "98%", "80%", -1, windowQueryWork )
	
	listBox:AddItem( "��������:"..currentGame.name, nil )
	listBox:AddItem( base.string.format( "���� �����: %.2f", produceDiskWork.diskPrice ), nil )	
	listBox:AddItem( produceDiskWork:GetNumberDisk().." ��", nil )	
	listBox:AddItem( base.string.format( "$%d", _GetFinalPrice() ), nil )
	listBox:AddItem( "��������� ������: $"..produceDiskWork.rentPrice * machines, nil)
	listBox:AddItem( "������������������ (��\���):"..produceDiskWork.hourPerfomance * machines, nil ) 
	listBox:AddItem( "��������� ������ ($\���):"..produceDiskWork.hourPrice * machines, nil )
	listBox:AddItem( base.string.format( "���� �������������� �����: $ %.2f", produceDiskWork.advPrice ), nil )
	listBox:AddItem( "����� ������ �� ����:"..produceDiskWork.diskInDay.." ��", nil )			
	listBox:AddItem( "��������� ������:"..produceDiskWork.hourPrice.." ($/���)", nil )
	listBox:AddItem( "������������������:"..produceDiskWork.hourPerfomance.." (��/���)", nil ) 
	listBox:AddItem( "����� �� ����������: $"..produceDiskWork.rentPrice, nil )
	listBox:AddItem( "������ �������������: "..base.string.format( "%d $", localGetFinalDiscount() * 100 ), nil )
	listBox:AddItem( "����� ������������: "..produceDiskWork.numberMachine, nil )
	listBox:AddItem( "���� ������������: "..produceDiskWork.numberDay.." (��)", nil )
	
	local dd = base.string.format( "�������� %d", _GetFinalPrice() )
	button.Stretch( "25%", "82%", "24%+", "98%", "", windowQueryWork, -1, dd, _AddWork )
	button.Stretch( "51%", "82%", "24%+", "98%", "", windowQueryWork, -1, "�������", _CloseQueryAddWork )
end

function localChangeProducerDiscount()
--[[	local lastOrder = produceDiskWork:GetNumberDisk()
	local nDisk = currentDiskMachine:GetDiskProduced( company:GetName() )
	local discount = currentDiskMachine:GetDiscount()
	if nDisk < 100000 then nDisk = 100000 end
	
	discount =  discount + lastOrder / nDisk 
	--���� ������ ������ ��� ������� �������������
	--��� ������ ���������
	if discount > currentDiskMachine:GetMaxDiscount() then
		discount = currentDiskMachine:GetMaxDiscount()
	end
	
	--��������� ����� ������ ��� ������ �� ��������� �����
	currentDiskMachine:SetDiscount( discount )

	--� ���� ��������� �������������� ������ ��������� � ������
	for i=1, plant:GetDiskMachineNumber() do
		local dm = plant:GetDiskMachine( i-1 )
		if dm:GetName() ~= currentDiskMachine:GetName() then
			dm:SetDiscount( dm:GetDiscount() - 0.05 )		
		end		
	end
--]]
end
