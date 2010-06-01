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
local width = 800
local height = 600

function sworkCreateGameInSaleWindow( ptr )
	windowShop:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() ) )
	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	listboxGames:SetObject( guienv:AddListBox( 20, 20, width / 2 - 10, height - 20, -1, windowShop:Self() ) )
	
	local game = CLuaGame( nil )
	for i=0, applic:GetSaleGamesNumber() do
		game:SetObject( applic:GetSaleGame( i ) )
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
	labelCurrentMonthSale:SetObject( guienv:AddLabel( "������ �� ������� �����:", hw + 20, hh + 80, 
													  width - 20, hh + 100, -1, windowShop:Self() ) )
	--����� ������� �����
	labelAllTimeSale:SetObject( guienv:AddLabel( "������ �� ������� �����:", hw + 20, hh + 110, 
													  width - 20, hh + 130, -1, windowShop:Self() ) )
	--������� ������� ����
	prgRating:SetObject( guienv:AddProgressBar( windowShop:Self(), hw + 20, hh + 140, 
													width - 20, hh + 160, -1 ) )

	--���� ���� � ������������ �������� ����
	btnDecreaseGamePrice:SetObject( guienv:AddButton( hw + 20, hh + 170, width - 20, hh + 190, 
									windowShop:Self(), -1, "-" ) )
									
	labelGamePrice:SetObject( guienv:AddLabel( "����:", hw + 20, hh + 170, 
													 width - 20, hh + 190, -1, windowShop:Self() ) )	
													 
	btnIncreaseGamePrice:SetObject( guienv:AddButton( hw + 20, hh + 170, width - 20, hh + 190, 
									windowShop:Self(), -1, "+" ) )
	
end
