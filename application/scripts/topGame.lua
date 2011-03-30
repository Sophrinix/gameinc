local base = _G

module( "topGame" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

local company = nil
local tblGames = nil
local gamesChart = nil
local games = nil

local maxGameInChart = 20
local saveTypeTop = 2 

local SORT_PROFIT = 2
local SORT_SALE = 1

TIME_MONTH = 1
TIME_LIFE = 2

local function _SortByAllTime( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #games do
			if games[ i ].allTimeProfit > maxSort then
				maxSort = games[ i ].allTimeProfit
				index = i
			end
		end
	else
		for i=1, #games do
			if games[ i ].allTimeSales > maxSort then
				maxSort = games[ i ].allTimeSales
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
		for i=1, #games do
			if games[ i ].lastMonthProfit > maxSort then
				maxSort = games[ i ].lastMonthProfit
				index = i
			end
		end
	else
		for i=1, #games do
			if games[ i ].lastMonthSales > maxSort then
				maxSort = games[ i ].lastMonthSales
				index = i
			end
		end		
	end
	
	return index
end


local function _AddGamesSortBy( typeSort )
	base.LogScript( " sort by "..typeSort.."  time by "..saveTypeTop )
	tblGames:ClearRows()
	
	gamesChart = nil
	gamesChart = {}
	
	games = nil
	games = {}
	
	for i=1, applic.gamesNumber do
		base.table.insert( games, applic:GetGame( i-1 ) )
	end
	
	for pos=1, maxGameInChart do
		local index = 0

		if #games == 0 then break end
		
		if saveTypeTop == TIME_LIFE then
			index = _SortByAllTime( typeSort )
		else
			index = _SortByMonth( typeSort )
		end
		
		base.table.insert( gamesChart, games[ index ] )
		base.table.remove( games, index )
	end
		
	for i=1, #gamesChart do
		local game = gamesChart[ i ]
		if game == nil then return end
		
		local idx = tblGames:AddRow( tblGames.rowCount )
		tblGames:SetCellText( idx, 0, game.name, 0xff, 0xff, 0, 0 )
		if saveTypeTop == TIME_LIFE then
			tblGames:SetCellText( idx, 1, game.allTimeSales, 0xff, 0xff, 0, 0 )
			tblGames:SetCellText( idx, 2, game.allTimeProfit, 0xff, 0xff, 0, 0 )
		else
			tblGames:SetCellText( idx, 1, game.lastMonthSales, 0xff, 0xff, 0, 0 )
			tblGames:SetCellText( idx, 2, game.lastMonthProfit, 0xff, 0xff, 0, 0 )
		end
	
	end
end

local function _HeaderSelected()
	local activeColumn = tblGames.activeColumn
	
	if activeColumn == SORT_PROFIT then
		_AddGamesSortBy( SORT_PROFIT, saveTypeTop )
	else
		_AddGamesSortBy( SORT_SALE, saveTypeTop )
	end
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end

local function _CellSelected()

end

function Show( typeTop )
	saveTypeTop = typeTop
	company = applic.playerCompany

	local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, Hide )
	
	local img = guienv:AddImage( 0, 0, "0e", "0e", mainWindow, -1, "" );
	img.texture = "media/textures/chartsMonth.png"
	img.alphaChannel = true
	guienv:SendToBack( img )
		
	mainWindow:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, _CellSelected )
	mainWindow:AddLuaFunction( base.GUIELEMENT_TABLE_HEADER_SELECTED, _HeaderSelected )
	
	local lb = guienv:AddLabel( "", "33%", 20, "66%", 120, -1, mainWindow )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	lb.font = "font_28"
	lb.color = base.WHITE_COLOR
	
	if typeTop == TIME_MONTH then
		lb.text = "Чарт месяца"
	else
		lb.text = "Чарт всех времен"
	end
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblGames = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblGames.rowHeight = 24
	tblGames:AddColumn( "Название", -1 )
	tblGames:SetColumnWidth( 0, 282 )
	tblGames:AddColumn( "Продано копий", -1 )
	tblGames:SetColumnWidth( 1, 335 )
	tblGames:AddColumn( "Прибыль", -1 )
	tblGames:SetColumnWidth( 2, 230 )
			
	_AddGamesSortBy( SORT_PROFIT )
end
