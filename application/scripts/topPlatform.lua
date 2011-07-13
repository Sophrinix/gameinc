local base = _G

module( "topPlatform" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

local company = nil
local tblPls = nil
local plsChart = nil
local pls = nil

local maxPlsInChart = 12
local saveTypeTop = 2 

local SORT_PROFIT = 2
local SORT_SALE = 1

TIME_MONTH = 1
TIME_LIFE = 2

local function _SortByAllTime( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #pls do
			if pls[ i ].allTimeProfit > maxSort then
				maxSort = pls[ i ].allTimeProfit
				index = i
			end
		end
	else
		for i=1, #pls do
			if pls[ i ].allTimeSales > maxSort then
				maxSort = pls[ i ].allTimeSales
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
		for i=1, #pls do
			if pls[ i ].lastMonthProfit > maxSort then
				maxSort = pls[ i ].lastMonthProfit
				index = i
			end
		end
	else
		for i=1, #pls do
			if pls[ i ].lastMonthSales > maxSort then
				maxSort = pls[ i ].lastMonthSales
				index = i
			end
		end		
	end
	
	return index
end


local function _AddPlsSortBy( typeSort )
	base.LogScript( " sort platform by "..typeSort.."  time by "..saveTypeTop )
	tblPls:ClearRows()
	
	plsChart = nil
	plsChart = {}
	
	pls = nil
	pls = {}
	
	for i=1, applic.platformNumber do
		base.table.insert( pls, applic:GetPlatform( i-1 ) )
	end
	
	for pos=1, maxPlsInChart do
		local index = 0

		if #pls == 0 then break end
		
		if saveTypeTop == TIME_LIFE then
			index = _SortByAllTime( typeSort )
		else
			index = _SortByMonth( typeSort )
		end
		
		base.LogScript( "Added platform " .. pls[ index ].name .. " to chart" )
		base.table.insert( plsChart, pls[ index ] )
		base.table.remove( pls, index )
	end
		
	for i=1, #plsChart do
		local platform = plsChart[ i ]
		if platform == nil then return end
		
		local idx = tblPls:AddRow( tblPls.rowCount )
		tblPls:SetCellText( idx, 0, platform.name, 0xff, 0xff, 0xff, 0xff )
		if saveTypeTop == TIME_LIFE then
			tblPls:SetCellText( idx, 1, platform.allTimeSales, 0xff, 0xff, 0xff, 0xff )
			tblPls:SetCellText( idx, 2, platform.allTimeProfit, 0xff, 0xff, 0xff, 0xff )
		else
			tblPls:SetCellText( idx, 1, platform.lastMonthSales, 0xff, 0xff, 0xff, 0xff )
			tblPls:SetCellText( idx, 2, platform.lastMonthProfit, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

local function _HeaderSelected()
	local activeColumn = tblPls.activeColumn
	
	if activeColumn == SORT_PROFIT then
		_AddPlsSortBy( SORT_PROFIT, saveTypeTop )
	else
		_AddPlsSortBy( SORT_SALE, saveTypeTop )
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
	tutorial.Update( "shop/chartPlatforms" )
end

function Show( typeTop )
	saveTypeTop = typeTop
	company = applic.playerCompany
	
	base.rightPanel.AddYesNo( "Хотите узнать больше о чартах платформ?", ShowHelp, button.CloseBlend )

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
		lb.text = "Продажи игровых платформ за месяц"
		lb.font = "font_20"
	else
		lb.text = "Продажи игровых платформ за все время"
		lb.font = "font_18"
	end
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblPls = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblPls.rowHeight = 24
	tblPls:AddColumn( "Название", -1 )
	tblPls:SetColumnWidth( 0, 282 )
	tblPls:AddColumn( "Продано копий", -1 )
	tblPls:SetColumnWidth( 1, 335 )
	tblPls:AddColumn( "Прибыль", -1 )
	tblPls:SetColumnWidth( 2, 230 )
			
	_AddPlsSortBy( SORT_PROFIT )
	
	local layoutButtons =  guienv:AddLayout( 40, tblPls.bottom + 20, "40e", "60+", 4, -1, mainWindow )
	button.LayoutButton( "", layoutButtons, -1, "Игры", function() mainWindow:Remove(); base.topGame.Show( 2 ) end )
	local selfBtn = button.LayoutButton( "", layoutButtons, -1, "Платформы", button.NoFunction )
	button.LayoutButton( "", layoutButtons, -1, "Компании",  function() mainWindow:Remove(); base.topCompany.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "Жанры", function() mainWindow:Remove(); base.topGenres.Show() end )
	
	selfBtn.enabled = false
end
