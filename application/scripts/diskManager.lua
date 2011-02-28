local base = _G

module( "diskManager" )

local button = base.button
local tutorial = base.tutorial
local window = base.window
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

local function _AddLabel( text, xpos, ypos, ww, hh, parent )
	local lb = guienv:AddLabel( text, xpos, ypos, ww, hh, -1, parent )
	lb.color = base.toColor( 0xff, 0xff, 0xff, 0xff )	
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	return lb, ypos + 40
end

local function _GetAddinMachine()
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

local function _GetFinalPrice()
	return produceDiskWork.price * ( 1 - _GetFinalDiscount() )
end

local function _UpdateLabels()
	local machines = produceDiskWork.numberMachine
	
	labelGameName.text = currentGame.name
	
	--labelPricePrint:SetText( "Плата за размещение: $"..produceDiskWork:GetRentPrice() )
	--labelPerfomance:SetText( "Производительность:"..produceDiskWork:GetHourPerfomance().." (шт/час)" )
	--labelPriceHour:SetText( "Стоимость работы:"..produceDiskWork:GetHourPrice().." ($/час)" )
	
	labelNumberMachine.text = produceDiskWork.numberMachine
	labelNumberDay.text = produceDiskWork.numberDay.." (дн)"
	labelDiskNumber.text = produceDiskWork.numberDisk.." шт" 
	labelDiskPrice.text = base.string.format( "%.2f", produceDiskWork.diskPrice )
	labelFinalPrice.text = "$"..base.string.format( "%d", _GetFinalPrice() )
	
	--labelAdvPrice:SetText( "Цены дополнительных вещей: $"..base.string.format( "%.2f", produceDiskWork:GetAdvPrice() ) )
	--labelDiskInDay:SetText( "Всего дисков за день:"..produceDiskWork:GetDiskInDay().." шт" )
	--labelPricePrintAll:SetText( "Стоимость аренды: $"..produceDiskWork:GetRentPrice() * machines )
	--labelPerfomanceAll:SetText( "Производительность (шт\час):"..produceDiskWork:GetHourPerfomance() * machines ) 
	--labelPriceHourAll:SetText( "Стоимость работы ($\час):"..produceDiskWork:GetHourPrice() * machines )
	
	--labelRelationWithPlant:SetText( "Скидка производителя: "..base.string.format( "%d", localGetFinalDiscount() * 100 ).."%" )
end

local function _SelectGame()
	guienv:BringToFront( windowSelectGame )
	windowSelectGame.visible = true
end

local function _SelectGameClose()
	windowSelectGame.visible = false
end

local function _GameSelected()
	currentGame = base.CLuaGame( cmbxGames.selectedObject )
	produceDiskWork:SetGame( currentGame )
	_UpdateLabels()		
	
	windowSelectGame.visible = false
	btnGame:SetImage( 0, 0, 0, 0, currentGame.viewImage )
end

local function _AddGames()
	btnGame = button.Stretch( 100, 400, "150+", "230+", "pure_button", wndDPP, -1, "Выбрать\n игру", _SelectGame )

	windowSelectGame = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectGame.visible = false
	windowSelectGame.draggable = false
	windowSelectGame.closeButton.visible = false
	
	cmbxGames = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectGame )
	cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	cmbxGames.drawBorder = false
	cmbxGames.onSelect = _GameSelected
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectGame, -1, "Выбрать", _SelectGame )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectGame, -1, "Закрыть", _SelectGameClose )
		
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		if game.haveBox then 
			cmbxGames:AddItem( game.viewImage, game.name, game.object )
		end	
	end
end

local function _SelectMachine()
	guienv:BringToFront( windowSelectMachine )
	windowSelectMachine.visible = true
end

local function _SelectMachineClose()
	windowSelectMachine.visible = false
end

local function _MachineSelected()
	currentDiskMachine = base.CLuaDiskMachine( cmbxProduceType.selectedObject )
	produceDiskWork.produceType = currentDiskMachine.object
	_UpdateLabels()
	btnMachine:SetImage( 0, 0, 0, 0, currentDiskMachine.texture )
end

local function  _AddMachines()
	btnMachine = button.Stretch( 130, 170, 250, 265, "pure_button", wndDPP, -1, "Выбрать\n производство", _SelectMachine )
	
	windowSelectMachine = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, wndDPP )
	windowSelectMachine.visible = false
	windowSelectMachine.draggable = false
	windowSelectMachine.closeButton.visible = false
	
	cmbxProduceType = guienv:AddPictureFlow( "5%", "5%", "80%", "80%", -1, windowSelectMachine )
	cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	cmbxProduceType.drawBorder = false
	cmbxProduceType.onSelect = _MachineSelected
	
	button.Stretch( "15%", "82%", "49%", "95%", "", windowSelectMachine, -1, "Выбрать", _SelectMachine )
	button.Stretch( "51%", "82%", "85%", "95%", "", windowSelectMachine, -1, "Закрыть", _SelectMachineClose )
	
	for i=1, plant.machineNumber do
		local dm = plant:GetDiskMachine( i-1 )
		cmbxProduceType:AddItem( dm.texture, dm.name, dm.object )		
	end
end

local function _IncDayNumber()
	dayOfProduce = dayOfProduce + 1
	produceDiskWork.numberDay = dayOfProduce
	
	_UpdateLabels()
end

local function _DecDayNumber()
	if dayOfProduce > 1 then
		dayOfProduce = dayOfProduce - 1
		produceDiskWork.numberDay = dayOfProduce
	end
	_UpdateLabels()
end

local function _IncMachineNumber()
	numberMachine = numberMachine + _GetAddinMachine()
	produceDiskWork.numberMachine = numberMachine
	
	_UpdateLabels()
