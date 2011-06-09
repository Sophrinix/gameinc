local base = _G

module( "reklameCampanies" )

local applic = base.applic
local guienv = base.guienv
local window = base.window

local _mainWindow = nil
local _lbxCamp = nil

local function _Hide()
	
end

function Show()
	company = applic.playerCompany
	plant = applic.plant

	local txsBlur = base.driver:CreateBlur( "marketing.png", 2, 4 )
	_mainWindow = window.fsWindow( txsBlur.path, _Hide )	
	
	local tmpWnd = guienv:AddWindow( "reklamesList.png", "12%", "12%", "791+", "412+", -1, _mainWindow )
	local lb = guienv:AddLabel( "Обзор запущенных рекламных кампаний", 24, 19, 770, 71, -1, tmpWnd )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	tmpWnd.closeButton.visible = false
	tmpWnd.draggable = false
	tmpWnd.drawBody = false
	_lbxCamp = guienv:AddListBox( 24, 74, 770, 390, -1, tmpWnd )
	_lbxCamp.itemHeight = 32
	
	for i=1, plant.campaniesNumber do
		local rcmp = plant:GetReklame( i-1 )
		
		local text = rcmp.objectType .. ": name=" .. rcmp.objectName .. " uniq=" .. rcmp.uniq .." company="..rcmp.company.." day="..rcmp.numberDay
		_lbxCamp:AddItem( text, nil )
	end
end
