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

local lbComission = nil
local _BridgeComissionText = "Комиссия биржи 2%:"

local function _UpdateLabel()
	edShare.text = shareStart + wantShareNumber
	local deltaMoney = wantShareNumber * shareCompany.pieCost
	edMoney.text = base.string.format( "%d", -deltaMoney )
	
	local comission = base.string.format( "%d", base.math.ceil( deltaMoney * 0.02 ) );
	lbComission.text = _BridgeComissionText.."\nВы заплатите $"..comission.." за оформление сделки"; 
	
	if deltaMoney > 0 then 
		edMoney.overrideColor = base.NrpARGB( 0xff, 0xff, 0, 0 )
	else
		edMoney.overrideColor = base.NrpARGB( 0xff, 0, 0xff, 0 )
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
	onEnd( shareCompany, wantShareNumber )
end

function Show( text, shrCmp, actionAfterEnd )
	rbank = base.applic.bank
	company = base.applic.playerCompany
	local shareCur = rbank:GetShares( company.name, shrCmp )

	shareStart = shareCur
	onEnd = actionAfterEnd
	shareCompany = shrCmp
	wantShareNumber = 0
	
	if mainWindow then
		mainWindow:Remove()
	end
	
	mainWindow = guienv:AddWindow( "money_select.png", "33%", "33%", "284+", "142+", -1, guienv.root )
	mainWindow.closeButton.visible = false
	
	guienv:AddLabel( text, 80, 45, "195+", "55+", -1, mainWindow )
	
	edShare = guienv:AddEdit( shareStart,  58, 10, "166+", "25+", -1, mainWindow )
	edShare.drawBody = false
	edMoney = guienv:AddEdit( base.string.format( "%.03f", shareStart * shrCmp.pieCost ),  
							  66, 110, "110+", "25+", -1, mainWindow )
	edMoney.drawBody = false
	
	button.EqualeTexture( 182, 110, "money_select_minus", mainWindow, -1, "", _DownShare )
	button.EqualeTexture( 212, 110, "money_select_plus", mainWindow, -1, "", _UpShare )
	button.Stretch( "30e", "30e", "0e", "0e", "button_ok", mainWindow, -1, "",	_OnEnd )
	
	lbComission = guienv:AddLabel( _BridgeComissionText, edShare.left, edShare.bottom, 
								   "5e", edMoney.top, -1, mainWindow )
	lbComission:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	lbComission.font = "font_12" 
end
