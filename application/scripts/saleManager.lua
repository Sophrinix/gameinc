local base = _G

module( "saleManager" )

local guienv = base.guienv

local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic
local window = base.window

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

local function _AddGames()
	listboxGames:Clear()
	
	local game = nil
	for i=1, applic.gamesNumber do
		game = applic:GetGame( i-1 )
		if game.inSale then
			listboxGames:AddItem( game.name, game.object )
		end
	end
end

local function _CloseAnonceGame()
	windowAnonce:Remove()
end

local function _StartSaling()
	applic:AddGameToMarket( anoncePictureFlow.selectedObject )
	_AddGames()
	_CloseAnonceGame()	
end


local function _AnonceGame()
	windowAnonce = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, guienv.root ) 
	windowAnonce.closeButton.visible = false
	
	anoncePictureFlow = guienv:AddPictureFlow( 10, 10, "10e", "50e", -1, windowAnonce )
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		
		if not game.inSale then 
			anoncePictureFlow:AddItem( game.viewImage, game.name, game.object )
		end	
	end
	
	local btnOk = guienv:AddButton( "25%", "40e", "24%+", "10e", windowAnonce, -1, "������ �������" )
	btnOk.action = _StartSaling
	
	local btnCancel = guienv:AddButton( "51%", "40e", "24%+", "10e", windowAnonce, -1, "�����" )
	btnCancel.action = _CloseAnonceGame
end

local function _UpdateGameParams()
	if selectedGame then
		labelGameName.text = "��������: " .. selectedGame.name
		labelLastMonthSale.text = "������ �� ������� �����:"..selectedGame.lastMonthSales
		labelProfit.text = "�������:" .. selectedGame.allTimeProfit
		labelAllTimeSale.text = "������ �� ��� �����:" .. selectedGame.allTimeSales
		--prgRating:SetPos( selectedGame:GetCurrentQuality() ) 
			
		if selectedGame.company.object == company.object then
			btnDecreaseGamePrice.visible = true
			btnIncreaseGamePrice.visible = true		
			labelGamePrice.text = "����:" .. selectedGame.price
		else
			btnDecreaseGamePrice.visible = false
			btnIncreaseGamePrice.visible = false 
			
			local price = selectedGame.allTimeProfit / selectedGame.allTimeSales
			labelGamePrice.text = "����:" .. base.string.format( "%0.2f", price )
		end
	end
end

local function _ListboxChanged()
	selectedGame = base.CLuaGame( listboxGames.selectedObject )
	
	if selectedGame then
		imageGamePreview.texture = selectedGame.viewImage
		imageGamePreview.scale = true
	end
end

local function _DecreasePrice()
	selectedGame.price = selectedGame.price - 1
	labelGamePrice.text = "#TRANSLATE_TEXT_PRICE:" .. selectedGame.price
end

local function _IncreasePrice()
	selectedGame.price = selectedGame.price + 1
	labelGamePrice.text = "#TRANSLATE_TEXT_PRICE:" .. selectedGame.price
end

function Hide()
	mainWindow:Remove()
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end

function Show()
	company = applic.playerCompany

	local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, Hide )
	
	local img = guienv:AddImage( 0, 0, "0e", "0e", mainWindow, -1, "" );
	img.texture = "media/textures/gameInSale.png"
	img.alphaChannel = true
	guienv:SendToBack( img )

	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	listboxGames = guienv:AddComponentListBox( 45, 320, 327, 590, -1, mainWindow )
	listboxGames.onChangeSelect = _ListboxChanged
	_AddGames()

	--���������� ����������� ���� ������ �� ������
	imageGamePreview = guienv:AddImage( 20, 20, 304, 204, mainWindow, -1, "" )
	
	--���������� ��� ������������ �������� ��������� ������
	--�������� ����
	local pos = { x=390, y=45 }
	local size = { w="560+", h="30+", ww=560, hh=30 }
 	labelGameName = guienv:AddLabel( "��������: ", pos.x, pos.y, size.w, size.h, -1, mainWindow )
 	pos.y = pos.y + size.hh
	
	--������ �� ������� �����
	labelLastMonthSale = guienv:AddLabel( "������ �� ������� �����:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--������ �� ������� �����
	labelProfit = guienv:AddLabel( "������ �� ���� �����:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--����� ������� �����
	labelAllTimeSale = guienv:AddLabel( "������ �� ��� �����:", pos.x, pos.y, size.w, size.h, -1, mainWindow )
	pos.y = pos.y + size.hh
	--������� ������� ����
	prgRating = guienv:AddProgressBar( mainWindow, pos.x, pos.y, size.w, size.h, -1 )
	pos.y = pos.y + size.hh
	--prgRating:SetImage( )
	--prgRating:SetFillImage( )

	--���� ���� � ������������ �������� ����
	btnDecreaseGamePrice = guienv:AddButton( pos.x, pos.y, size.h, size.h, mainWindow, -1, "-" )
	btnDecreaseGamePrice.action = _DecreasePrice
									
	labelGamePrice = guienv:AddLabel( "#TRANSLATE_TEXT_PRICE:", pos.x + size.hh, pos.y, 
																size.w, size.h, -1, mainWindow )
													 
	btnIncreaseGamePrice = guienv:AddButton( pos.x + size.ww - size.hh, pos.y, pos.x + size.ww, size.h, 
											 mainWindow, -1, "+" )
	btnIncreaseGamePrice.action = _IncreasePrice
	
	--���������� ������ "������������ ����", �� ������� ����� ��������� ���� �� �����
	buttonAnonceGame = guienv:AddButton( pos.x, 380, size.w, size.h, mainWindow, -1, "������������ ����" )
	buttonAnonceGame.action = _AnonceGame
	
	guienv:AddLoopTimer( 1000, _UpdateGameParams, mainWindow )
end