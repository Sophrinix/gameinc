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
	local discount = currentDiskMachine:GetDiscount() + currentDiskMachine:GetLineDiscount() * produceDiskWork:GetNumberMachine()
	if discount > currentDiskMachine:GetMaxDiscount() then
		discount = currentDiskMachine:GetMaxDiscount()
	end
	
	return discount
end

local function localGetFinalPrice()
	return produceDiskWork:GetPrice() * ( 1 - localGetFinalDiscount() )
end

local function UpdateLabels()
	local machines = produceDiskWork:GetNumberMachine()
	
	labelGameName:SetText( currentGame:GetName() )
	
	--labelPricePrint:SetText( "����� �� ����������: $"..produceDiskWork:GetRentPrice() )
	--labelPerfomance:SetText( "������������������:"..produceDiskWork:GetHourPerfomance().." (��/���)" )
	--labelPriceHour:SetText( "��������� ������:"..produceDiskWork:GetHourPrice().." ($/���)" )
	
	labelNumberMachine:SetText( produceDiskWork:GetNumberMachine() )
	labelNumberDay:SetText( produceDiskWork:GetNumberDay().." (��)" )
	labelDiskNumber:SetText(  produceDiskWork:GetNumberDisk().." ��" ) 
	labelDiskPrice:SetText( base.string.format( "%.2f", produceDiskWork:GetDiskPrice() ) )  
	labelFinalPrice:SetText( "$"..base.string.format( "%d", localGetFinalPrice() ) )
	
	--labelAdvPrice:SetText( "���� �������������� �����: $"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ) )
	--labelDiskInDay:SetText( "����� ������ �� ����:"..produceDiskWork:GetDiskInDay().." ��" )
	--labelPricePrintAll:SetText( "��������� ������: $"..produceDiskWork:GetRentPrice() * machines )
	--labelPerfomanceAll:SetText( "������������������ (��\���):"..produceDiskWork:GetHourPerfomance() * machines ) 
	--labelPriceHourAll:SetText( "��������� ������ ($\���):"..produceDiskWork:GetHourPrice() * machines )
	
	--labelRelationWithPlant:SetText( "������ �������������: "..base.string.format( "%d", localGetFinalDiscount() * 100 ).."%" )
end

local function localAddLabel( text, xpos, ypos, ww, hh, parent )
	local lb = guienv:AddLabel( text, xpos, ypos, ww, hh, -1, parent )
	lb:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )	
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	return lb, ypos + 40
end

function SelectGame()
	guienv:BringToFront( windowSelectGame )
	windowSelectGame:SetVisible( true )
end

function SelectGameClose()
	windowSelectGame:SetVisible( false )	
end

function GameSelected()
	currentGame = base.CLuaGame( cmbxGames:GetSelectedObject() )
	produceDiskWork:SetGame( currentGame )
	UpdateLabels()		
	
	windowSelectGame:SetVisible( false )	
	btnGame:SetImage( 0, 0, 0, 0, currentGame:GetViewImage() )
end

local function localAddGames()
	btnGame = button.Stretch( 100, 400, "150+", "230+", "pure_button", wndDPP, -1, "�������\n ����", "./diskManager.SelectGame()" )

	windowSelectGame = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectGame:SetVisible( false )
	windowSelectGame:SetDraggable( false )
	windowSelectGame:GetCloseButton():SetVisible( false )
	windowSelectGame:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.GameSelected()" )
	
	cmbxGames = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectGame )
	cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	cmbxGames:SetDrawBorder( false )
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectGame, -1, "�������", "./diskManager.SelectGame()" )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectGame, -1, "�������", "./diskManager.SelectGameClose()" )
		
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		if game:HaveBox() then 
			cmbxGames:AddItem( game:GetViewImage(), game:GetName(), game.object )
		end	
	end
end

function SelectMachine()
	guienv:BringToFront( windowSelectMachine )
	windowSelectMachine:SetVisible( true )
end

function SelectMachineClose()
	windowSelectMachine:SetVisible( false )	
end

function MachineSelected()
	currentDiskMachine = base.CLuaDiskMachine( cmbxProduceType:GetSelectedObject() )
	produceDiskWork:SetProduceType( currentDiskMachine.slef )
	UpdateLabels()
	btnMachine:SetImage( 0, 0, 0, 0, currentDiskMachine:GetTexture() )
	windowSelectMachine:SetVisible( false )
end

