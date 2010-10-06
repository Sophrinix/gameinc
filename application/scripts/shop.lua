local base = _G

module( "shop" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local company = nil

local mainWindow = nil
local listboxGames = nil
local buttonAnonceGame = nil
local imageGamePreview = nil
local labelGameName = nil
local labelLastMonthSale = nil
local labelCurrentMonthSale = nil
local labelAllTimeSale = nil
local prgRating = nil
local btnDecreaseGamePrice = nil
local labelGamePrice = nil
local btnIncreaseGamePrice = nil
local listboxCompanyGame = nil
local anoncePictureFlow = nil
local windowAnonce = nil
local selectedGame = nil
local lastTimeParamsUpdate = base.GetTickCount()

function Hide()
	mainWindow:Remove()
	mainWindow = nil
end

function Show()  
    if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/windowShop_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./shop.Hide()" )
	end

	--���� � �������
	button.EqualeTexture( 147, 333, "gameInSale", mainWindow:Self(), -1, "", "./shop.ShowGamesMarket()")
	
	--���-���� ������
	button.EqualeTexture( 703, 67, "toplistmonth", mainWindow:Self(), -1, "", "./shop.ShowMonthTopList()" )

	--���-���� �� ��� �����
	button.EqualeTexture( 119, 94, "toplisttime", mainWindow:Self(), -1, "", "./shop.ShowAllTimeTopList()")

	--������� �������
	button.EqualeTexture( 861, 268, "showMagazines", mainWindow:Self(), -1, "", "./shop.ShowJournals()" )
end

local function localFillListboxGame()
	listboxGames:Clear()
	
	local game = nil
	for i=1, applic:GetGamesNumber() do
		game = applic:GetGame( i-1 )
		if game:IsSaling() then
			listboxGames:AddItem( game:GetName(), game:Self() )
		end
	end
end

local function localUpdateCurrentGameParams()
	labelGameName:SetText( "��������: " .. selectedGame:GetName() )
	labelLastMonthSale:SetText( "������ �� ������� �����:" .. selectedGame:GetLastMonthSales() )
	labelCurrentMonthSale:SetText( "������ �� ���� �����:" .. selectedGame:GetCurrentMonthSales() )
	labelAllTimeSale:SetText( "������ �� ��� �����:" .. selectedGame:GetAllTimeSales() )
	--prgRating:SetPos( selectedGame:GetCurrentQuality() ) 
	labelGamePrice:SetText( "����:" .. selectedGame:GetPrice() )
	
	if selectedGame:GetCompany() == company:Self() then
		btnDecreaseGamePrice:SetVisible( true )
		btnIncreaseGamePrice:SetVisible( true )		
	else
		btnDecreaseGamePrice:SetVisible( false )
		btnIncreaseGamePrice:SetVisible( false )			
	end
end

function GameInSaleWindowListboxChanged()
    if base.GetNrpSender() == listboxGames:Self() then
		selectedGame:SetObject( listboxGames:GetSelectedObject() )
		localUpdateCurrentGameParams()
    end
end

function UpdateGameParams()
	
end

function ShowGamesMarket()

end

function DecreaseGamePrice()
	selectedGame:SetPrice( selectedGame:GetPrice() - 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function IncreaseGamePrice()
	selectedGame:SetPrice( selectedGame:GetPrice() + 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function ShowGameInSale()
	company = applic:GetPlayerCompany()
	windowShop = guienv:AddWindow( "", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	
	windowShop:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./shop.ListboxChanged()" )
	windowShop:AddLuaFunction( base.GUIELEMENT_AFTER_DRAW, "./shop.UpdateGameParams()" )
	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	listboxGames:SetObject( guienv:AddListBox( 20, 20, scrWidth / 2 - 10, scrHeight - 20, -1, windowShop:Self() ) ) 
	localFillListboxGame()
	
	--���������� ������ "������������ ����", �� ������� ����� ��������� ���� �� �����
	buttonAnonceGame = guienv:AddButton( scrWidth / 2, scrHeight -50, scrWidth - 20, scrHeight - 20, 
								windowShop:Self(), -1, "������������ ����" )
	buttonAnonceGame:SetAction( "./shop.AnonceGame()" )
	
	--���������� ����������� ���� ������ �� ������
	imageGamePreview = guienv:AddImage( scrWidth / 2 + 20, 20, scrWidth - 20, scrHeight / 2 - 20, windowShop:Self(), -1, "" )
	
	--���������� ��� ������������ �������� ��������� ������
	--�������� ����
	local hw = scrWidth / 2 + 20
	local hh = scrHeight / 2 + 20
	labelGameName = guienv:AddLabel( "��������: ", hw + 20, hh + 20, scrWidth - 20, hh + 40, -1, windowShop:Self() )
	
	--������ �� ������� �����
	labelLastMonthSale = guienv:AddLabel( "������ �� ������� �����:", hw + 20, hh + 50, 
												   scrWidth - 20, hh + 70, -1, windowShop:Self() )
	--������ �� ������� �����
	labelCurrentMonthSale = guienv:AddLabel( "������ �� ���� �����:", hw + 20, hh + 80, 
													  scrWidth - 20, hh + 100, -1, windowShop:Self() )
	--����� ������� �����
	labelAllTimeSale = guienv:AddLabel( "������ �� ��� �����:", hw + 20, hh + 110, 
													  scrWidth - 20, hh + 130, -1, windowShop:Self() )
	--������� ������� ����
	prgRating = guienv:AddProgressBar( windowShop:Self(), hw + 20, hh + 140, 
													scrWidth - 20, hh + 160, -1 )
	--prgRating:SetImage( )
	--prgRating:SetFillImage( )

	--���� ���� � ������������ �������� ����
	btnDecreaseGamePrice = guienv:AddButton( hw + 20, hh + 170, hw + 40, hh + 190, 
									windowShop:Self(), -1, "-" )
	btnDecreaseGamePrice:SetAction( "sworkWindowShopDecreaseGamePrice" ) 
									
	labelGamePrice = guienv:AddLabel( "#TRANSLATE_TEXT_PRICE:", hw + 40, hh + 170, 
													    sctWidth - 40, hh + 190, -1, windowShop:Self() )
													 
	btnIncreaseGamePrice = guienv:AddButton( scrWidth - 40, hh + 170, scrWidth - 20, hh + 190, 
									windowShop:Self(), -1, "+" )
	btnIncreaseGamePrice:SetAction( "./shop.IncreaseGamePrice()" ) 	
end

function ShowJournals()

end

function AnonceGame()
	windowAnonce = guienv:AddWindow( "", scrWidth / 2 - 200, scrHeight / 2 - 120,
						  				 scrWidth / 2 + 200, scrHeight / 2 + 120, 
								         -1, windowShop:Self() ) 
	
	anoncePictureFlow = guienv:AddPictureFlow( 10, 10, 390, 200, -1, windowAnonce:Self() )
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		
		if not game:IsSaling() then 
			anoncePictureFlow:AddItem( game:GetViewImage(), game:GetName(), game:Self() )
		end	
	end
	
	local btnOk = guienv:AddButton( 10, 240 - 40, 190, 240, windowAnonce:Self(), -1, "������ �������" )
	btnOk:SetAction( "sworkWindowShopStartGameSaling" )
	
	local btnCancel = guienv:AddButton( 210, 240 - 40, 390, 240, windowAnonce:Self(), -1, "�����" )
	btnCancel:SetAction( "sworkWindowShopCloseAnonceGame" )
end

function StartGameSaling()
	local game = anoncePictureFlow:GetSelectedObject()
	
	applic:AddGameToMarket( game )
	localFillListboxGame()
	sworkWindowShopCloseAnonceGame()	
end

function ShowMonthTopList()

end

function ShowAllTimeTopList()

end

function CloseAnonceGame()
	windowAnonce:Remove()
end