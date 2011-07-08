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
		guienv:MessageBox( "Не хватает средств для размещения производства", false, false, button.CloseParent, button.NoFunction )
		return
	end
	--изменим отношение производителя к игроку за размещение заказа
	diskManager.ChangeProducerDiscount()
	
	diskManager.company:AddBalance( "Оплата производства", -price )
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
	
	listBox:AddItem( "Название:"..produceDiskWork.game.name, nil )
	listBox:AddItem( base.string.format( "Цена копии: $%.2f", produceDiskWork.diskPrice ), nil )	
	listBox:AddItem( "Будет произведено "..produceDiskWork.numberDisk.." коробок", nil )
	listBox:AddItem( "В день производится "..produceDiskWork.diskInDay.." коробок", nil )		
	listBox:AddItem( "Производительность одной линии :"..produceDiskWork.hourPerfomance.." (шт/час)", nil ) 
	listBox:AddItem( "Общая производительность : "..produceDiskWork.hourPerfomance * machines .. " (шт/час)", nil ) 

	listBox:AddItem( "Стоимость запуска одной линии составит $"..produceDiskWork.rentPrice, nil )
	listBox:AddItem( "Стоимость запуска производства составит $"..produceDiskWork.rentPrice * machines, nil)
	listBox:AddItem( "Стоимость работы ($/час):"..produceDiskWork.hourPrice * machines, nil )
	listBox:AddItem( base.string.format( "Стоимость дополнительных вещей: $ %.2f", produceDiskWork.addonPrice ), nil )
		
	listBox:AddItem( "Стоимость работы:"..produceDiskWork.hourPrice.." ($/час)", nil )
	listBox:AddItem( "Скидка производителя: "..base.string.format( "%d $", diskManager.GetFinalDiscount() * 100 ), nil )
	
	listBox:AddItem( "Линий производства: "..produceDiskWork.numberMachine, nil )
	listBox:AddItem( "Длительность производства: "..produceDiskWork.numberDay.." (дн)", nil )
	
	listBox:AddItem( base.string.format( "Полная стоимость $ %d", price ), nil )
	
	button.Stretch( "12%", "70e", "49%", "40+", "", _mainWindow, -1, base.string.format( "Оплатить $ %d", price ), _AddWork )
	button.Stretch( "51%", "70e", "88%", "40+", "", _mainWindow, -1, "Отказаться", button.CloseParent )
end
