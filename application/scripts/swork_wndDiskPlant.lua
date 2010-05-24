local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )
local plant = CLuaPlant( NrpGetPlant() )
local company = CLuaCompany( nil )
local wndDPP = CLuaWindow( nil )
local produceDiskWork = CLuaPlantWork( nil )
local dayOfProduce = 0
local numberMachine = 0
local labelPricePrint = CLuaLabel( nil )
local currentDiskMachine = CLuaDiskMachine( nil )
local labelPerdomance = CLuaLabel( nil )
local labelPriceHour = CLuaLabel( nil )
local labelNumberMachine = CLuaLabel( nil )
local labelNumberDay = CLuaLabel( nil )
local labelDiskNumber = CLuaLabel( nil )
local labelFinalPrice = CLuaLabel( nil )
local labelDiskPrice = CLuaLabel( nil )
local labelAdvPrice = CLuaLabel( nil )
local labelDiskInDay = CLuaLabel( nil )
local cmbxProduceType = CLuaComboBox( nil )
local cmbxGames = CLuaComboBox( nil )

local width = 800
local height = 600

local addons = { }

function sworkCreateDiskProducePlantWindow( ptr )
	numberMachine = 0
	dayOfProduce = 0
	company:SetObject( applic:GetPlayerCompany() )
	
	produceDiskWork:Create( company:GetName() )
	wndDPP:SetObject( guienv:GetElementByName( WINDOW_DISKPRODUCEPLANT_NAME ) )
	wndDPP:Remove()
	
	wndDPP:SetObject( guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_DISKPRODUCEPLANT_ID, guienv:GetRootGUIElement() ) )
	wndDPP:SetName( WINDOW_DISKPRODUCEPLANT_NAME )
	
	--добавим выпадающий список типа аппаратов
	cmbxProduceType:SetObject( guienv:AddComboBox( "", 10, 20, width / 2 - 10, 40, -1, wndDPP:Self() ) )
	for i=1, applic:GetDiskMachineNumber() do
		local dm = CLuaDiskMachine( applic:GetDiskMachine( i-1 ) )
		cmbxProduceType:AddItem( dm:GetName(), dm:Self() )		
	end
	
	cmbxGames:SetObject( guienv:AddComboBox( "", 10, height / 2, width / 2 - 10, height / 2 + 20,  -1, wndDPP:Self() ) )
	for i=1, company:GetGameNumber() do
		local game = CLuaGame( company:GetGame( i-1 ) )
		if game:HaveBox() then 
			cmbxGames:AddItem( game:GetName(), game:Self() )
		end	
	end

	wndDPP:AddLuaFunction( GUIELEMENT_CMBXITEM_SELECTED, "sworkWndDiskProducePlantCmbxItemSelected" )
	
	--добавим метку цены за наем одного аппарата
	labelPricePrint:SetObject( guienv:AddLabel( "Плата за размещение:", 
												width / 2 + 10, 20, width - 10, 40, 
												-1, wndDPP:Self() ) )
	--Добавим метку производительности аппарата
	labelPerdomance:SetObject( guienv:AddLabel( "Производительность (коробок\час):", 
												width / 2 + 10, 50, width - 10, 70, 
												-1, wndDPP:Self() ) )
	--добавим метку стоимости работы в час
	labelPriceHour:SetObject( guienv:AddLabel( "Стоимость работы ($\час):", 
											   width / 2 + 10, 80, width - 10, 100, 
											   -1, wndDPP:Self() ) )
	
	--добавим кнопки изменения количества аппаратов для производства дисков
	local btn = CLuaButton( guienv:AddButton( 10, 50, 60, 100, wndDPP:Self(), -1, "+" ) )
	btn:SetAction( "sworkWndDiskProducePlantIncMachineNumber" )
	
	labelNumberMachine:SetObject( guienv:AddLabel( "Количество линий сборки:",
												   100, 50, width / 2 - 60, 100, 
												   -1, wndDPP:Self() ) )
	btn:SetObject( guienv:AddButton( width / 2 - 60, 50, width / 2 - 10, 100, wndDPP:Self(), -1, "-" ) )
	btn:SetAction( "sworkWndDiskProducePlantDecMachineNumber" )
	
	--добавим кнопки изменения количества дней производства
	btn:SetObject( guienv:AddButton( 10, 110, 60, 160, wndDPP:Self(), -1, "+" ) )
	btn:SetAction( "sworkWndDiskProducePlantIncDayNumber" )
	
	labelNumberDay:SetObject( guienv:AddLabel( "Дней производства:", 
											   100, 110, width / 2 - 60, 160, 
											   -1, wndDPP:Self() ) )
	btn:SetObject( guienv:AddButton( width / 2 - 60, 110, width / 2 - 10, 160, wndDPP:Self(), -1, "-" ) )
	btn:SetAction( "sworkWndDiskProducePlantDecDayNumber" )
	
	--добавим метку количества произведенных дисков
	labelDiskNumber:SetObject( guienv:AddLabel( "Количество дисков:", 
												width / 2 + 10, 170, width - 10, 220, 
												-1, wndDPP:Self() ) )
	
	--добавим метку общей цены за производство
	labelFinalPrice:SetObject( guienv:AddLabel( "Общая цена:", 
											    width / 2 + 10, 230, width - 10, 280, 
											    -1, wndDPP:Self() ) )
	
	--добавим метку цены одного диска
	labelDiskPrice:SetObject( guienv:AddLabel( "Цена одного диска:", 
											   width / 2 + 10, 290, width - 10, 340, 
											   -1, wndDPP:Self() ) )
	--Добавим метку стоимости дополнительных фишек
	labelAdvPrice:SetObject( guienv:AddLabel(  "Цены дополнительных вещей:", 
											   width / 2 + 10, 350, width - 10, 400, 
											   -1, wndDPP:Self() ) )

	--Добавим метку количества произведенных дисков за день
	labelDiskInDay:SetObject( guienv:AddLabel(  "Всего дисков за день:", 
											   width / 2 + 10, 410, width - 10, 450, 
											   -1, wndDPP:Self() ) )

											   
	local createBtn = CLuaButton( guienv:AddButton( 10, height - 50, width / 2 - 10, height - 10, 
													wndDPP:Self(), -1, "Запустить" ) )
	createBtn:SetAction( "sworkWndDiskProducePlantLocate" )
	
	local closeBtn = CLuaButton( guienv:AddButton( width / 2 + 10, height - 50, width - 10, height - 10, 
												   wndDPP:Self(), -1, "Выход" ) )
	closeBtn:SetAction( "sworkWndDiskProducePlantClose" )
