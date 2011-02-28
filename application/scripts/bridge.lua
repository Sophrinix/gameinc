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

local function _Hide()
end

function FillTableCompanies()
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

function Show()
	company = base.applic.playerCompany
	
	mainWindow = window.fsWindow( "media/maps/bank_select.png", _Hide )
	
	tblCompanies = guienv:AddTable( 10, 135, "10e", "10e", -1, mainWindow )
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
end
