local base = _G

module( "shareSelect" )

local guienv = base.guienv
local button = base.button

local mainWindow = nil
local edShare = nil
local edMoney = nil
local onEnd = nil
local shareStart = nil
local shareEnd = nil

local function _UpShare()
	moneyEnd = moneyEnd + moneyDelta
	edFull.text = moneyEnd
	edDelta.text = moneyEnd - moneyStart
end

local function _DownShare()
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

function Show( text, shrCmp, actionAfterEnd )
	local shareCur = rbank.GetShares( company.name, currentCompany )
	
	shareEnd = shareCur
	shareStart = shareCur
	onEnd = actionAfterEnd
	
	mainWindow = guienv:AddWindow( "media/textures/money_select.png", "33%", "33%", "284+", "142+", -1, guienv.root )
	mainWindow.closeButton.visible = false
	
	guienv:AddLabel( text, 80, 45, "195+", "55+", -1, mainWindow )
	
	edShare = guienv:AddEdit( shareStart,  58, 10, "166+", "25+", -1, mainWindow )
	edShare.drawBody = false
	edMoney = guienv:AddEdit( shareStart * shrCmp.pieCost,  66, 110, "110+", "25+", -1, mainWindow )
	edMoney.drawBody = false
	
	button.EqualeTexture( 182, 110, "money_select_minus", mainWindow, -1, "", _DownShare )
	button.EqualeTexture( 212, 110, "money_select_plus", mainWindow, -1, "", _UpShare )
	button.Stretch( "30e", "30e", "0e", "0e", "button_ok", mainWindow, -1, "",	_OnEnd )
end
