local base = _G

module( "reklameManager" ) 

local company = nil
local plant = nil
local applic = base.applic
local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local mainWindow = nil
local campaniesWindow = nil
local picflowReklames = nil
local lableDayNumber = nil
local btnDecDayNumber = nil
local btnIncDayNumber = nil
local labelCostInDay = nil
local labelPrefFamous = nil
local labelGameFamous = nil
local labelPrice = nil
local labelName = nil

local addingDays = 0

local currentWork = nil

local lbxGames = nil
local selectedGame = nil
local linkSelectedGame = nil

local btnApplyWork = nil

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

local function localAddLabel( textr, x1, y1, x2, y2 )
	local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, campaniesWindow:Self() )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	label:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )

	return label
end

function Show()
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/marketing_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./reklameWindow:Remove(); reklameWindow = nil" )
	end
	
	--get loan
	button.EqualeTexture( 534, 255, "reklameCampanies", mainWindow:Self(), -1, "", "./reklameManager.ShowCampaniesManager()" )
end

function ShowCampaniesManager()
	company = applic:GetPlayerCompany()
	plant = base.NrpGetPlant()
	
	if campaniesWindow == nil then
		campaniesWindow = guienv:AddWindow( "media/marketing_select.png", 0, 0, scrWidth, 
									   scrHeight, -1, guienv:GetRootGUIElement() )
		campaniesWindow:GetCloseButton():SetVisible( false )
	end
		
	--блок рекламы на листовках
	picflowReklames = guienv:AddPictureFlow( 60, 10, scrWidth - 10, scrHeight / 3, -1, campaniesWindow:Self() )
	picflowReklames:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
	picflowReklames:SetDrawBorder( false )

	btnDecDayNumber = guienv:AddButton( 10, scrHeight / 3 + 20, 70, scrHeight / 3 + 80, campaniesWindow:Self(), -1, "-" )
	btnDecDayNumber:SetAction( "./reklameManager.DecDay()" )
	btnDecDayNumber:SetVisible( false )
	
	btnIncDayNumber = guienv:AddButton( scrWidth / 2 - 80, scrHeight / 3 + 20, 
										scrWidth / 2 - 20, scrHeight / 3 + 80, 
										campaniesWindow:Self(), -1, "+" ) 										
	btnIncDayNumber:SetAction( "./reklameManager.IncDay()" )
	btnIncDayNumber:SetVisible( false )
	
	local yof = scrHeight / 3
	local rightY = scrWidth / 2 - 80
	labelName = localAddLabel( "", 60, yof + 0, rightY, yof + 40 )
	lableDayNumber = localAddLabel( "", 60, yof + 40, rightY, yof + 90 )
	labelPrice = localAddLabel( "0", 60, yof + 100, rightY, yof + 150 ) 
	labelCostInDay = localAddLabel( "0", 60, yof + 160, rightY, yof + 210 ) 
	labelPrefFamous = localAddLabel( "0", 60, yof + 220, rightY, yof + 270 ) 
	labelGameFamous = localAddLabel( "0", 60, yof + 280, rightY, yof + 330 ) 
	
	picflowReklames:AddItem( reklames[ 1 ]:GetTexture(), reklames[ 1 ]:GetName(), reklames[ 1 ]:Self() )
	--блок рекламы в газете
	picflowReklames:AddItem( reklames[ 2 ]:GetTexture(), reklames[ 2 ]:GetName(), reklames[ 2 ]:Self() )
	--блок рекламы на радио
	picflowReklames:AddItem( reklames[ 3 ]:GetTexture(), reklames[ 3 ]:GetName(), reklames[ 3 ]:Self() )
	--блок рекламы по телевидению
	picflowReklames:AddItem( reklames[ 4 ]:GetTexture(), reklames[ 4 ]:GetName(), reklames[ 4 ]:Self() )
	
	lbxGames = guienv:AddComponentListBox( scrWidth / 2 + 10, scrHeight / 3 + 20, scrWidth - 10, scrHeight - 80, -1, campaniesWindow:Self() )
	localFillGamesListBox()
		
	btnApplyWork = guienv:AddButton( 10, scrHeight - 70, scrWidth / 2 - 10, scrHeight - 20, campaniesWindow:Self(), -1, base.STR_APPLY )
	btnApplyWork:SetAction( "./reklameManager.ApplyNewWork()" )
	btnApplyWork:SetVisible( false )
	
	local btnExit = guienv:AddButton( scrWidth / 2 + 10, scrHeight - 70, scrWidth - 10, scrHeight - 20, campaniesWindow:Self(), -1, base.STR_EXIT )
	btnExit:SetAction( "./reklameManager.HideCampaniesWindow()" )
	
	--заказать статью в игровом журнале
		--список журналов
			
	--картинка с отображением игры
	campaniesWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./reklameManager.SelectNewWork()" )
end

function HideCampaniesWindow()
	campaniesWindow:Remove()
	campaniesWindow = nil
end

local function localUpdateLabels()
	labelName:SetText( currentWork:GetTypeName() )
	local text = base.STR_DAYLEFT .. "  " .. currentWork:GetNumberDay()
	
	if addingDays > 0 then 
		text = text .. base.string.format( "( %s  %d )", base.STR_ADDDAYS, addingDays )
	end
	
	lableDayNumber:SetText( text )
	labelPrice:SetText( base.STR_MONEYLEFT .. "   $" .. currentWork:GetPrice() )
	labelCostInDay:SetText(  base.STR_DAYCOST .. "   $" .. currentWork:GetDayCost() )	
	labelPrefFamous:SetText( base.STR_PREFFAMOUS .. "   " .. currentWork:GetFamous() .. "%" ) 
	labelGameFamous:SetText( selectedGame:GetName() .. ":  " .. base.STR_FAMOUS .. "  " .. selectedGame:GetFamous() .. "%" ) 	
end

function SelectNewWork()
	currentWork = base.CLuaReklame( picflowReklames:GetSelectedObject() )
	selectedGame = base.CLuaDevelopProject( lbxGames:GetSelectedObject() )
	currentWork:SetReklameObject( selectedGame:GetName() )
	addingDays = 0	
	localUpdateLabels()	
	
	local vis = selectedGame:Empty() == 0
	btnApplyWork:SetVisible( vis )
	btnIncDayNumber:SetVisible( vis )
	btnDecDayNumber:SetVisible( vis )
end

function IncDay()
	addingDays = addingDays + 10
	
	localUpdateLabels()
end

function DecDay()
	if addingDays - 10 >= 0 then
		addingDays = addingDays - 10
	end

	localUpdateLabels()
end

function ApplyNewWork()
	if addingDays > 0 then
		currentWork:SetNumberDay( currentWork:GetNumberDay() + addingDays )
		plant:AddReklameWork( currentWork:Self() )
		addingDays = 0
		localUpdateLabels()
	end
end

