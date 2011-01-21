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
local labelGameName = nil 

local windowSelectMachine = nil
local windowSelectGame = nil

local cmbxGames = nil

local addons = { }

local function localGetAddinMachine()
	if numberMachine < 10 then return 1 
	elseif numberMachine >= 10 and numberMachine < 30 then return 5 
	elseif numberMachine >= 30 and numberMachine < 100 then return 10 
	elseif numberMachine >= 100 and numberMachine < 500 then return 50 
	elseif numberMachine >= 500 then return 100 end
end

local function UpdateAddons( ptrGame )
	local game = base.CLuaGame( ptrGame )
	
	for i=1, #addons do
		addons[ 1 ]:Remove()
		base.table.remove( addons, 1 )
	end
	
	for i=1, game.boxAddonsNumber do
		local addon = game:GetBoxAddon( i-1 )
		local link = guienv:AddLinkBox( addon:GetName(), 
					  				    20 + 70 * ( i - 1 ), 20 + scrHeight * 0.75,
													 20 + 70 * i, 20 + scrHeight * 0.75 + 70,
													 -1, wndDPP )
		link:SetData( game:GetBoxAddon( i-1 ) )
		addons[ i ] = link
	end
end

local function localAddLabel( text, xpos, ypos, ww, hh )
	if xpos then offset = xpos end
	
	local lb = guienv:AddLabel( text, xpos, yoff, ww, hh, -1, wndDPP )
	lb:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )	
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	return lb, ypos + 40
end

function SelectGame()
	windowSelectGame:SetVisible( true )
end

local function localAddGames()
	local btn = guienv:AddButton( 100, 400, "150+", "230+", wndDPP, -1, "Выбрать\n игру" )
	btn:SetAction( "./diskManager.SelectGame()" )

	windowSelectGame = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectGame:SetVisible( false )
	windowSelectGame:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.GameSelected()" )
	
	cmbxGames = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectGame )
	cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	cmbxGames:SetDrawBorder( false )
	
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		if game:HaveBox() then 
			cmbxGames:AddItem( game:GetViewImage(), game:GetName(), game:Self() )
		end	
	end
end

function SelectMachine()
	windowSelectMachine:SetVisible( true )
end

local function  localAddMachines()
	local btn = guienv:AddButton( 130, 170, 250, 265, wndDPP, -1, "Выбрать\n производство" )
	btn:SetAction( "./diskManager.SelectMachine()" )
	
	windowSelectMachine = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectMachine:SetVisible( false )
	windowSelectMachine:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./diskManager.MachineSelected()" )
	
	cmbxProduceType = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectMachine )
	cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	cmbxProduceType:SetDrawBorder( false )
	
	for i=1, plant:GetDiskMachineNumber() do
		local dm = plant:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm:GetTexture(), dm:GetName(), dm:Self() )		
	end
end

