local base = _G

module( "shareSelect" )

local guienv = base.guienv
local button = base.button
local rbank = nil
local company = nil
local shareCompany = nil

local mainWindow = nil
local edShare = nil
local edMoney = nil
local onEnd = nil
local shareStart = nil
local wantShareNumber = nil

local function _UpdateLabel()
	edShare.text = shareStart + wantShareNumber
	local deltaMoney = wantShareNumber * shareCompany.pieCost
	edMoney.text = deltaMoney
	
	if deltaMoney > 0 then 
		edMoney.overrideColor = base.toColor( 0xff, 0, 0xff, 0 )
	else
		edMoney.overrideColor = base.toColor( 0xff, 0xff, 0, 0 )
	end
end

local function _UpShare()
	local needMoney = ( wantShareNumber + 100 ) * shareCompany.pieCost
	if rbank:GetAvaibleShares( shareCompany ) > ( wantShareNumber + 100 ) and company.balance > needMoney then
		wantShareNumber = wantShareNumber + 100
		_UpdateLabel()
	end
end

local function _DownShare()
	if shareStart + wantShareNumber > 0 then
		wantShareNumber = wantShareNumber - 100
		_UpdateLabel()
	end
end

local function _OnEnd()
	mainWindow:Remove()
	mainWindow = nil
	onEnd( shareCompany, shareStart + wantShareNumber )
end

function Show( text, shrCmp, actionAfterEnd )
	rbank = base.applic.bank
	company = base.applic.playerCompany
	local shareCur = rbank:GetShares( company.name, currentCompany )

	shareStart = shareCur
	onEnd = actionAfterEnd
	shareCompany = shrCmp
	wantShareNumber = 0
	
	if mainWindow then
		mainWindow:Remove()
	end
	
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
