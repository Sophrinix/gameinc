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

local cmbxGames = nil

local addons = { }

local function localGetAddinMachine()
	if numberMachine < 10 then return 1 
	elseif numberMachine > 10 and numberMachine < 30 then return 5 
	elseif numberMachine > 30 and numberMachine < 100 then return 10 
	elseif numberMachine > 100 and numberMachine < 500 then return 50 
	elseif numberMachine > 500 then return 100 end
end

local function UpdateAddons( ptrGame )
	local game = base.CLuaGame( ptrGame )
	
	for i=1, #addons do
		addons[ 1 ]:Remove()
		base.table.remove( addons, 1 )
	end
	
	local addonsNumber = game:GetBoxAddonsNumber()
	for i=1, addonsNumber do
		local addon = game:GetBoxAddon( i-1 )
		local link = guienv:AddLinkBox( addon:GetName(), 
					  				    20 + 70 * ( i - 1 ), 20 + scrHeight * 0.75,
													 20 + 70 * i, 20 + scrHeight * 0.75 + 70,
													 -1, wndDPP:Self() )
		link:SetData( game:GetBoxAddon( i-1 ) )
		addons[ i ] = link
	end
end

local function localAddLabel( text, yoff, left )
	local offset = scrWidth / 2 + 10
	
	if left and left == true then offset = 10 end
	
	local lb = guienv:AddLabel( text, offset, yoff, offset + scrWidth / 2 - 20, yoff + 20, -1, wndDPP:Self() )
	lb:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )	
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	return lb, yoff + 40
end

local function localAddGames()
	localAddLabel( "Какую игру будем штамповать?", 40 )
	cmbxGames = guienv:AddPictureFlow( scrWidth / 2 + 10, 60, scrWidth - 10, 60 + 150, -1, wndDPP:Self() )
	cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	cmbxGames:SetDrawBorder( false )
	
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		if game:HaveBox() then 
			cmbxGames:AddItem( game:GetViewImage(), game:GetName(), game:Self() )
		end	
	end
end

local function  localAddMachines()
	localAddLabel( "Выбери тип производства?", 40, true )
	cmbxProduceType = guienv:AddPictureFlow( 10, 60, scrWidth / 2 - 10, 60 + 150, -1, wndDPP:Self() )
	cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	cmbxProduceType:SetDrawBorder( false )
	
	for i=1, plant:GetDiskMachineNumber() do
		local dm = plant:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm:GetTexture(), dm:GetName(), dm:Self() )		
	end
end

local function localAddLabels()
	local yoff = 250
	--добавим метку цены за наем одного аппарата
	labelPricePrintAll, yoff = localAddLabel( "Стоимость размещение заказа:", yoff )
	--Добавим метку производительности аппарата
	labelPerfomanceAll, yoff = localAddLabel( "Производительность линии (коробок\час):", yoff ) 
	--добавим метку стоимости работы в час
	labelPriceHourAll, yoff =  localAddLabel( "Стоимость работы ($\час):", yoff )
	--добавим метку количества произведенных дисков
	labelDiskNumber, yoff = localAddLabel( "Количество дисков:", yoff )	
	--Добавим метку стоимости дополнительных фишек
	labelAdvPrice, yoff =   localAddLabel(  "Цены дополнительных вещей:", yoff )
	--добавим метку цены одного диска
	labelDiskPrice, yoff =  localAddLabel( "Цена одного диска:", yoff )
	--Добавим метку количества произведенных дисков за день
	labelDiskInDay, yoff =  localAddLabel(  "Будет произведено в день:", yoff )			
	--Добавим метку количества произведенных дисков за день
	labelDiskTrash, yoff =  localAddLabel(  "Вероятность поломки:", yoff )	
	
	--добавим метку общей цены за производство
	labelFinalPrice, yoff = localAddLabel( "Всего:", yoff )
	
	yoff = 250
	--добавим кнопки изменения количества аппаратов для производства дисков
	
	labelNumberMachine = localAddLabel( "Количество линий сборки:", yoff, true )
	guienv:AddButton( 10, yoff, 60, yoff + 50, wndDPP:Self(), -1, "+" ):SetAction( "./diskManager.IncMachineNumber()" )	
	guienv:AddButton( scrWidth / 2 - 60, yoff, scrWidth / 2 - 10, yoff + 50, wndDPP:Self(), -1, "-" ):SetAction( "./diskManager.DecMachineNumber()" )
	
	yoff = yoff + 60
	--добавим кнопки изменения количества дней производства
	labelNumberDay = localAddLabel( "Дней производства:", yoff, true )
	guienv:AddButton( 10, yoff, 60, yoff + 50, wndDPP:Self(), -1, "+" ):SetAction( "./diskManager.IncDayNumber()" )
	guienv:AddButton( scrWidth / 2 - 60, yoff, scrWidth / 2 - 10, yoff+50, wndDPP:Self(), -1, "-" ):SetAction( "./diskManager.DecDayNumber()" )	 
	
	yoff = yoff + 60
	--добавим метку цены за наем одного аппарата
	labelPricePrint, yoff = localAddLabel( "Стоимость аренды:", yoff, true )
	labelPerfomance, yoff = localAddLabel( "Производительность (коробок\час):", yoff, true ) 
	labelPriceHour, yoff =  localAddLabel( "Стоимость работы ($\час):", yoff, true )
	labelRelationWithPlant, yoff = localAddLabel( "Скидка производителя: ", yoff, true )