local function  localAddMachines()
	btnMachine = button.Stretch( 130, 170, 250, 265, "pure_button", wndDPP, -1, "�������\n ������������", "./diskManager.SelectMachine()" )
	
	windowSelectMachine = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectMachine:SetVisible( false )
	windowSelectMachine:SetDraggable( false )
	windowSelectMachine:GetCloseButton():SetVisible( false )
	windowSelectMachine:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.MachineSelected()" )
	
	cmbxProduceType = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectMachine )
	cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	cmbxProduceType:SetDrawBorder( false )
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectMachine, -1, "�������", "./diskManager.SelectMachine()" )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectMachine, -1, "�������", "./diskManager.SelectMachineClose()" )
	
	for i=1, plant:GetDiskMachineNumber() do
		local dm = plant:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm:GetTexture(), dm:GetName(), dm.object )		
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
	guienv:AddButton( 260, 220, "40+", "30+", wndDPP, -1, "+" ):SetAction( "./diskManager.IncMachineNumber()" )	
	guienv:AddButton( 260, 250, "40+", "30+", wndDPP, -1, "-" ):SetAction( "./diskManager.DecMachineNumber()" )
	
	--������� ������ ��������� ���������� ���� ������������
	labelNumberDay, _ = localAddLabel( "numberday", 275, 180, "120+", "30+", wndDPP )
	guienv:AddButton( 410, 160, "32+", "32+", wndDPP, -1, "+" ):SetAction( "./diskManager.IncDayNumber()" )
	guienv:AddButton( 410, 192, "32+", "32+", wndDPP, -1, "-" ):SetAction( "./diskManager.DecDayNumber()" )	 
	
	guienv:AddButton( 480, 220, "190+", "64+", wndDPP, -1, "-" ):SetAction( "./diskManager.SetAntipiratForce()" )
end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic.playerCompany
	plant = base.NrpGetPlant()
	produceDiskWork = base.CLuaPlantWork():Create( company:GetName() )
	
	if wndDPP == nil then
		wndDPP = guienv:AddWindow( "media/textures/MachinePlant.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		wndDPP:SetDraggable( false )
	else
		wndDPP:SetVisible( true )
	end
	
	--����, ������� ����� �����������
	localAddGames()
	
	--������� ������ ���������
	localAddMachines()	
	--������� ������� �� ����� � ����� ���� �� �������
	wndDPP:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.ItemSelected()" )
	
	localAddLabels()
	
	local btn = button.Stretch( 700, 385, "226+", "275+", "button_start", wndDPP, -1, "", "./diskManager.QueryAddWork()" )
	btn:SetHoveredImage( 0, 0, 0, 0, "media/buttons/button_start_next.png" )
		
	button.Stretch( "80e", "80e", "10e", "10e", "button_down", wndDPP, -1, "", "./diskManager.Hide()" )
end

function IncDayNumber()
	dayOfProduce = dayOfProduce + 1
	produceDiskWork:SetNumberDay( dayOfProduce )
	
	UpdateLabels()
end

function DecDayNumber()
	if dayOfProduce > 1 then
		dayOfProduce = dayOfProduce - 1
		produceDiskWork:SetNumberDay( dayOfProduce )
	end
	UpdateLabels()
end

function IncMachineNumber()
	numberMachine = numberMachine + localGetAddinMachine()
	produceDiskWork:SetNumberMachine( numberMachine )
	
	UpdateLabels()
end

function DecMachineNumber( ptr )
	if numberMachine > 1 then
		numberMachine = numberMachine - localGetAddinMachine()
		produceDiskWork:SetNumberMachine( numberMachine )
	end
	
	UpdateLabels()
end

function CloseQueryAddWork()
	windowQueryWork:Remove()
end

function QueryAddWork()
	local machines = produceDiskWork:GetNumberMachine()
	windowQueryWork = guienv:AddWindow( "", "25%", 0, "50%+", "100%", -1, wndDPP )
	local listBox = guienv:AddListBox( "2%", "2%", "98%", "80%", -1, windowQueryWork )
	
	listBox:AddItem( "��������:"..currentGame:GetName(), nil )
	listBox:AddItem( base.string.format( "���� �����: %.2f", produceDiskWork:GetDiskPrice() ), nil )	
	listBox:AddItem( produceDiskWork:GetNumberDisk().." ��", nil )	
	listBox:AddItem( base.string.format( "$%d", localGetFinalPrice() ), nil )
	listBox:AddItem( "��������� ������: $"..produceDiskWork:GetRentPrice() * machines, nil)
	listBox:AddItem( "������������������ (��\���):"..produceDiskWork:GetHourPerfomance() * machines, nil ) 
	listBox:AddItem( "��������� ������ ($\���):"..produceDiskWork:GetHourPrice() * machines, nil )
	listBox:AddItem( base.string.format( "���� �������������� �����: $ %.2f", produceDiskWork:GetAdvPrice() ), nil )
	listBox:AddItem( "����� ������ �� ����:"..produceDiskWork:GetDiskInDay().." ��", nil )			
	listBox:AddItem( "��������� ������:"..produceDiskWork:GetHourPrice().." ($/���)", nil )
	listBox:AddItem( "������������������:"..produceDiskWork:GetHourPerfomance().." (��/���)", nil ) 
	listBox:AddItem( "����� �� ����������: $"..produceDiskWork:GetRentPrice(), nil )
	listBox:AddItem( "������ �������������: "..base.string.format( "%d $", localGetFinalDiscount() * 100 ), nil )
	listBox:AddItem( "����� ������������: "..produceDiskWork:GetNumberMachine(), nil )
	listBox:AddItem( "���� ������������: "..produceDiskWork:GetNumberDay().." (��)", nil )
	
	local dd = base.string.format( "�������� %d", localGetFinalPrice() )
	button.Stretch( "25%", "82%", "24%+", "98%", "", windowQueryWork, -1, dd, "./diskManager.AddWork()" )
	button.Stretch( "51%", "82%", "24%+", "98%", "", windowQueryWork, -1, "�������", "./diskManager.CloseQueryAddWork()" )
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

function AddWork()	
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
