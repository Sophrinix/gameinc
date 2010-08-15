local company = nil

local width = 800
local height = 600

local wndReklame = nil
local linkPaperProduction = CLuaLinkBox( nil )
local lableDayNumberPaper = nil
local btnDecDayNumberPaperRklm = CLuaButton( nil )
local btnIncDayNumberPaperRklm = CLuaButton( nil )
local labelPricePaperRklm = nil

local linkMagazineProduction = CLuaLinkBox( nil )
local btnDecDayNumberMagazineRklm = CLuaButton( nil )
local btnIncDayNumberMagazineRklm = CLuaButton( nil )
local labelPriceMagazineRklm = nil
local lableDayNumberMagazine = nil

local linkRadioProduction = CLuaLinkBox( nil )
local btnDecDayNumberRadioRklm = CLuaButton( nil )
local btnIncDayNumberRadioRklm = CLuaButton( nil )
local labelPriceRadioRklm = nil
local lableDayNumberRadio = nil

local linkTeleProduction = CLuaLinkBox( nil )
local btnDecDayNumberTeleRklm = CLuaButton( nil )
local btnIncDayNumberTeleRklm = CLuaButton( nil )
local labelPriceTeleRklm = nil
local lableDayNumberTV = nil

local lbxGames = CLuaListBox( nil )
local selectedGame = CLuaGame( nil )
local linkSelectedGame = CLuaLinkBox( nil )

local paperWork = CLuaReklame( nil )
paperWork:Create( "paper", "" )

local magazineWork = CLuaReklame( nil )
magazineWork:Create( "magazine", "" )

local radioWork = CLuaReklame( nil )
radioWork:Create( "radio", "" )

local tvWork = CLuaReklame( nil )
tvWork:Create( "tv", "" )

local function localFillGamesListBox()
	local game = CLuaGame( nil )
	for i=1, company:GetGameNumber() do
		game:SetObject( company:GetGame( i-1 ) )
		lbxGames:AddItem( game:GetName(), game:Self() )
	end	
end

function sworkCreateWindowReklama( ptr )
	wndReklame:SetObject( guienv:GetElementByID( WINDOW_REKLAME_ID ) )
	company = applic:GetPlayerCompany()
	
	if wndReklame:Empty() == 0 then
		wndReklame:Remove()
	end
	
	wndReklame = guienv:AddWindow( "ReklameWindow", 0, 20, width, height, WINDOW_REKLAME_ID, guienv:GetRootGUIElement() )
	
	--���� ������� �� ���������
	linkPaperProduction:SetObject( guienv:AddLinkBox( "��������", 
												      10, 20, 
												      200, 210, 
												      -1, wndReklame:Self() ) )
	btnDecDayNumberPaperRklm = guienv:AddButton( 10, 220, 50, 240, wndReklame:Self(), -1, "-" )
	btnIncDayNumberPaperRklm = guienv:AddButton( 160, 220, 200, 240, wndReklame:Self(), -1, "+" ) 
	lableDayNumberPaper = guienv:AddLabel( "0", 60, 220, 160, 240, -1, wndReklame:Self() )

	btnIncDayNumberPaperRklm:SetAction( "sworkWindowReklamaChangeDayNumberPaper" )
	btnDecDayNumberPaperRklm:SetAction( "sworkWindowReklamaChangeDayNumberPaper" )
	labelPricePaperRklm = guienv:AddLabel( "���������", 10, 240, 200, 260, -1, wndReklame:Self() )

	--���� ������� � ������
	linkMagazineProduction:SetObject( guienv:AddLinkBox( "������", 
												      210, 20, 
												      400, 210, 
												      -1, wndReklame:Self() ) )
	btnDecDayNumberMagazineRklm = guienv:AddButton( 210, 220, 260, 240, wndReklame:Self(), -1, "-" )
	btnIncDayNumberMagazineRklm = guienv:AddButton( 360, 220, 400, 240, wndReklame:Self(), -1, "+" )
	lableDayNumberMagazine = guienv:AddLabel( "0", 260, 220, 360, 240, -1, wndReklame:Self() )

	btnIncDayNumberMagazineRklm:SetAction( "sworkWindowReklamaChangeDayNumberMagazine" )
	btnDecDayNumberMagazineRklm:SetAction( "sworkWindowReklamaChangeDayNumberMagazine" )
	labelPriceMagazineRklm = guienv:AddLabel( "���������", 210, 250, 400, 270, -1, wndReklame:Self() )
	
	--���� ������� �� �����
	linkRadioProduction:SetObject( guienv:AddLinkBox( "�����", 
												      410, 20, 
												      600, 210, 
												      -1, wndReklame:Self() ) )
	btnDecDayNumberRadioRklm = guienv:AddButton( 410, 220, 460, 240, wndReklame:Self(), -1, "-" )
	lableDayNumberRadio = guienv:AddLabel( "0", 460, 220, 560, 240, -1, wndReklame:Self() )
	
	btnIncDayNumberRadioRklm = guienv:AddButton( 560, 220, 600, 240, wndReklame:Self(), -1, "+" )
	btnIncDayNumberRadioRklm:SetAction( "sworkWindowReklamaChangeDayNumberRadio" )
	btnDecDayNumberRadioRklm:SetAction( "sworkWindowReklamaChangeDayNumberRadio" )
	labelPriceRadioRklm = guienv:AddLabel( "���������", 410, 280, 600, 300, -1, wndReklame:Self() )

	--���� ������� �� �����������
	linkTeleProduction:SetObject( guienv:AddLinkBox( "�����������", 
												      610, 20, 
												      800, 210, 
												      -1, wndReklame:Self() ) )
	btnDecDayNumberTeleRklm = guienv:AddButton( 610, 220, 660, 240, wndReklame:Self(), -1, "-" )
	lableDayNumberTV = guienv:AddLabel( "0", 660, 220, 760, 240, -1, wndReklame:Self() )
	
	btnIncDayNumberTeleRklm = guienv:AddButton( 760, 220, 800, 240, wndReklame:Self(), -1, "+" )
	btnIncDayNumberTeleRklm:SetAction( "sworkWindowReklamaChangeDayNumberTV" )
	btnDecDayNumberTeleRklm:SetAction( "sworkWindowReklamaChangeDayNumberTV" )
	labelPriceTeleRklm =  guienv:AddLabel( "���������", 610, 280, 800, 300, -1, wndReklame:Self() )
	
	lbxGames:SetObject( guienv:AddListBox( width / 2 + 10, 310, width - 10, height - 80, -1, wndReklame:Self() ) )
	localFillGamesListBox()
	linkSelectedGame:SetObject( guienv:AddLinkBox( "Image", width / 4 - 100, 310, width / 4 + 100, height - 80, -1, wndReklame:Self() ))
		
	local btnApplyWork = guienv:AddButton( 10, height - 70, width / 2 - 10, height - 20, wndReklame:Self(), -1, "���������" )
	btnApplyWork:SetAction( "sworkWindowReklamaApplyNewWork" )
	
	local btnExit = guienv:AddButton( width / 2 + 10, height - 70, width - 10, height - 20, wndReklame:Self(), -1, "�����" )
	btnExit:SetAction( "sworkCloseWindowReklama" )
	--���� ������� � ���������
		--���� �� ���� ������ �������� ������� � �����
		
	--�������� ������ � ������� �������
		--������ ��������
		
	--������ � ��������� ��������, 
	--� ������ ������������ ����������� ������� �������
	
	--�������� � ������������ ����
