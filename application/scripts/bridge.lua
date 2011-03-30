local base = _G

module( "bridge" )

local button = base.button
local tutorial = base.tutorial
local guienv = base.guienv
local applic = base.applic
local window = base.window
local company = nil
local mainWindow = nil
local tblCompanies = nil
local companyLogo = nil
local currentCompany = nil

local function _Hide()
end

local function _FillTableCompanies()
	tblCompanies:ClearRows()

	for i=1, applic.companyNumber do
		local cmp = applic.GetCompany( i-1 )
		
		local idx = tblCompanies:AddRow( tblCompanies:GetRowCount() )
		tblCompanies:SetCellText( idx, 0, cmp.name, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 1, cmp.profitLastYear, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 2, cmp.pieCost, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 3, cmp.dividend, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 4, 0, 0xff, 0xff, 0, 0 )
	end
end

function _CellSelected()
	local selRow = tblCompanies.activeRow
	
	local cmpName = tblCompanies:GetCellText( selRow, 0 )
	currentCompany = base.applic:GetCompany( cmpName )
	
	companyLogo.texture = currentCompany.texture
	
	local prc = packet / currentCompany.allPie	
	--если есть возможность взять контроль над компанией 
	buttonTryControl.enabled = prc > 0.15
end

local function _BuyAc()
	
end

local function _SellAc()

end

local function _TryControl()

end

function Show()
	company = base.applic.playerCompany
	
	local txsBlur = base.driver:CreateBlur( "bank.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, _Hide )
	
	tblCompanies = guienv:AddTable( 10, 135, "74%", "10e", -1, mainWindow )
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
	tblCompanies:AddColumn( "Пакет", -1 )
	tblCompanies:SetColumnWidth( 5, 100 )
	
	local ww = mainWindow.width * 0.2
	companyLogo = guienv:AddImage( "75%", 135, ww.."+", ww.."+", guienv.root, -1, "" )	
	
	local packet = currentCompany:GetPiePacket( company.name )
	labelAcNumber = guienv:AddLabel( "Акций: "..packet, "76%", companyLogo.bottom, "10e", "20+", -1, mainWindow )
	labelAllAc = guienv:AddLabel( "Всего: "..currentCompany.allPie, "76%", labelAcNumber.bottom, "10e", "20+", mainWindow )
	
	local price = packet * currentCompany.pieCost
	labelPrice = guienv:AddLabel( "Цена: "..price, "76%", labelAllAc.bottom, "10e", "20+", mainWindow )
	
	buttonBuy = button.EqualeTexture( "76%", labelPrice.bottom + 10, "buyAc", mainWindow, -1, "", _BuyAc )
	buttonSell = button.EqualeTexture( "76%", buttonBuy.bottom + 10, "sellAc", mainWindow, -1, "", _SellAc )
	buttonTryControl = button.EqualTexture( "76%", buttonSell.bottom + 10, "tryControl", mainWindow, "", _TryControl )
	
	mainWindow:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, _CellSelected )
	
	_FillTableCompanies()
end