end

local function _DecMachineNumber( ptr )
	if numberMachine > 1 then
		numberMachine = numberMachine - _GetAddinMachine()
		produceDiskWork.numberMachine = numberMachine
	end
	
	_UpdateLabels()
end

local function _SetAntipiratForce()

end

local function _AddLabels()
	labelGameName, _ = _AddLabel( "GameName", 270, 60, "400+", "40+", wndDPP )
	--добавим метку цены одного диска
	labelDiskPrice, _ =  _AddLabel( "???", 340, 330, "140+", "40+", wndDPP )
	
	--добавим метку общей цены за производство
	labelDiskNumber, _ = _AddLabel( "???", 330, 490, "140+", "30+", wndDPP )	
	labelFinalPrice, _ = _AddLabel( "Всего:", 330, 410, "140+", "30+", wndDPP )
	
	--добавим кнопки изменения количества аппаратов для производства дисков
	labelNumberMachine, _ = _AddLabel( "???", 300, 230, "30+", "40+", wndDPP )
	local btn = guienv:AddButton( 260, 220, "40+", "30+", wndDPP, -1, "+" )
	btn.action = _IncMachineNumber
	
	btn = guienv:AddButton( 260, 250, "40+", "30+", wndDPP, -1, "-" )
	btn.action = _DecMachineNumber
	
	--добавим кнопки изменения количества дней производства
	labelNumberDay, _ = _AddLabel( "numberday", 275, 180, "120+", "30+", wndDPP )
	btn = guienv:AddButton( 410, 160, "32+", "32+", wndDPP, -1, "+" )
	btn.action = _IncDayNumber
	btn = guienv:AddButton( 410, 192, "32+", "32+", wndDPP, -1, "-" )
	btn.action = _DecDayNumber
	
	btn = guienv:AddButton( 480, 220, "190+", "64+", wndDPP, -1, "Pirates" )
	btn.action = _SetAntipiratForce
end

local function _AddWork()	
	--изменим отношение производителя к игроку за размещение заказа
	localChangeProducerDiscount()
	
	company:AddBalance( -_GetFinalPrice() )
	plant:AddProduceWork( produceDiskWork )
	
	produceDiskWork:Remove()
	wndDPP:Remove()
	wndDPP = nil
end

local function _Hide()

end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	company = base.applic.playerCompany
	plant = base.applic.plant
	produceDiskWork = base.CLuaPlantWork():Create( company.name )
	
	wndDPP = window.fsWindow( "media/textures/MachinePlant.png", _Hide )
	
	--игры, которые можно производить
	_AddGames()
	
	--добавим список аппаратов
	_AddMachines()	
	
	_AddLabels()
	
	local btn = button.Stretch( 700, 385, "226+", "275+", "button_start", wndDPP, -1, "", QueryAddWork )
	btn:SetHoveredImage( 0, 0, 0, 0, "media/buttons/button_start_next.png" )
end

function CloseQueryAddWork()
	windowQueryWork:Remove()
end

function QueryAddWork()
	local machines = produceDiskWork.numberMachine
	windowQueryWork = guienv:AddWindow( "", "25%", 0, "50%+", "100%", -1, wndDPP )
	local listBox = guienv:AddListBox( "2%", "2%", "98%", "80%", -1, windowQueryWork )
	
	listBox:AddItem( "Название:"..currentGame.name, nil )
	listBox:AddItem( base.string.format( "Цена копии: %.2f", produceDiskWork.diskPrice ), nil )	
	listBox:AddItem( produceDiskWork:GetNumberDisk().." шт", nil )	
	listBox:AddItem( base.string.format( "$%d", _GetFinalPrice() ), nil )
	listBox:AddItem( "Стоимость аренды: $"..produceDiskWork.rentPrice * machines, nil)
	listBox:AddItem( "Производительность (шт\час):"..produceDiskWork.hourPerfomance * machines, nil ) 
	listBox:AddItem( "Стоимость работы ($\час):"..produceDiskWork.hourPrice * machines, nil )
	listBox:AddItem( base.string.format( "Цены дополнительных вещей: $ %.2f", produceDiskWork.advPrice ), nil )
	listBox:AddItem( "Всего дисков за день:"..produceDiskWork.diskInDay.." шт", nil )			
	listBox:AddItem( "Стоимость работы:"..produceDiskWork.hourPrice.." ($/час)", nil )
	listBox:AddItem( "Производительность:"..produceDiskWork.hourPerfomance.." (шт/час)", nil ) 
	listBox:AddItem( "Плата за размещение: $"..produceDiskWork.rentPrice, nil )
	listBox:AddItem( "Скидка производителя: "..base.string.format( "%d $", localGetFinalDiscount() * 100 ), nil )
	listBox:AddItem( "Линий производства: "..produceDiskWork.numberMachine, nil )
	listBox:AddItem( "Дней производства: "..produceDiskWork.numberDay.." (дн)", nil )
	
	local dd = base.string.format( "Оплатить %d", _GetFinalPrice() )
	button.Stretch( "25%", "82%", "24%+", "98%", "", windowQueryWork, -1, dd, _AddWork )
	button.Stretch( "51%", "82%", "24%+", "98%", "", windowQueryWork, -1, "Закрыть", _CloseQueryAddWork )
end

function localChangeProducerDiscount()
--[[	local lastOrder = produceDiskWork:GetNumberDisk()
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
	for i=1, plant:machineNumber do
		local dm = plant:GetDiskMachine( i-1 )
		if dm:GetName() ~= currentDiskMachine:GetName() then
			dm:SetDiscount( dm:GetDiscount() - 0.05 )		
		end		
	end
--]]
end
