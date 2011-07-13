local base = _G

module( "topGenres" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

local company = nil
local tblGenres = nil
local genresChart = nil
local genres = nil

local function _SortByAllTime( typeSort )
	local maxSort = 0
	local index = 0
	
	for i=1, #genres do
		if genres[ i ].interest > maxSort then
			maxSort = genres[ i ].interest
			index = i
		end
	end		
	
	return index
end

local function _GetTextByPercent( prc )
	if prc < 0.1 then
		return "Рынок переполнен этим жанром"
	elseif prc >= 0.1 and prc < 0.3 then
		return "Игр такого жанра очень много"
	elseif prc >= 0.3 and prc < 0.5 then
		return "Покупателям малоинтересен"
	elseif prc >= 0.5 and prc < 0.7 then
		return "Жанр востребован на рынке"
	elseif prc >= 0.7 and prc < 0.9 then
		return "Немного игр такого жанра"
	elseif prc >= 0.9 then
		return "Жанр почти неизвестен"
	end
end

local function _AddGenresSortBy()
	tblGenres:ClearRows()
	
	genresChart = nil
	genresChart = {}
	
	genres = nil
	genres = {}
	
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech.techGroup == base.PT_GENRE then
			base.table.insert( genres, tech )
		end
	end
	
	for pos=1, #genres do
		local index = _SortByAllTime()
		base.table.insert( genresChart, genres[ index ] )
		base.table.remove( genres, index )
	end
		
	for i=1, #genresChart do
		local genre = genresChart[ i ]
		if genre == nil then return end
		
		local idx = tblGenres:AddRow( tblGenres.rowCount )
		tblGenres:SetCellText( idx, 0, genre.name, 0xff, 0xff, 0xff, 0xff )

		tblGenres:SetCellText( idx, 1, _GetTextByPercent( genre.interest ), 0xff, 0xff, 0xff, 0xff )
		--tblGenres:SetCellText( idx, 2, game.lastMonthProfit, 0xff, 0xff, 0, 0 )
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
	tutorial.Update( "shop/chartGenres" )
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
	
	local lb = guienv:AddLabel( "", "33%", 20, "66%", 120, -1, mainWindow )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	lb.font = "font_28"
	lb.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff )
	
	lb.text = "Востребованность жанров"
	lb.font = "font_20"
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblGenres = guienv:AddTable( 90, 150, 935, 530, -1, mainWindow )
	tblGenres.rowHeight = 24
	tblGenres:AddColumn( "Название", -1 )
	tblGenres:SetColumnWidth( 0, 282 )
	tblGenres:AddColumn( "Интерес", -1 )
	tblGenres:SetColumnWidth( 1, 335 )
	tblGenres:AddColumn( "", -1 )
	tblGenres:SetColumnWidth( 2, 230 )
			
	_AddGenresSortBy( SORT_PROFIT )
	
	local layoutButtons = guienv:AddLayout( 40, tblGenres.bottom + 20, "40e", "60+", 4, -1, mainWindow ) 	
	button.LayoutButton( "", layoutButtons, -1, "Игры", function() mainWindow:Remove(); base.topGame.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "Платформы", function() mainWindow:Remove(); base.topPlatform.Show( 2 ) end )
	button.LayoutButton( "", layoutButtons, -1, "Компании", function() mainWindow:Remove(); base.topCompany.Show( 2 ) end  )
	local selfBtn = button.LayoutButton( "", layoutButtons, -1, "Жанры", button.NoFunction )
	
	selfBtn.enabled = false
end
