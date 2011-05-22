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
local buttonAnonceGame = nil
local imageGamePreview = nil
local labelGameName = nil
local labelLastMonthSale = nil
local labelProfit = nil
local labelAllTimeSale = nil
local btnDecreaseGamePrice = nil
local labelGamePrice = nil
local btnIncreaseGamePrice = nil
local listboxCompanyGame = nil
local anoncePictureFlow = nil
local windowAnonce = nil
local selectedGame = nil

listboxGames = nil
prgRating = nil

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

local function _HaveGameNoSale()
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		
		if game and not game.inSale then 
			return true
		end	
	end	
	
	return false
end

local function _AnonceGame()
	windowAnonce = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, guienv.root ) 
	windowAnonce.closeButton.visible = false
	
	anoncePictureFlow = guienv:AddPictureFlow( 30, 30, "30e", "80e", -1, windowAnonce )
	anoncePictureFlow.drawBody = false
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		
		if not game.inSale then 
			anoncePictureFlow:AddItem( game.viewImage, game.name, game.object )
		end	
	end
	
	local btnOk = guienv:AddButton( "25%", "70e", "24%+", "30e", windowAnonce, -1, "������ �������" )
	btnOk.action = _StartSaling
	
	local btnCancel = guienv:AddButton( "51%", "70e", "24%+", "30e", windowAnonce, -1, "�����" )
	btnCancel.action = _CloseAnonceGame
end

local function _UpdateGameParams()
	if selectedGame then
		labelGameName.text = "��������: " .. selectedGame.name
		labelLastMonthSale.text = "������ �� ������� �����:"..selectedGame.lastMonthSales
		labelProfit.text = "�������:" .. selectedGame.allTimeProfit
		labelAllTimeSale.text = "������ �� ��� �����:" .. selectedGame.allTimeSales
		
		prgRating.position = selectedGame.rating 
		prgVideo.position = selectedGame.graphikRating
		prgSound.position = selectedGame.soundRating
		prgGameplay.position = selectedGame.gameplayRating
		prgBugs.position = selectedGame.bugsRating
			
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
	
	buttonAnonceGame.visible = _HaveGameNoSale()
end

local function _ListboxChanged()
	selectedGame = base.CLuaGame( listboxGames.selectedObject )
	
	if selectedGame then
		imageGamePreview.texture = selectedGame.viewImage
		imageGamePreview.scale = true
		
		_UpdateGameParams()
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
	mainWindow = nil
end

local function _AddProgressBar( text, left, top, right, bottom )
	local label = guienv:AddLabel( text, left, top, right, bottom, -1, mainWindow )
	local ret = guienv:AddProgressBar( mainWindow, label.right, top, right, bottom, -1 )
	ret.image = "media/progressbar/prgNoStars.png"
	ret.fillImage = "media/progressbar/prgStars.png"
	ret.alphaChannel = true
	return ret 
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
	local size = { w="560+", h="30+" }
 	labelGameName = guienv:AddLabel( "��������: ", pos.x, pos.y, size.w, size.h, -1, mainWindow )

	--������ �� ������� �����
	labelLastMonthSale = guienv:AddLabel( "������ �� ������� �����:", pos.x, labelGameName.bottom + 15, size.w, size.h, -1, mainWindow )
	--������ �� ������� �����
	labelProfit = guienv:AddLabel( "������ �� ���� �����:", pos.x, labelLastMonthSale.bottom + 15, size.w, size.h, -1, mainWindow )
	--����� ������� �����
	labelAllTimeSale = guienv:AddLabel( "������ �� ��� �����:", pos.x, labelProfit.bottom + 15, size.w, size.h, -1, mainWindow )
	
	--���� ���� � ������������ �������� ����								
	labelGamePrice = guienv:AddLabel( "#TRANSLATE_TEXT_PRICE:", pos.x, labelAllTimeSale.bottom + 15, 
																size.w, size.h, -1, mainWindow )
																
	btnDecreaseGamePrice = guienv:AddButton( pos.x, labelGamePrice.top, size.h, size.h, mainWindow, -1, "-" )
	btnDecreaseGamePrice.visible = false
	btnDecreaseGamePrice.action = _DecreasePrice
											 
	btnIncreaseGamePrice = guienv:AddButton( labelLastMonthSale.right - 30, labelGamePrice.top, size.h, size.h, 
											 mainWindow, -1, "+" )
	btnIncreaseGamePrice.action = _IncreasePrice
	btnIncreaseGamePrice.visible = false

	--������� ������� ����
	size.pw = "126+"
	size.ph = "24+"
	
	prgRating = _AddProgressBar( "����� �������", pos.x, labelGamePrice.bottom + 15, size.pw, size.ph )
	prgVideo = _AddProgressBar( "�����", pos.x, prgRating.bottom + 15, size.pw, size.ph )
	prgSound = _AddProgressBar( "����", prgVideo.right + 20, prgVideo.top, size.pw, size.ph )
	prgGameplay = _AddProgressBar( "����������", pos.x, prgSound.bottom + 15, size.pw, size.ph )
	prgBugs = _AddProgressBar( "��������", prgGameplay.right + 20, prgGameplay.top, size.pw, size.ph )
	
	--���������� ������ "������������ ����", �� ������� ����� ��������� ���� �� �����
	buttonAnonceGame = button.Stretch( "80%", "15%", "128+", "128+", "btnAnonceGame", mainWindow, -1, "", _AnonceGame )
	
	guienv:AddLoopTimer( 5000, _UpdateGameParams, mainWindow )
	
	tutorial.Update( "shop/saleManager" )
end