end

function sworkWindowReklamaChangeDayNumberPaper( ptr )
	if ptr == btnDecDayNumberPaperRklm:Self() then
		if paperWork:GetNumberDay() - 10 then
			paperWork:SetNumberDay( paperWork:GetNumberDay() - 10 )
		end
	else
		paperWork:SetNumberDay(  paperWork:GetNumberDay() + 10 )
	end
	
	lableDayNumberPaper:SetText( paperWork:GetNumberDay() )
end

function sworkWindowReklamaChangeDayNumberMagazine( ptr )
	if ptr == btnDecDayNumberMagazineRklm:Self() then
		if tvWork:GetNumberDay() - 10 then
			tvWork:SetNumberDay( tvWorkwd:GetNumberDay() - 10 )
		end
	else
		magazineWork:SetNumberDay( magazineWork:GetNumberDay() + 10 )	
	end

	lableDayNumberMagazine:SetText( magazineWork:GetNumberDay() )	
end

function sworkWindowReklamaChangeDayNumberTV( ptr )
	if ptr == btnDecDayNumberTeleRklm:Self() then
		if tvWork:GetNumberDay() - 10 then
			tvWork:SetNumberDay( tvWork:GetNumberDay() - 10 )
		end
	else
			tvWork:SetNumberDay( tvWork:GetNumberDay() + 10 )		
	end

	lableDayNumberTV:SetText( tvWork:GetNumberDay() )
end

function sworkWindowReklamaChangeDayNumberRadio( ptr )
	if ptr == btnDecDayNumberRadioRklm:Self() then
		if radioWork:GetNumberDay() - 10 then
			radioWork:SetNumberDay( radioWork:GetNumberDay() - 10 )
		end
	else
		radioWork:SetNumberDay( radioWork:GetNumberDay() + 10 )		
	end
	
	lableDayNumberRadio:SetText( radioWork:GetNumberDay() )
end

function sworkWindowReklamaApplyNewWork( ptr )
	if paperWork:GetNumberDay() > 0 then
		plant:AddReklameWork( paperWork:Self() )
	end
	
	if magazineWork:GetNumberDay() then
		plant:AddReklameWork( magazineWork:Self() )
	end
	
	if radioWork:GetNumberDay() then
		plant:AddReklameWork( radioWork:Self() )			
	end
	
	if tvWork:GetNumberDay() then
		plant:AddReklameWork( tvWork:Self() )			
	end
end

function sworkCloseWindowReklama( ptr )
		
end