local function localAddLabels()

	labelGameName, _ = localAddLabel( "???", 270, 60, "400+", "40+" )
	--добавим метку цены одного диска
	labelDiskPrice, yoff =  localAddLabel( "???", 330, 330, "140+", "30+" )
	
	--добавим метку общей цены за производство
	labelDiskNumber, yoff = localAddLabel( "???", 330, 490, "140+", "30+" )	
	
	labelFinalPrice, yoff = localAddLabel( "Всего:", 330, 400, "140+", "30+" )
	
	--добавим метку цены за наем одного аппарата
	--labelPricePrintAll, yoff = localAddLabel( "Стоимость размещение заказа:", yoff )
	--Добавим метку производительности аппарата
	--labelPerfomanceAll, yoff = localAddLabel( "Производительность линии (коробок\час):", yoff ) 
	--добавим метку стоимости работы в час
	--labelPriceHourAll, yoff =  localAddLabel( "Стоимость работы ($\час):", yoff )
	--добавим метку количества произведенных дисков
	--
	--Добавим метку стоимости дополнительных фишек
	--labelAdvPrice, yoff =   localAddLabel(  "Цены дополнительных вещей:", yoff )
	
	
	--Добавим метку количества произведенных дисков за день
	--labelDiskInDay, yoff =  localAddLabel(  "Будет произведено в день:", yoff )			
	--Добавим метку количества произведенных дисков за день
	--labelDiskTrash, yoff =  localAddLabel(  "Вероятность поломки:", yoff )	
	
	yoff = 250
	--добавим кнопки изменения количества аппаратов для производства дисков
	
	guienv:AddButton( 260, 220, "40+", "30+", wndDPP, -1, "+" ):SetAction( "./diskManager.IncMachineNumber()" )	
	guienv:AddButton( 260, 250, "40+", "30+", wndDPP, -1, "-" ):SetAction( "./diskManager.DecMachineNumber()" )
	
	--добавим кнопки изменения количества дней производства
	labelNumberDay = localAddLabel( "???", 175, 180, "120+", "30+" )
	guienv:AddButton( 410, 160, "32+", "32+", wndDPP, -1, "+" ):SetAction( "./diskManager.IncDayNumber()" )
	guienv:AddButton( 410, 192, "32+", "32+", wndDPP, -1, "-" ):SetAction( "./diskManager.DecDayNumber()" )	 
	
	guienv:AddButton( 480, 220, "190+", "64+", wndDPP, -1, "-" ):SetAction( "./diskManager.SetAntipiratForce()" )
	--добавим метку цены за наем одного аппарата
	--labelPricePrint, yoff = localAddLabel( "Стоимость аренды:", yoff, true )
	--labelPerfomance, yoff = localAddLabel( "Производительность (коробок\час):", yoff, true ) 
	--labelPriceHour, yoff =  localAddLabel( "Стоимость работы ($\час):", yoff, true )
	--labelRelationWithPlant, yoff = localAddLabel( "Скидка производителя: ", yoff, true )
end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic.playerCompany
	plant = base.NrpGetPlant()
	produceDiskWork = base.CLuaPlantWork():Create( company:GetName() )
	
	if wndDPP == nil then
		wndDPP = guienv:AddWindow( "media/textures/MachinePlant.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
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
	
	guienv:AddButton( 10, scrHeight - 50, scrWidth / 2 - 10, scrHeight - 10, wndDPP, -1, "Запустить" ):SetAction( "./diskManager.QueryAddWork()" )
	guienv:AddButton( scrWidth / 2 + 10, scrHeight - 50, scrWidth - 10, scrHeight - 10, wndDPP, -1, "Выход" ):SetAction( "./diskManager.Hide()" )
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
	
	labelPricePrint:SetText( "Плата за размещение: $"..produceDiskWork:GetRentPrice() )
	labelPerfomance:SetText( "Производительность:"..produceDiskWork:GetHourPerfomance().." (шт/час)" )
	labelPriceHour:SetText( "Стоимость работы:"..produceDiskWork:GetHourPrice().." ($/час)" )
	labelNumberMachine:SetText( "Количество линий сборки:"..produceDiskWork:GetNumberMachine() )
	labelNumberDay:SetText( "Дней производства:"..produceDiskWork:GetNumberDay().." (дн)" )
	labelDiskNumber:SetText(  "Количество дисков:"..produceDiskWork:GetNumberDisk().." шт" ) 
	labelDiskPrice:SetText( "Цена одного диска: $"..base.string.format( "%.2f", produceDiskWork:GetDiskPrice() ) )  
	labelFinalPrice:SetText( "Общая стоимость: $"..base.string.format( "%d", localGetFinalPrice() ) )
	labelAdvPrice:SetText( "Цены дополнительных вещей: $"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ) )
	labelDiskInDay:SetText( "Всего дисков за день:"..produceDiskWork:GetDiskInDay().." шт" )
	labelPricePrintAll:SetText( "Стоимость аренды: $"..produceDiskWork:GetRentPrice() * machines )
	labelPerfomanceAll:SetText( "Производительность (шт\час):"..produceDiskWork:GetHourPerfomance() * machines ) 
	labelPriceHourAll:SetText( "Стоимость работы ($\час):"..produceDiskWork:GetHourPrice() * machines )
	labelRelationWithPlant:SetText( "Скидка производителя: "..base.string.format( "%d", localGetFinalDiscount() * 100 ).."%" )
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
	local dd = base.string.format( "%d", localGetFinalPrice() )
	guienv:MessageBox(  "Размещение заказа стоит "..dd.."$. Продолжить?", 
						true, true, 
						"./diskManager.AddWork()", "./diskManager.CloseQueryAddWork()" )
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
	--изменим отношение производителя к игроку за размещение заказа
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
