local base = _G

module( "plant" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local company = nil
local wndDPP = nil
local produceDiskWork = nil

local dayOfProduce = 0
local numberMachine = 0
local labelPricePrint = nil
local currentDiskMachine = nil
local labelPerdomance = nil
local labelPriceHour = nil
local labelNumberMachine = nil
local labelNumberDay = nil
local labelDiskNumber = nil
local labelFinalPrice = nil
local labelDiskPrice = nil
local labelAdvPrice = nil
local labelDiskInDay = nil
local cmbxProduceType = nil
local cmbxGames = nil
local plantWindow = mil

local addons = { }

function Hide()
	plantWindow:Remove()
	plantWindow = nil
end

function Show()
	if plantWindow then
		plantWindow:SetVisible( true )
	else
		plantWindow = guienv:AddWindow( "media/maps/plant_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		plantWindow:GetCloseButton():SetVisible( false )
		plantWindow:SetDraggable( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", plantWindow:Self(), -1, "",
						"./plant.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_PLANT )
	
	--box manager
	button.EqualeTexture( 94, 29, "boxManager", plantWindow:Self(), -1, "", "./gameboxManager.Show()" )
	--produce
	button.EqualeTexture( 407, 1, "produce", plantWindow:Self(), -1, "", "./plant.ShowDiskManager()" )
end

function ShowDiskManager()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic:GetPlayerCompany()
	
	produceDiskWork = base.CLuaPlantWork():Create( company:GetName() )
	
	if wndDPP == nil then
		wndDPP = guienv:AddWindow( "media/maps/plant_Select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		wndDPP:SetName( base.WINDOW_DISKPRODUCEPLANT_NAME )
		wndDPP:SetDraggable( false )
	else
		wndDPP:SetVisible( true )
	end
	
	--������� ���������� ������ ���� ���������
	cmbxProduceType = guienv:AddComboBox( "", 10, 20, scrWidth / 2 - 10, 40, -1, wndDPP:Self() )
	for i=1, base.applic:GetDiskMachineNumber() do
		local dm = base.applic:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm:GetName(), dm:Self() )		
	end
	
	cmbxGames = guienv:AddComboBox( "", 10, scrHeight / 2, scrWidth / 2 - 10, scrHeight / 2 + 20,  -1, wndDPP:Self() )
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		if game:HaveBox() then 
			cmbxGames:AddItem( game:GetName(), game:Self() )
		end	
	end

	wndDPP:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, "./plant.ComboboxItemSelected()" )
	
	--������� ����� ���� �� ���� ������ ��������
	labelPricePrint = guienv:AddLabel( "����� �� ����������:", 
												scrWidth / 2 + 10, 20, scrWidth - 10, 40, 
												-1, wndDPP:Self() )
	--������� ����� ������������������ ��������
	labelPerdomance = guienv:AddLabel( "������������������ (�������\���):", 
												scrWidth / 2 + 10, 50, scrWidth - 10, 70, 
												-1, wndDPP:Self() )
	--������� ����� ��������� ������ � ���
	labelPriceHour = guienv:AddLabel( "��������� ������ ($\���):", 
											   scrWidth / 2 + 10, 80, scrWidth - 10, 100, 
											   -1, wndDPP:Self() )
	
	--������� ������ ��������� ���������� ��������� ��� ������������ ������
	guienv:AddButton( 10, 50, 60, 100, wndDPP:Self(), -1, "+" ):SetAction( "./plant.IncMachineNumber()" )
	
	labelNumberMachine = guienv:AddLabel( "���������� ����� ������:",
												   100, 50, scrWidth / 2 - 60, 100, 
												   -1, wndDPP:Self() )
	guienv:AddButton( scrWidth / 2 - 60, 50, scrWidth / 2 - 10, 100, 
					  wndDPP:Self(), -1, "-" ):SetAction( "./plant.DecMachineNumber()" )
	
	--������� ������ ��������� ���������� ���� ������������
	guienv:AddButton( 10, 110, 60, 160, wndDPP:Self(), -1, "+" ):SetAction( "./plant.IncDayNumber()" )
	
	labelNumberDay = guienv:AddLabel( "���� ������������:", 
											   100, 110, scrWidth / 2 - 60, 160, 
											   -1, wndDPP:Self() )
	guienv:AddButton( scrWidth / 2 - 60, 110, scrWidth / 2 - 10, 160, 
					  wndDPP:Self(), -1, "-" ):SetAction( "./plant.DecDayNumber()" )
	
	--������� ����� ���������� ������������� ������
	labelDiskNumber = guienv:AddLabel( "���������� ������:", 
												scrWidth / 2 + 10, 170, scrWidth - 10, 220, 
												-1, wndDPP:Self() )
	
	--������� ����� ����� ���� �� ������������
	labelFinalPrice = guienv:AddLabel( "����� ����:", 
											    scrWidth / 2 + 10, 230, scrWidth - 10, 280, 
											    -1, wndDPP:Self() )
	
	--������� ����� ���� ������ �����
	labelDiskPrice = guienv:AddLabel( "���� ������ �����:", 
											   scrWidth / 2 + 10, 290, scrWidth - 10, 340, 
											   -1, wndDPP:Self() )
	--������� ����� ��������� �������������� �����
	labelAdvPrice = guienv:AddLabel(  "���� �������������� �����:", 
											   scrWidth / 2 + 10, 350, scrWidth - 10, 400, 
											   -1, wndDPP:Self() )

	--������� ����� ���������� ������������� ������ �� ����
	labelDiskInDay = guienv:AddLabel(  "����� ������ �� ����:", 
											   scrWidth / 2 + 10, 410, scrWidth - 10, 450, 
											   -1, wndDPP:Self() )

											   
	guienv:AddButton( 10, scrHeight - 50, scrWidth / 2 - 10, scrHeight - 10, 
					  wndDPP:Self(), -1, "���������" ):SetAction( "./plant.AddWork()" )
	
	guienv:AddButton( scrWidth / 2 + 10, scrHeight - 50, scrWidth - 10, scrHeight - 10, 
					  wndDPP:Self(), -1, "�����" ):SetAction( "./plant.HideProduceManager()" )
end

function AddWork()
	plant:AddProduceWork( produceDiskWork:Self() )
	
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

function HideProduceManager()
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

local function UpdateAddons( ptrGame )
	local game = base.CLuaGame( ptrGame )
	
	for i=1, #addons do
		addons[ 1 ]:Remove()
		table.remove( addons, 1 )
	end
	
	local addonsNumber = game:GetBoxAddonsNumber()
	for i=1, addonsNumber do
		local addon = game:GetBoxAddon( i-1 )
		local link = guienv:AddLinkBox( addon:GetName(), 
					  				    20 + 70 * ( i - 1 ), 20 + scrHeight * 0.75,
													 20 + 70 * i, 20 + height * 0.75 + 70,
													 -1, wndDPP:Self() )
		link:SetData( game:GetBoxAddon( i-1 ) )
		addons[ i ] = link
	end
end

local function UpdateLabels()
	labelPricePrint:SetText( "����� �� ����������:"..produceDiskWork:GetRentPrice().."$" )
	labelPerdomance:SetText( "������������������:"..produceDiskWork:GetHourPerfomance().."(�������\���)" )
	labelPriceHour:SetText( "��������� ������:"..produceDiskWork:GetHourPrice().."($\���)" )
	labelNumberMachine:SetText( "���������� ����� ������:"..produceDiskWork:GetNumberMachine().."(��)" )
	labelNumberDay:SetText( "���� ������������:"..produceDiskWork:GetNumberDay().."(��)" )
	labelDiskNumber:SetText(  "���������� ������:"..produceDiskWork:GetNumberDisk().."(��)" ) 
	labelDiskPrice:SetText( "���� ������ �����:"..base.string.format( "%.2f", produceDiskWork:GetDiskPrice() ).."$" )  
	labelFinalPrice:SetText( "����� ���������:"..produceDiskWork:GetPrice().."$" )
	labelAdvPrice:SetText( "���� �������������� �����:"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ).."$" )
	labelDiskInDay:SetText( "����� ������ �� ����:"..produceDiskWork:GetDiskInDay().." ��" )
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

local function localGetAddinMachine()
	if numberMachine < 10 then return 1 end
	if numberMachine > 10 and numberMachine < 30 then return 5 end
	if numberMachine > 30 and numberMachine < 100 then return 10 end
	if numberMachine > 100 and numberMachine < 500 then return 50 end
	if numberMachine > 500 then return 100 end
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

function ComboboxItemSelected()
	local cmbx = base.CLuaComboBox( base.NrpGetSender() )
	
	if cmbxProduceType:Self() == cmbx:Self() then
		currentDiskMachine:SetObject( cmbx:GetSelectedObject() )
		produceDiskWork:SetProduceType( currentDiskMachine:Self() )
		UpdateLabels()
		return
	end
	
	if cmbxGames:Self() == cmbx:Self() then
		produceDiskWork:SetGame( cmbx:GetSelectedObject() )
		UpdateAddons( cmbx:GetSelectedObject() )
		UpdateLabels()			
	end
end