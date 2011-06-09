local base = _G

module( "selectDiskMachine" )

local guienv = base.guienv
local button = base.button
local window = base.window

local _mainWindow = nil
local _cmbxProduceType = nil

local function _SelectMachine( sender )
	window.Hide( sender )
end

local function _SelectMachineClose()
	base.diskManager.currentDiskMachine = nil
	window.Hide( sender )
end

local function _MachineSelected()
	local dm = base.CLuaDiskMachine( _cmbxProduceType.selectedObject )
	
	base.LogScript( "selected diskMachine "..dm.name )
	base.diskManager.currentDiskMachine = dm
	base.diskManager.UpdateLabels()
end

function Show()
	local txs = base.driver:CreateBlur( "plantrd.png", 2, 4 )
	_mainWindow = guienv:AddWindow( txs.path, 0, 0, "0e", "0e", -1, base.diskManager.wndDPP )
	_mainWindow.closeButton.visible = false
	_mainWindow.draggable = false
	_mainWindow.drawBody = false
	
	_cmbxProduceType = guienv:AddPictureFlow( "5%", "5%", "95%", "180e", -1, _mainWindow )
	_cmbxProduceType:SetPictureRect( 0, 0, 140, 140 )
	_cmbxProduceType.drawBody = false
	_cmbxProduceType.onSelect = _MachineSelected
	
	button.Stretch( "25%", "150e", "25%+", "60+", "", _mainWindow, -1, "Запомнить", _SelectMachine )
	button.Stretch( "50%", "150e", "25%+", "60+", "", _mainWindow, -1, "Отменить", _SelectMachineClose )
	
	local plant = base.applic.plant
	for i=1, plant.machineNumber do
		local dm = plant:GetDiskMachine( i-1 )
		_cmbxProduceType:AddItem( dm.texture, dm.name, dm.object )		
	end
	
	_cmbxProduceType.selectedIndex = _cmbxProduceType.count - 1
end
