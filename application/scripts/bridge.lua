local base = _G

IncludeScript( "shareSelect" )

module( "bridge" )

local button = base.button
local tutorial = base.tutorial
local guienv = base.guienv
local applic = base.applic
local window = base.window
local rbank = nil
local company = nil
local mainWindow = nil
local currentCompany = nil

local tblCompanies = nil
local companyLogo = nil
local labelAcNumber = nil
local labelAllAc = nil
local labelSelfAc = nil
local labelPrice = nil
local btnChangeShare = nil
local btnTryControl = nil

local function _Hide()
	base.package.loaded[ "shareSelect" ] = false
end

local function _FillTableCompanies()
	tblCompanies:ClearRows()

	for i=1, applic.companyNumber do
		local cmp = applic:GetCompany( i-1 )
		
		if cmp.allPie > 0 then
			local idx = tblCompanies:AddRow( tblCompanies.rowCount )
			tblCompanies:SetCellText( idx, 0, cmp.name, 0xff, 0xff, 0, 0 )
			tblCompanies:SetCellText( idx, 1, cmp.profitLastYear, 0xff, 0xff, 0, 0 )
			tblCompanies:SetCellText( idx, 2, base.string.format( "%.3f", cmp.pieCost ), 0xff, 0xff, 0, 0 )
			tblCompanies:SetCellText( idx, 3, (cmp.dividend * 100).."%", 0xff, 0xff, 0, 0 )
			tblCompanies:SetCellText( idx, 4, rbank:GetPieCostDynamic( cmp ), 0xff, 0xff, 0, 0 )

			local prc = rbank:GetShares( company.name, cmp ) / cmp.allPie * 100	
			local selfPie = cmp.selfPie / cmp.allPie * 100
			
			tblCompanies:SetCellText( idx, 5, base.string.format( "%d%% (%d%%)", prc, selfPie ), 0xff, 0xff, 0, 0 )
		end
	end
end

local function _UpdateInfo()
	base.LogScript( "company logo to "..currentCompany.name.." from "..currentCompany.texture )
	companyLogo.texture = currentCompany.texture
	
	local sharesNumber = rbank:GetShares( company.name, currentCompany )

	labelAcNumber.text = "Куплено: "..sharesNumber
	labelAllAc.text = "Всего: "..currentCompany.allPie
	labelPrice.text = "Цена: $" .. base.string.format( "%.3f", currentCompany.pieCost )
	labelSelfAc.text = "Доступно: ".. rbank:GetAvaibleShares( currentCompany )
	
	--если есть возможность взять контроль над компанией 
	btnTryControl.enabled = ( sharesNumber / currentCompany.allPie ) > 0.15
end

local function _CellSelected()
	local selRow = tblCompanies.activeRow
	
	local cmpName = tblCompanies:GetCellText( selRow, 0 )
	currentCompany = applic:GetCompanyByName( cmpName )
	
	_UpdateInfo()	
end

local function _EndingChangeShare( shareCompany, newShare )
	rbank:ChangeShares( company.name, shareCompany, newShare )
	_FillTableCompanies()
	_UpdateInfo()
end

local function _ChangeShare()
	if currentCompany == nil then
		return
	end
	
	local shareCur = rbank:GetShares( company.name, currentCompany )
	base.shareSelect.Show( "", currentCompany, _EndingChangeShare )
end

local function _EndingTryControl()

end

local function _TryControl()
	local shareCur = rbank:GetShares( company.name, currentCompany )
	base.companyControl.Show( "", currentCompany, _EndingTryControl )
end

function Show()
	rbank = base.applic.bank
	company = base.applic.playerCompany
	
	local txsBlur = base.driver:CreateBlur( "bridge.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, _Hide )
	
	tblCompanies = guienv:AddTable( "5%", "10%", "74%", "95%", -1, mainWindow )
	tblCompanies.rowHeight = 24
	tblCompanies:AddColumn( "Название", -1 )
	tblCompanies:SetColumnWidth( 0, 100 )
	tblCompanies:AddColumn( "Прибыль", -1 )
	tblCompanies:SetColumnWidth( 1, 100 )
	tblCompanies:AddColumn( "Стоимость акции", -1 )
	tblCompanies:SetColumnWidth( 2, 100 )
	tblCompanies:AddColumn( "Дивиденды", -1 )
	tblCompanies:SetColumnWidth( 3, 100 )
	tblCompanies:AddColumn( "Изменение", -1 )
	tblCompanies:SetColumnWidth( 4, 100 )
	tblCompanies:AddColumn( "Доля", -1 )
	tblCompanies:SetColumnWidth( 5, 100 )
	
	local ww = mainWindow.width * 0.2
	companyLogo = guienv:AddImage( "75%", "5%", ww.."+", ww.."+", mainWindow, -1, "" )	
	companyLogo.scale = true
	companyLogo.alphaChannel = true
	
	labelAcNumber = guienv:AddLabel( "Куплено: ", "76%", companyLogo.bottom, "10e", "20+", -1, mainWindow )
	labelAllAc = guienv:AddLabel( "Всего: ", "76%", labelAcNumber.bottom, "10e", "20+", -1, mainWindow )
	labelSelfAc = guienv:AddLabel( "Доступно: ", "76%", labelAllAc.bottom, "10e", "20+", -1, mainWindow )
	labelPrice = guienv:AddLabel( "Цена: ", "76%", labelSelfAc.bottom, "10e", "20+", -1, mainWindow )
	
	btnChangeShare = button.Stretch( "76%", labelPrice.bottom + 10, "95%", "30+", "changeShare", mainWindow, -1, "Изменить", _ChangeShare )
	btnTryControl = button.Stretch( "76%", btnChangeShare.bottom + 10, "95%", "30+", "tryControl", mainWindow, -1, "Слияние", _TryControl )
	
	mainWindow:Bind( base.GUIELEMENT_TABLE_CHANGED, _CellSelected )
	
	_FillTableCompanies()
end