local base = _G

module( "monthTopGame" )

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

local function localAddGames()
	tblGames:Clear()
	
	local game = nil
	applic:RefreshAllTimeGamesChart()
	for i=1, 0xff do
		game = applic:GetAllTimerChartGame( i-1 )
		if game == nil then return end
		
		local idx = tbl:AddRow( tbl:GetRowCount() )
		tbl:SetCellText( idx, 0, i, 0xff, 0xff, 0, 0 )
		tbl:SetCellText( idx, 1, game:GetAllTimeSales(), 0xff, 0xff, 0, 0 )
		tbl:SetCellText( idx, 2, game:GetAllTimeProfit(), 0xff, 0xff, 0, 0 )
	end
end

function Show()
	company = applic:GetPlayerCompany()

	mainWindow = guienv:AddWindow( "media/textures/chartsMonth.png", 
								   0, 0, scrWidth, scrHeight, 
								   -1, guienv:GetRootGUIElement() )
	mainWindow:SetDrawBody( false )
	mainWindow:GetCloseButton():SetVisible( false )
	mainWindow:SetDraggable( false )
	
	mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./monthTopGame.ListboxChanged()" )
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	tblGames = guienv:AddComponentListBox( 90, 150, 935, 530, -1, mainWindow )
	tblGames:AddColumn( "Название", -1 )
	tblGames:SetColumnWidth( 0, 282 )
	tblGames:AddColumn( "Продано копий", -1 )
	tblGames:SetColumnWidth( 1, 335 )
	tblGames:AddColumn( "Прибыль", -1 )
	tblGames:SetColumnWidth( 2, 230 )
	localAddGames()
	
	--adding closeButton
	button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			"button_down", mainWindow, -1, "",
					"./saleManager.Hide()" )
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end
