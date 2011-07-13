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
		
		listBox:AddItem( "Название:"..plWork.game.name, nil )
		listBox:AddItem( base.string.format( "Цена копии: $%.2f", plWork.diskPrice ), nil )	
		listBox:AddItem( "Будет произведено "..plWork.numberDisk.." коробок", nil )
		listBox:AddItem( "В день производится "..plWork.diskInDay.." коробок", nil )		
		listBox:AddItem( "Производительность одной линии :"..plWork.hourPerfomance.." (шт/час)", nil ) 
		listBox:AddItem( "Общая производительность : "..plWork.hourPerfomance * machines .. " (шт/час)", nil ) 

		listBox:AddItem( "Стоимость запуска одной линии составит $"..plWork.rentPrice, nil )
		listBox:AddItem( "Стоимость запуска производства составит $"..plWork.rentPrice * machines, nil)
		listBox:AddItem( "Стоимость работы ($/час):"..plWork.hourPrice * machines, nil )
		listBox:AddItem( base.string.format( "Стоимость дополнительных вещей: $ %.2f", plWork.addonPrice ), nil )
			
		listBox:AddItem( "Стоимость работы:"..plWork.hourPrice.." ($/час)", nil )
		--listBox:AddItem( "Скидка производителя: "..base.string.format( "%d $", diskManager.GetFinalDiscount() * 100 ), nil )
		
		listBox:AddItem( "Линий производства: "..plWork.numberMachine, nil )
		listBox:AddItem( "Длительность производства: "..plWork.numberDay.." (дн)", nil )
		
		listBox:AddItem( base.string.format( "Полная стоимость $ %d", plWork.price ), nil )
		
		button.Stretch( "25%", "70e", "50%+", "40+", "", _infoWindow, -1, "Закрыть", button.CloseParent )
	else
		base.LogScript( "Plant work on index ".. elm.id .. " is null" )
		guienv:MessageBox( "Не могу создать пустой объект CLuaPlantWork. id="..elm.id, false, false, button.CloseParent, button.NoFunction ) 
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