end

function sworkWndDiskProducePlantLocate( ptr )
	plant:AddProduceWork( produceDiskWork:Self() )
	
	produceDiskWork:Remove()
	wndDPP:Remove()
end

function sworkWndDiskProducePlantClose( ptr )
	produceDiskWork:Remove()
	wndDPP:Remove()
end

local function UpdateAddons( ptrGame )
	local game = CLuaGame( ptrGame )
	
	for i=1, #addons do
		addons[ 1 ]:Remove()
		table.remove( addons, 1 )
	end
	
	local addonsNumber = game:GetBoxAddonsNumber()
	for i=1, addonsNumber do
		local addon = CLuaTech( game:GetBoxAddon( i-1 ) )
		local link = CLuaLinkBox( guienv:AddLinkBox( addon:GetName(), 
													 20 + 70 * ( i - 1 ), 20 + height * 0.75,
													 20 + 70 * i, 20 + height * 0.75 + 70,
													 -1, wndDPP:Self() ) )
		link:SetData( game:GetBoxAddon( i-1 ) )
		addons[ i ] = link
	end
end

local function UpdateLabels()
	labelPricePrint:SetText( "Плата за размещение:"..produceDiskWork:GetRentPrice().."$" )
	labelPerdomance:SetText( "Производительность:"..produceDiskWork:GetHourPerfomance().."(коробок\час)" )
	labelPriceHour:SetText( "Стоимость работы:"..produceDiskWork:GetHourPrice().."($\час)" )
	labelNumberMachine:SetText( "Количество линий сборки:"..produceDiskWork:GetNumberMachine().."(шт)" )
	labelNumberDay:SetText( "Дней производства:"..produceDiskWork:GetNumberDay().."(дн)" )
	labelDiskNumber:SetText(  "Количество дисков:"..produceDiskWork:GetNumberDisk().."(шт)" ) 
	labelDiskPrice:SetText( "Цена одного диска:"..string.format( "%.2f", produceDiskWork:GetDiskPrice() ).."$" )  
	labelFinalPrice:SetText( "Общая стоимость:"..produceDiskWork:GetPrice().."$" )
	labelAdvPrice:SetText( "Цены дополнительных вещей:"..string.format( "%.2f", produceDiskWork:GetAdvPrice() ).."$" )
	labelDiskInDay:SetText( "Всего дисков за день:"..produceDiskWork:GetDiskInDay().." шт" )
end

function sworkWndDiskProducePlantIncDayNumber( ptr )
	dayOfProduce = dayOfProduce + 1
	produceDiskWork:SetNumberDay( dayOfProduce )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantDecDayNumber( ptr )
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

function sworkWndDiskProducePlantIncMachineNumber( ptr )

	numberMachine = numberMachine + localGetAddinMachine()
	produceDiskWork:SetNumberMachine( numberMachine )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantDecMachineNumber( ptr )
	if numberMachine > 1 then
		numberMachine = numberMachine - localGetAddinMachine()
		produceDiskWork:SetNumberMachine( numberMachine )
	end
	
	UpdateLabels()
end

function sworkWndDiskProducePlantCmbxItemSelected( ptr )
	local cmbx = CLuaComboBox( ptr )
	
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