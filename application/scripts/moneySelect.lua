local base = _G

module( "moneySelect" )

local guienv = base.guienv
local button = base.button

moneyStart = 0
moneyEnd = 0
local mainWindow = nil
local edFull = nil
local edDelta = nil
local moneyDelta = 0
local onEnd = nil

local function _UpMoney()
	moneyEnd = moneyEnd + moneyDelta
	edFull.text = moneyEnd
	edDelta.text = moneyEnd - moneyStart
end

local function _DownMoney()
	if moneyEnd > moneyDelta then
		moneyEnd = moneyEnd - moneyDelta
		edFull.text = moneyEnd
		edDelta.text = moneyEnd - moneyStart
	end
end

local function _OnEnd()
	mainWindow:Remove()
	onEnd( moneyStart, moneyEnd )
end

function Show( text, start, interval, actionAfterEnd )
	moneyDelta = interval
	moneyEnd = start
	moneyStart = start
	onEnd = actionAfterEnd
	mainWindow = guienv:AddWindow( "money_select.png", "33%", "33%", "284+", "142+", -1, guienv.root )
	mainWindow.closeButton.visible = false
	
	label = guienv:AddLabel( text, 80, 45, "195+", "55+", -1, mainWindow )
	
	edFull = guienv:AddEdit( start,  58, 10, "166+", "25+", -1, mainWindow )
	edFull.drawBody = false
	edDelta = guienv:AddEdit( 0,  66, 110, "110+", "25+", -1, mainWindow )
	edDelta.drawBody = false
	
	button.EqualeTexture( 182, 110, "money_select_minus", mainWindow, -1, "", _DownMoney )
	button.EqualeTexture( 212, 110, "money_select_plus", mainWindow, -1, "", _UpMoney )
	button.Stretch( "30e", "30e", "0e", "0e", "button_ok", mainWindow, -1, "",	_OnEnd )
end
