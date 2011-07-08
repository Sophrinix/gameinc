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

local maxGameInChart = 10
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
		tblGames:SetCellText( idx, 0, game.name, 0xff, 0xff, 0xff, 0xff )
		if saveTypeTop == TIME_LIFE then
			tblGames:SetCellText( idx, 1, game.allTimeSales, 0xff, 0xff, 0xff, 0xff )
			tblGames:SetCellText( idx, 2, game.allTimeProfit, 0xff, 0xff, 0xff, 0xff )
		else
			tblGames:SetCellText( idx, 1, game.lastMonthSales, 0xff, 0xff, 0xff, 0xff )
			tblGames:SetCellText( idx, 2, game.lastMonthProfit, 0xff, 0xff, 0xff, 0xff )
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

function ShowHelp()
	tutorial.Update( "shop/chartGames" )
end

function Show( typeTop )
	saveTypeTop = typeTop
	company = applic.playerCompany
	
	base.rightPanel.AddYesNo( "������ ������ ������ �� ������� ������?", ShowHelp, button.CloseBlend )

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
	lb.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff  )
	
	if typeTop == TIME_MONTH then
		lb.text = "���� ������"
	else
		lb.text = "���� ���� ������"
	end
	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	tblGames = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblGames.rowHeight = 24
	tblGames:AddColumn( "��������", -1 )
	tblGames:SetColumnWidth( 0, 282 )
	tblGames:AddColumn( "������� �����", -1 )
	tblGames:SetColumnWidth( 1, 335 )
	tblGames:AddColumn( "�������", -1 )
	tblGames:SetColumnWidth( 2, 230 )
			
	_AddGamesSortBy( SORT_PROFIT )
	
	local layoutButtons = guienv:AddLayout( 40, tblGames.bottom + 20, "40e", "60+", 4, -1, mainWindow ) 	
	local selfBtn = button.LayoutButton( "", layoutButtons, -1, "����", button.NoFunction )
	button.LayoutButton( "", layoutButtons, -1, "���������", function() mainWindow:Remove(); base.topPlatform.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "��������", function() mainWindow:Remove(); base.topCompany.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "�����", function() mainWindow:Remove(); base.topGenres.Show() end )
	
	selfBtn.enabled = false
end
