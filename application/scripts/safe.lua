local base = _G

module( "safe" )

local _mainWindow = nil
local _safeWindow = nil
local company = nil
local applic = base.applic
local button = base.button
local guienv = base.guienv
local window = base.window

local function _Hide()
	_mainWindow = nil
end

function Show()
	company = applic.playerCompany
	
	local txsBlur = base.driver:CreateBlur( "director_cabinet.png", 2, 4 )
	_mainWindow = window.fsWindow( txsBlur.path, _Hide )
	_safeWindow = guienv:AddWindow( "safe_open.png", "12%", "12%", "512+", "400+", -1, _mainWindow )
	_safeWindow.closeButton.visible = false
	_safeWindow.draggable = false
	_safeWindow.drawBody = false
end