local base = _G

module( "reklameManager" ) 

local company = nil
local plant = nil
local applic = base.applic
local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local wndReklame = nil
local picflowReklames = nil
local lableDayNumber = nil
local btnDecDayNumber = nil
local btnIncDayNumber = nil
local labelPrice = nil
local currentWork = nil

local lbxGames = nil
local selectedGame = nil
local linkSelectedGame = nil

local reklames = {}
reklames[ 1 ] = base.CLuaReklame():Create( "paper", "" )
reklames[ 2 ] = base.CLuaReklame():Create( "magazine", "" )
reklames[ 3 ] = base.CLuaReklame():Create( "radio", "" )
reklames[ 4 ] = base.CLuaReklame():Create( "tv", "" )

local function localFillGamesListBox()
	local game = nil
	for i=1, company:GetGameNumber() do
		game = company:GetGame( i-1 )
		lbxGames:AddItem( game:GetName(), game:Self() )
	end	
	
	local project = nil
	for i=1, company:GetDevProjectNumber() do
		project = company:GetDevProject( i-1 )
		lbxGames:AddItem( project:GetName(), project:Self() )
	end	
end

function Show()
	company = applic:GetPlayerCompany()
	plant = base.NrpGetPlant()
	
	if wndReklame == nil then
		wndReklame = guienv:AddWindow( "media/marketing_select.png", 0, 0, scrWidth, 
									   scrHeight, -1, guienv:GetRootGUIElement() )
		wndReklame:GetCloseButton():SetVisible( false )
	end
		
	--блок рекламы на листовках
	picflowReklames = guienv:AddPictureFlow( 60, 10, scrWidth - 10, scrHeight / 3, -1, wndReklame:Self() )
	picflowReklames:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
	picflowReklames:SetDrawBorder( false )

	btnDecDayNumber = guienv:AddButton( 10, scrHeight / 3 + 20, 70, scrHeight / 3 + 80, wndReklame:Self(), -1, "-" )
	btnDecDayNumber:SetAction( "./reklameManager.IncDay()" )
	
	btnIncDayNumber = guienv:AddButton( scrWidth / 2 - 80, scrHeight / 3 + 20, 
										scrWidth / 2 - 20, scrHeight / 3 + 80, 
										wndReklame:Self(), -1, "+" ) 										
	btnIncDayNumber:SetAction( "./reklameManager.DecDay()" )
	
	local yof = scrHeight / 3
	lableDayNumber = guienv:AddLabel( "0", 60, yof + 20, scrWidth / 2 - 80, yof + 80, -1, wndReklame:Self() )
	lableDayNumber:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	
	labelPrice = guienv:AddLabel( "0", 60, yof + 100, scrWidth / 2 - 80, yof + 160, -1, wndReklame:Self() ) 
	labelPrice:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )

	picflowReklames:AddItem( reklames[ 1 ]:GetTexture(), reklames[ 1 ]:GetName(), reklames[ 1 ]:Self() )
	--блок рекламы в газете
	picflowReklames:AddItem( reklames[ 2 ]:GetTexture(), reklames[ 2 ]:GetName(), reklames[ 2 ]:Self() )
	--блок рекламы на радио
	picflowReklames:AddItem( reklames[ 3 ]:GetTexture(), reklames[ 3 ]:GetName(), reklames[ 3 ]:Self() )
	--блок рекламы по телевидению
	picflowReklames:AddItem( reklames[ 4 ]:GetTexture(), reklames[ 4 ]:GetName(), reklames[ 4 ]:Self() )
	
	lbxGames = guienv:AddComponentListBox( scrWidth / 2 + 10, scrHeight / 3 + 20, scrWidth - 10, scrHeight - 80, -1, wndReklame:Self() )
	localFillGamesListBox()
		
	local btnApplyWork = guienv:AddButton( 10, scrHeight - 70, scrWidth / 2 - 10, scrHeight - 20, wndReklame:Self(), -1, base.STR_APPLY )
	btnApplyWork:SetAction( "./reklameManager.ApplyNewWork()" )
	
	local btnApplyWork = guienv:AddButton( scrWidth / 2 + 10, scrHeight - 70, scrWidth - 10, scrHeight - 20, wndReklame:Self(), -1, base.STR_EXIT )
	btnApplyWork:SetAction( "./reklameManager.Hide()" )
	
	--заказать статью в игровом журнале
		--список журналов
			
	--картинка с отображением игры
	wndReklame:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./reklameManager.SelectNewWork()" )
end

function Hide()
	wndReklame:Remove()
	wndReklame = nil
end

function SelectNewWork()
	currentWork = CLuaReklame( picflowReklames:GetSelectedObject() )
	
	lableDayNumber:SetText( currentWork:GetNumberDay() )
	labelPrice:SetText( currentWork:GetPrice() )
end

function IncDay()
	currentWork:SetNumberDay(  currentWork:GetNumberDay() + 10 )
	
	lableDayNumber:SetText( currentWork:GetNumberDay() )
end

function DecDay()
	if currentWork:GetNumberDay() - 10 > 0 then
		currentWork:SetNumberDay( currentWork:GetNumberDay() - 10 )
	end

	lableDayNumber:SetText( currentWork:GetNumberDay() )	
end

function ApplyNewWork()
	if currentWork:GetNumberDay() > 0 then
		plant:AddReklameWork( currentWork:Self() )
	end
end

