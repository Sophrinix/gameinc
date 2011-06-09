local base = _G

module( "selectDiskGame" )

local guienv = base.guienv
local button = base.button
local window = base.window

local _company = nil
local _mainWindow = nil
local _cmbxGames = nil

local function _GameSelected( sender )
	local game = base.CLuaGame( _cmbxGames.selectedObject )
	base.LogScript( "user selected "..game.name )
	base.diskManager.currentGame = game
	base.diskManager.UpdateLabels()		
end

local function _SelectGame( sender )
	window.Hide( sender )	
end

local function _SelectGameClose( sender )
	base.diskManager.currentGame = nil
	window.Hide( sender )
end

function Show()
	_company = base.applic.playerCompany

	local txs = base.driver:CreateBlur( "plantrd.png", 2, 4 )
	_mainWindow = guienv:AddWindow( txs.path, 0, 0, "0e", "0e", -1, base.diskManager.wndDPP )
	_mainWindow.draggable = false
	_mainWindow.closeButton.visible = false
	
	_cmbxGames = guienv:AddPictureFlow( "5%", "5%", "95%", "180e", -1, _mainWindow )
	_cmbxGames:SetPictureRect( 0, 0, 140, 140 )
	_cmbxGames.drawBody = false
	_cmbxGames.onSelect = _GameSelected
	
	button.Stretch( "25%", "150e", "25%+", "60+", "", _mainWindow, -1, "Запомнить", _SelectGame )
	button.Stretch( "50%", "150e", "25%+", "60+", "", _mainWindow, -1, "Отменить", _SelectGameClose )
		
	for i=1, _company.gameNumber do
		local game = _company:GetGame( i-1 )
		
		if game.haveBox then 
			_cmbxGames:AddItem( game.viewImage, game.name, game.object )
		end	
	end
	
	_cmbxGames.selectedIndex = _cmbxGames.count - 1
end
