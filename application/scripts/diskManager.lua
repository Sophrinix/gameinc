local base = _G

IncludeScript( "selectDiskMachine" )
IncludeScript( "selectDiskGame" )
IncludeScript( "diskManagerReport" )

module( "diskManager" )

local button = base.button
local tutorial = base.tutorial
local window = base.window
local guienv = base.guienv
local company = nil

local dayOfProduce = 0
local numberMachine = 0
local labelPricePrint = nil

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
local btnGame = nil
local btnMachine = nil
local btnStartWork = nil
local windowQueryWork = nil

local addons = { }

wndDPP = nil
currentGame = nil
currentDiskMachine = nil
produceDiskWork = nil

local function _AddLabel( text, xpos, ypos, ww, hh, parent )
	local lb = guienv:AddLabel( text, xpos, ypos, ww, hh, -1, parent )
	lb.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff )	
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

function GetFinalDiscount()
	if currentDiskMachine == nil then
		return 0
	end
	
	local discount = currentDiskMachine.discount + currentDiskMachine.lineDiscount * produceDiskWork.numberMachine
	if discount > currentDiskMachine.maxDiscount then
		discount = currentDiskMachine.maxDiscount
	end
	
	return discount
end

function GetFinalPrice()
	return produceDiskWork.price * ( 1 - GetFinalDiscount() )
end

function UpdateLabels()
	local machines = produceDiskWork.numberMachine
	
	if currentGame then
		labelGameName.text = currentGame.name
	else
		labelGameName.text = "Игра не выбрана"
	end
	
	labelNumberMachine.text = produceDiskWork.numberMachine
	labelNumberDay.text = produceDiskWork.numberDay.." (дн)"
	labelDiskNumber.text = produceDiskWork.numberDisk.." шт" 
	labelDiskPrice.text = base.string.format( "%.2f", produceDiskWork.diskPrice )
	labelFinalPrice.text = "$"..base.string.format( "%d", GetFinalPrice() )
	
	if currentGame ~= nil then
		btnGame:SetImage( 0, 0, 0, 0, currentGame.viewImage )
		produceDiskWork.game = currentGame.object
	end
	
	if currentDiskMachine ~= nil then
		btnMachine:SetImage( 0, 0, 0, 0, currentDiskMachine.texture )
		produceDiskWork.produceType = currentDiskMachine.object
	end
	
	btnStartWork.enabled = currentGame and currentDiskMachine and
						   produceDiskWork.numberMachine > 0 and produceDiskWork.numberDay > 0
end

local function _IncDayNumber()
	dayOfProduce = dayOfProduce + 1
	produceDiskWork.numberDay = dayOfProduce
	
	UpdateLabels()
end

local function _DecDayNumber()
	if dayOfProduce > 1 then
		dayOfProduce = dayOfProduce - 1
		produceDiskWork.numberDay = dayOfProduce
	end
	UpdateLabels()
end

local function _IncMachineNumber()
	numberMachine = numberMachine + _GetAddinMachine()
	produceDiskWork.numberMachine = numberMachine
	
	UpdateLabels()
end

local function _DecMachineNumber( ptr )
	if numberMachine > 1 then
		numberMachine = numberMachine - _GetAddinMachine()
		produceDiskWork.numberMachine = numberMachine
	end
	
	UpdateLabels()
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

local function _Hide()
	base.package.loaded[ "selectDiskGame" ] = false
	base.package.loaded[ "selectDiskMachine" ] = false
	base.package.loaded[ "diskManagerReport" ] = false
end

function Show()
	numberMachine = 0
	dayOfProduce = 0
	currentDiskMachine = nil
	currentGame = nil
	company = base.applic.playerCompany
	produceDiskWork = base.CLuaPlantWork():Create( company.name )
	
	wndDPP = window.fsWindow( "plantrd.png", _Hide )
	
	--игры, которые можно производить
	btnGame = button.Stretch( 100, 400, "150+", "230+", "plantNoGame", wndDPP, -1, "Выбрать\n игру", base.selectDiskGame.Show )
	
	--добавим список аппаратов
	btnMachine = button.Stretch( 130, 170, 250, 265, "plantNoMachine", wndDPP, -1, "Выбрать\n производство", base.selectDiskMachine.Show )
	
	_AddLabels()
	
	btnStartWork = button.Stretch( 700, 385, "226+", "275+", "button_start", wndDPP, -1, "", base.diskManagerReport.Show )
	btnStartWork:SetHoveredImage( 0, 0, 0, 0, "media/buttons/button_start_next.png" )
	btnStartWork.enabled = false
end

function ChangeProducerDiscount()
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
