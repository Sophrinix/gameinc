local base = _G

module( "saleManager" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

local company = nil
local listboxGames = nil
local buttonAnonceGame = nil
local imageGamePreview = nil
local labelGameName = nil
local labelLastMonthSale = nil
local labelProfit = nil
local labelAllTimeSale = nil
local prgRating = nil
local btnDecreaseGamePrice = nil
local labelGamePrice = nil
local btnIncreaseGamePrice = nil
local listboxCompanyGame = nil
local anoncePictureFlow = nil
local windowAnonce = nil
local selectedGame = nil
local lastTimeParamsUpdate = base.os.clock()

local function CloseAnonceGame()
	windowAnonce:Remove()
end

function AnonceGame()
	windowAnonce = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", 
								     -1, guienv:GetRootGUIElement() ) 
	windowAnonce:GetCloseButton():SetVisible( false )
	
	anoncePictureFlow = guienv:AddPictureFlow( 10, 10, "10e", "50e", -1, windowAnonce:Self() )
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		
		if not game:IsSaling() then 
			anoncePictureFlow:AddItem( game:GetViewImage(), game:GetName(), game:Self() )
		end	
	end
	
	local btnOk = guienv:AddButton( "25%", "40e", "24%+", "10e", windowAnonce, -1, "Начать продажи" )
	btnOk:SetAction( "./saleManager.StartSaling()" )
	
	local btnCancel = guienv:AddButton( "51%", "40e", "24%+", "10e", windowAnonce, -1, "Выход" )
	btnCancel:SetAction( "./saleManager.CloseAnonceGame()" )
end

local function localUpdateCurrentGameParams()
	labelGameName:SetText( "Название: " .. selectedGame:GetName() )
	labelLastMonthSale:SetText( "Продаж за прошлый месяц:"..selectedGame:GetLastMonthSales() )
	labelProfit:SetText( "Прибыль:" .. selectedGame:GetAllTimeProfit() )
	labelAllTimeSale:SetText( "Продаж за все время:" .. selectedGame:GetAllTimeSales() )
	--prgRating:SetPos( selectedGame:GetCurrentQuality() ) 
		
	if selectedGame:GetCompany():Self() == company:Self() then
		btnDecreaseGamePrice:SetVisible( true )
		btnIncreaseGamePrice:SetVisible( true )		
		labelGamePrice:SetText( "Цена:" .. selectedGame:GetPrice() )
	else
		btnDecreaseGamePrice:SetVisible( false )
		btnIncreaseGamePrice:SetVisible( false )			
		
		local price = selectedGame:GetAllTimeProfit() / selectedGame:GetAllTimeSales()
		labelGamePrice:SetText( "Цена:" .. base.string.format( "%0.2f", price ) )
	end
end

function UpdateGameParams()
	if base.os.clock() - lastTimeParamsUpdate > 0 then
		
	end
end

function ListboxChanged()
    if base.NrpGetSender() == listboxGames:Self() then
		selectedGame = base.CLuaGame( listboxGames:GetSelectedObject() )
		localUpdateCurrentGameParams()
    end
end

local function localAddGames()
	listboxGames:Clear()
	
	local game = nil
	for i=1, applic:GetGamesNumber() do
		game = applic:GetGame( i-1 )
		if game:IsSaling() then
			listboxGames:AddItem( game:GetName(), game:Self() )
		end
	end
end

function Show()
	company = applic.playerCompany

	mainWindow = guienv:AddWindow( "media/textures/gameInSale.png", 
								   0, 0, scrWidth, scrHeight, 
								   -1, guienv:GetRootGUIElement() )
	mainWindow:SetDrawBody( false )
	mainWindow:GetCloseButton():SetVisible( false )
	mainWindow:SetDraggable( false )
	
	mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./saleManager.ListboxChanged()" )
	mainWindow:AddLuaFunction( base.GUIELEMENT_AFTER_DRAW, "./saleManager.UpdateGameParams()" )
	
	--добавим окно с листбоксом
	--в листбоксе поместим список игр, которые щас в продаже
	listboxGames = guienv:AddComponentListBox( 45, 320, 327, 590, -1, mainWindow )
	localAddGames()
	
	--расположим изображение игры справа от списка
	imageGamePreview = guienv:AddImage( 20, 20, 304, 204, mainWindow, -1, "" )
	
	--расположим под изображением основные параметры продаж
	--название игры
	local pos = { x=390, y=45 }
	local size = { w="560+", h="30+", ww=560, hh=30 }
 	labelGameName = guienv:AddLabel( "Название: ", pos.x, pos.y, size.w, size.h, -1, mainWindow )
 	pos.y = pos.y + size.hh
	
	--продаж за прошлый месяц
	labelLastMonthSale = guienv:AddLabel( "Продаж за прошлый месяц:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--продаж за текущий месяц
	labelProfit = guienv:AddLabel( "Продаж за этот месяц:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--всего продано копий
	labelAllTimeSale = guienv:AddLabel( "Продаж за все время:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--текущий рэйтинг игры
	prgRating = guienv:AddProgressBar( mainWindow, pos.x, pos.y, size.w, size.h, -1 )
	pos.y = pos.y + size.hh
	--prgRating:SetImage( )
	--prgRating:SetFillImage( )

	--цена игры с возможностью изменять цену
	btnDecreaseGamePrice = guienv:AddButton( pos.x, pos.y, size.h, size.h, mainWindow, -1, "-" )
	btnDecreaseGamePrice:SetAction( "./saleManager.DecreasePrice()" ) 
									
	labelGamePrice = guienv:AddLabel( "#TRANSLATE_TEXT_PRICE:", pos.x + size.hh, pos.y, 
																size.w, size.h, -1, mainWindow )
													 
	btnIncreaseGamePrice = guienv:AddButton( pos.x + size.ww - size.hh, pos.y, pos.x + size.ww, size.h, 
											 mainWindow, -1, "+" )
	btnIncreaseGamePrice:SetAction( "./saleManager.IncreasePrice()" ) 	
	
		--расположим кнопку "Анонсировать игру", по которой можно поместить игру на рынок
	buttonAnonceGame = guienv:AddButton( pos.x, 380, size.w, size.h, mainWindow, -1, "Анонсировать игру" )
	buttonAnonceGame:SetAction( "./saleManager.AnonceGame()" )
	
	--adding closeButton
	button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			"button_down", mainWindow, -1, "",
					"./saleManager.Hide()" )

end

function StartSaling()
	local game = anoncePictureFlow:GetSelectedObject()
	
	applic:AddGameToMarket( game )
	localAddGames()
	CloseAnonceGame()	
end

function DecreasePrice()
	selectedGame:SetPrice( selectedGame:GetPrice() - 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function IncreasePrice()
	selectedGame:SetPrice( selectedGame:GetPrice() + 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end