end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic:GetPlayerCompany()
	plant = base.NrpGetPlant()
	produceDiskWork = base.CLuaPlantWork():Create( company:GetName() )
	
	if wndDPP == nil then
		wndDPP = guienv:AddWindow( "media/maps/plant_Select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		wndDPP:SetName( base.WINDOW_DISKPRODUCEPLANT_NAME )
		wndDPP:SetDraggable( false )
	else
		wndDPP:SetVisible( true )
	end
	
	--игры, которые можно производить
	localAddGames()
	
	--добавим список аппаратов
	localAddMachines()	
	--повесим событие на выбор в каком либо из списков
	wndDPP:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.ItemSelected()" )
	
	localAddLabels()
	
	guienv:AddButton( 10, scrHeight - 50, scrWidth / 2 - 10, scrHeight - 10, wndDPP:Self(), -1, "Запустить" ):SetAction( "./diskManager.QueryAddWork()" )
	guienv:AddButton( scrWidth / 2 + 10, scrHeight - 50, scrWidth - 10, scrHeight - 10, wndDPP:Self(), -1, "Выход" ):SetAction( "./diskManager.Hide()" )
end


local function UpdateLabels()
	local machines = produceDiskWork:GetNumberMachine();
	
	labelPricePrint:SetText( "Плата за размещение: $"..produceDiskWork:GetRentPrice() )
	labelPerfomance:SetText( "Производительность:"..produceDiskWork:GetHourPerfomance().." (шт/час)" )
	labelPriceHour:SetText( "Стоимость работы:"..produceDiskWork:GetHourPrice().." ($/час)" )
	labelNumberMachine:SetText( "Количество линий сборки:"..produceDiskWork:GetNumberMachine() )
	labelNumberDay:SetText( "Дней производства:"..produceDiskWork:GetNumberDay().." (дн)" )
	labelDiskNumber:SetText(  "Количество дисков:"..produceDiskWork:GetNumberDisk().." шт" ) 
	labelDiskPrice:SetText( "Цена одного диска: $"..base.string.format( "%.2f", produceDiskWork:GetDiskPrice() ) )  
	
	local discount = currentDiskMachine:GetDiscount() + currentDiskMachine:GetLineDiscount() * machines
	local finalPrice = produceDiskWork:GetPrice() * ( 1 - discount )
	labelFinalPrice:SetText( "Общая стоимость: $"..base.string.format( "%d", finalPrice ) )
	
	labelAdvPrice:SetText( "Цены дополнительных вещей: $"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ) )
	labelDiskInDay:SetText( "Всего дисков за день:"..produceDiskWork:GetDiskInDay().." шт" )
	labelPricePrintAll:SetText( "Стоимость аренды: $"..produceDiskWork:GetRentPrice() * machines )
	labelPerfomanceAll:SetText( "Производительность (шт\час):"..produceDiskWork:GetHourPerfomance() * machines ) 
	labelPriceHourAll:SetText( "Стоимость работы ($\час):"..produceDiskWork:GetHourPrice() * machines )
	labelRelationWithPlant:SetText( "Скидка производителя: "..base.string.format( "%d", discount * 100 ).."%" )
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

function ItemSelected()
	local lbx = base.CLuaPictureFlow( base.NrpGetSender() )
	
	if cmbxProduceType:Self() == lbx:Self() then
		currentDiskMachine = base.CLuaDiskMachine( lbx:GetSelectedObject() )
		produceDiskWork:SetProduceType( currentDiskMachine:Self() )
		UpdateLabels()
	elseif cmbxGames:Self() == lbx:Self() then
		produceDiskWork:SetGame( lbx:GetSelectedObject() )
		UpdateAddons( lbx:GetSelectedObject() )
		UpdateLabels()			
	end
end

function CloseQueryAddWork()
	local parent = base.CLuaElement( base.NrpGetSender() ):GetParent()
	base.CLuaElement( parent ):Remove()
end

function QueryAddWork()
	local discount = currentDiskMachine:GetDiscount() + currentDiskMachine:GetLineDiscount() * produceDiskWork:GetNumberMachine()
	local finalPrice = produceDiskWork:GetPrice() * ( 1 - discount )
	guienv:MessageBox( "Размещение заказа стоит "..finalPrice.."$. Продолжить?", true, true, "./diskManager.AddWork()", "./diskManager.CloseQueryAddWork()" )
end

function localChangeProducerDiscount()
	local lastOrder = produceDiskWork:GetNumberDisk()
	local nDisk = currentDiskMachine:GetDiskProduced( company:GetName() )
	local discount = currentDiskMachine:GetDiscount()
	if nDisk < 100000 then nDisk = 100000 end
	
	discount =  discount + lastOrder / nDisk 
	--есть предел скидки для каждого производителя
	--его нельзя превыщать
	if discount > currentDiskMachine:GetMaxDiscount() then
		discount = currentDiskMachine:GetMaxDiscount()
	end
	
	--установим новую скидку для игрока на следующий заказ
	currentDiskMachine:SetDiscount( discount )

	--у всех остальных производителей снизим отнощение к игроку
	for i=1, plant:GetDiskMachineNumber() do
		local dm = plant:GetDiskMachine( i-1 )
		if dm:GetName() ~= currentDiskMachine:GetName() then
			dm:SetDiscount( dm:GetDiscount() - 0.05 )		
		end		
	end
end

function AddWork()
	local discount = currentDiskMachine:GetDiscount() + currentDiskMachine:GetLineDiscount() * produceDiskWork:GetNumberMachine()
	local finalPrice = produceDiskWork:GetPrice() * ( 1 - discount )
	
	--изменим отношение производителя к игроку за размещение заказа
	localChangeProducerDiscount()
	
	company:AddBalance( -finalPrice )
	plant:AddProduceWork( produceDiskWork:Self() )
	
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

function Hide()
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end
