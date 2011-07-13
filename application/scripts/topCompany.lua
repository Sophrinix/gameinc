local base = _G

module( "topCompany" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

local company = nil
local tblCompanies = nil
local companiesChart = nil
local companies = nil

local maxCompanyInChart = 10
local saveTypeTop = 2 

local SORT_PROFIT = 2
local SORT_SALE = 1

TIME_MONTH = 1
TIME_LIFE = 2

local function _SortByAllTime( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #companies do
			if companies[ i ].profitLastYear > maxSort then
				maxSort = companies[ i ].profitLastYear
				index = i
			end
		end
	else
		for i=1, #companies do
			if companies[ i ].allTimeSales > maxSort then
				maxSort = companies[ i ].allTimeSales
				index = i
			end
		end		
	end
	
	return index
end

local function _SortByMonth( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #companies do
			if companies[ i ].lastMonthProfit > maxSort then
				maxSort = companies[ i ].lastMonthProfit
				index = i
			end
		end
	else
		for i=1, #companies do
			if companies[ i ].lastMonthSales > maxSort then
				maxSort = companies[ i ].lastMonthSales
				index = i
			end
		end		
	end
	
	return index
end


local function _AddCompaniesSortBy( typeSort )
	base.LogScript( " sort by "..typeSort.."  time by "..saveTypeTop )
	tblCompanies:ClearRows()
	
	companiesChart = nil
	companiesChart = {}
	
	companies = nil
	companies = {}
	
	for i=1, applic.companyNumber do
		base.table.insert( companies, applic:GetCompany( i-1 ) )
	end
	
	for pos=1, maxCompanyInChart do
		local index = 0

		if #companies == 0 then break end
		
		if saveTypeTop == TIME_LIFE then
			index = _SortByAllTime( typeSort )
		else
			index = _SortByMonth( typeSort )
		end
		
		base.table.insert( companiesChart, companies[ index ] )
		base.table.remove( companies, index )
	end
		
	for i=1, #companiesChart do
		local company = companiesChart[ i ]
		if company == nil then return end
		
		local idx = tblCompanies:AddRow( tblCompanies.rowCount )
		tblCompanies:SetCellText( idx, 0, company.name, 0xff, 0xff, 0xff, 0xff )
		if saveTypeTop == TIME_LIFE then
			tblCompanies:SetCellText( idx, 1, company.allTimeSales, 0xff, 0xff, 0xff, 0xff )
			tblCompanies:SetCellText( idx, 2, company.profitLastYear, 0xff, 0xff, 0xff, 0xff )
		else
			tblCompanies:SetCellText( idx, 1, company.lastMonthSales, 0xff, 0xff, 0xff, 0xff )
			tblCompanies:SetCellText( idx, 2, company.lastMonthProfit, 0xff, 0xff, 0xff, 0xff )
		end
	
	end
end

local function _HeaderSelected()
	local activeColumn = tblCompanies.activeColumn
	
	if activeColumn == SORT_PROFIT then
		_AddCompaniesSortBy( SORT_PROFIT, saveTypeTop )
	else
		_AddCompaniesSortBy( SORT_SALE, saveTypeTop )
	end
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end

local function _CellSelected()

end

function ShowHelp()
	tutorial.Update( "bridge/main" )
end

function Show( typeTop )
	saveTypeTop = typeTop
	company = applic.playerCompany
	
	base.rightPanel.AddYesNo( "Хотите узнать больше об игровых чартах?", ShowHelp, button.CloseBlend )

	local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, Hide )
	
	local img = guienv:AddImage( 0, 0, "0e", "0e", mainWindow, -1, "" );
	img.texture = "chartsMonth.png"
	img.alphaChannel = true
	guienv:SendToBack( img )
		
	mainWindow:Bind( base.GUIELEMENT_SELECTED_AGAIN, _CellSelected )
	mainWindow:Bind( base.GUIELEMENT_TABLE_HEADER_SELECTED, _HeaderSelected )
	
	local lb = guienv:AddLabel( "", "33%", 20, "66%", 120, -1, mainWindow )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	lb.font = "font_28"
	lb.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff )
	
	if typeTop == TIME_MONTH then
		lb.text = "Рынок компаний за месяц"
		lb.font = "font_20"
	else
		lb.text = "Топ-лист компаний"
	end
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblCompanies = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblCompanies.rowHeight = 24
	tblCompanies:AddColumn( "Название", -1 )
	tblCompanies:SetColumnWidth( 0, 282 )
	tblCompanies:AddColumn( "Продано игр", -1 )
	tblCompanies:SetColumnWidth( 1, 335 )
	tblCompanies:AddColumn( "Прибыль", -1 )
	tblCompanies:SetColumnWidth( 2, 230 )
			
	_AddCompaniesSortBy( SORT_PROFIT )
	
	local layoutButtons =  guienv:AddLayout( 40, tblCompanies.bottom + 20, "40e", "60+", 4, -1, mainWindow )
	button.LayoutButton( "", layoutButtons, -1, "Игры", function() mainWindow:Remove(); base.topGame.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "Платформы", function() mainWindow:Remove(); base.topPlatform.Show( 2 ) end )
	local selfBtn = button.LayoutButton( "", layoutButtons, -1, "Компании", button.NoFunction )
	button.LayoutButton( "", layoutButtons, -1, "Жанры", function() mainWindow:Remove(); base.topGenres.Show() end )
	
	selfBtn.enabled = false
end
