local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local applic = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( applic:GetPlayerCompany() )

local windowShop = CLuaWindow( nil )
local listboxGames = CLuaListBox( nil )
local buttonAnonceGame = CLuaButton( nil )
local imageGamePreview = CLuaImage( nil )
local labelGameName = CLuaLabel( nil )
local labelLastMonthSale = CLuaLabel( nil )
local labelCurrentMonthSale = CLuaLabel( nil )
local labelAllTimeSale = CLuaLabel( nil )
local prgRating = CLuaProgressBar( nil )
local btnDecreaseGamePrice = CLuaButton( nil )
local labelGamePrice = CLuaLabel( nil )
local btnIncreaseGamePrice = CLuaButton( nil )
local listboxCompanyGame = CLuaListBox( nil )
local anoncePictureFlow = CLuaPictureFlow( nil )
local windowAnonce = CLuaWindow( nil )
local width = 800
local height = 600

function sworkCreateGameInSaleWindow( ptr )
	windowShop:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() ) )
	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	listboxGames:SetObject( guienv:AddListBox( 20, 20, width / 2 - 10, height - 20, -1, windowShop:Self() ) )
	
	local game = CLuaGame( nil )
	for i=1, applic:GetMarketGamesNumber() do
		game:SetObject( applic:GetMarketGame( i-1 ) )
		listboxGames:AddItem( game:GetName(), game:Self() )
	end
	
	--���������� ������ "������������ ����", �� ������� ����� ��������� ���� �� �����
	buttonAnonceGame:SetObject( guienv:AddButton( width / 2, height -50, width - 20, height - 20, 
								windowShop:Self(), -1, "������������ ����" ) )
	buttonAnonceGame:SetAction( "sworkWindowShopAnonceGame" )
	
	--���������� ����������� ���� ������ �� ������
	imageGamePreview:SetObject( guienv:AddImage( width / 2 + 20, 20, width - 20, height / 2 - 20, windowShop:Self(), -1, "" ) ) 
	
	--���������� ��� ������������ �������� ��������� ������
	--�������� ����
	local hw = width / 2 + 20
	local hh = height / 2 + 20
	labelGameName:SetObject( guienv:AddLabel( "", hw + 20, hh + 20, width - 20, hh + 40, -1, windowShop:Self() ) )
	
	--������ �� ������� �����
	labelLastMonthSale:SetObject( guienv:AddLabel( "������ �� ������� �����:", hw + 20, hh + 50, 
												   width - 20, hh + 70, -1, windowShop:Self() ) )
	--������ �� ������� �����
	labelCurrentMonthSale:SetObject( guienv:AddLabel( "������ �� ���� �����:", hw + 20, hh + 80, 
													  width - 20, hh + 100, -1, windowShop:Self() ) )
	--����� ������� �����
	labelAllTimeSale:SetObject( guienv:AddLabel( "������ �� ��� �����:", hw + 20, hh + 110, 
													  width - 20, hh + 130, -1, windowShop:Self() ) )
	--������� ������� ����
	prgRating:SetObject( guienv:AddProgressBar( windowShop:Self(), hw + 20, hh + 140, 
													width - 20, hh + 160, -1 ) )
	--prgRating:SetImage( )
	--prgRating:SetFillImage( )

	--���� ���� � ������������ �������� ����
	btnDecreaseGamePrice:SetObject( guienv:AddButton( hw + 20, hh + 170, hw + 40, hh + 190, 
									windowShop:Self(), -1, "-" ) )
	btnDecreaseGamePrice:SetAction( "sworkWindowShopDecreaseGamePrice" ) 
									
	labelGamePrice:SetObject( guienv:AddLabel( "����:", hw + 40, hh + 170, 
													    width - 40, hh + 190, -1, windowShop:Self() ) )	
													 
	btnIncreaseGamePrice:SetObject( guienv:AddButton(  width - 40, hh + 170, width - 20, hh + 190, 
									windowShop:Self(), -1, "+" ) )
	btnIncreaseGamePrice:SetAction( "sworkWindowShopIncreaseGamePrice" ) 	
end

function sworkWindowShopAnonceGame( ptr )
	windowAnonce:SetObject( guienv:AddWindow( "", width / 2 - 200, height / 2 - 120,
						  				      width / 2 + 200, height / 2 + 120, 
								              -1, windowShop:Self() ) )
	
	anoncePictureFlow = CLuaPictureFlow( guienv:AddPictureFlow( 10, 10, 390, 200, -1, windowAnonce:Self() ) )
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company:GetGameNumber() do
		local game = CLuaGame( company:GetGame( i-1 ) )
		
		if not game:IsSaling() then 
			anoncePictureFlow:AddItem( nil, game:GetName(), game:Self() )
		end	
	end
	
	local btnOk = CLuaButton( guienv:AddButton( 10, 240 - 40, 190, 240, windowAnonce:Self(), -1, "������ �������" ) )
	btnOk:SetAction( "sworkWindowShopStartGameSaling" )
	
	local btnCancel = CLuaButton( guienv:AddButton( 210, 240 - 40, 390, 240, windowAnonce:Self(), -1, "�����" ) )
	btnCancel:SetAction( "sworkWindowShopCloseAnonceGame" )
end

function sworkWindowShopStartGameSaling( ptr )
	local game = anoncePictureFlow:GetSelectedObject()
	
	applic:AddMarketGame( game )
	sworkWindowShopCloseAnonceGame( ptr )	
end

function sworkCreateMonthTopListWindow( ptr )

end

function sworkCreateAllTimeTopListWindow( ptr )

end

function sworkWindowShopCloseAnonceGame( ptr )
	windowAnonce:Remove()
end