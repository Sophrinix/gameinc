local base = _G

module( "topGame" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
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

local function SortByAllTime( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #games do
			if games[ i ]:GetAllTimeProfit() > maxSort then
				maxSort = games[ i ]:GetAllTimeProfit()
				index = i
			end
		end
	else
		for i=1, #games do
			if games[ i ]:GetAllTimeSales() > maxSort then
				maxSort = games[ i ]:GetAllTimeSales()
				index = i
			end
		end		
	end
	
	return index
end

local function SortByMonth( typeSort )
	local maxSort = 0
	local index = 0
	
	if typeSort == SORT_PROFIT then
		for i=1, #games do
			if games[ i ]:GetLastMonthProfit() > maxSort then
				maxSort = games[ i ]:GetLastMonthProfit()
				index = i
			end
		end
	else
		for i=1, #games do
			if games[ i ]:GetLastMonthSales() > maxSort then
				maxSort = games[ i ]:GetLastMonthSales()
				index = i
			end
		end		
	end
	
	return index
end


local function localAddGamesSortBy( typeSort )
	base.LogScript( " sort by "..typeSort.."  time by "..saveTypeTop )
	tblGames:ClearRows()
	
	gamesChart = nil
	gamesChart = {}
	
	games = nil
	games = {}
	
	for i=1, applic:GetGamesNumber() do
		base.table.insert( games, applic:GetGame( i-1 ) )
	end
	
	for pos=1, maxGameInChart do
		local index = 0

		if #games == 0 then break end
		
		if saveTypeTop == TIME_LIFE then
			index = SortByAllTime( typeSort )
		else
			index = SortByMonth( typeSort )
		end
		
		base.table.insert( gamesChart, games[ index ] )
		base.table.remove( games, index )
	end
		
	for i=1, #gamesChart do
		local game = gamesChart[ i ]
		if game == nil then return end
		
		local idx = tblGames:AddRow( tblGames:GetRowCount() )
		tblGames:SetCellText( idx, 0, game:GetName(), 0xff, 0xff, 0, 0 )
		if saveTypeTop == TIME_LIFE then
			tblGames:SetCellText( idx, 1, game:GetAllTimeSales(), 0xff, 0xff, 0, 0 )
			tblGames:SetCellText( idx, 2, game:GetAllTimeProfit(), 0xff, 0xff, 0, 0 )
		else
			tblGames:SetCellText( idx, 1, game:GetLastMonthSales(), 0xff, 0xff, 0, 0 )
			tblGames:SetCellText( idx, 2, game:GetLastMonthProfit(), 0xff, 0xff, 0, 0 )
		end
	
	end
end

function HeaderSelected()
	local activeColumn = tblGames:GetActiveColumn()
	
	if activeColumn == SORT_PROFIT then
		localAddGamesSortBy( SORT_PROFIT, saveTypeTop )
	else
		localAddGamesSortBy( SORT_SALE, saveTypeTop )
	end
end

function Show( typeTop )
	saveTypeTop = typeTop
	company = applic.playerCompany

	mainWindow = guienv:AddWindow( "media/textures/chartsMonth.png", 
								   0, 0, scrWidth, scrHeight, 
								   -1, guienv:GetRootGUIElement() )
	mainWindow:SetDrawBody( false )
	mainWindow:GetCloseButton():SetVisible( false )
	mainWindow:SetDraggable( false )
	
	mainWindow:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, "./topGame.CellSelected()" )
	mainWindow:AddLuaFunction( base.GUIELEMENT_TABLE_HEADER_SELECTED, "./topGame.HeaderSelected()" )
	
	local lb = guienv:AddLabel( "", "33%", 20, "66%", 120, 
								-1, mainWindow )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	
	if typeTop == TIME_MONTH then
		lb:SetText( "Чарт месяца" )
	else
		lb:SetText( "Чарт всех времен" )	
	end
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblGames = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblGames:SetRowHeight( 24 )
	tblGames:AddColumn( "Название", -1 )
	tblGames:SetColumnWidth( 0, 282 )
	tblGames:AddColumn( "Продано копий", -1 )
	tblGames:SetColumnWidth( 1, 335 )
	tblGames:AddColumn( "Прибыль", -1 )
	tblGames:SetColumnWidth( 2, 230 )
	
	--adding closeButton
	button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			"button_down", mainWindow, -1, "",
					"./topGame.Hide()" )
					
	localAddGamesSortBy( SORT_PROFIT )
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end
