local base = _G

module( "reklameManager" ) 

local company = nil
local plant = nil
local applic = base.applic
local button = base.button
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
local realCampany = nil

local lbxGames = nil
local selectedGame = nil
local linkSelectedGame = nil

local btnApplyWork = nil

local reklames = {}

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

local function localCreateReklames()
	for i=1, plant:GetBaseReklameNumber() do
		reklames[ i ] = plant:GetBaseReklame( i-1 )
	end
end

function Show()
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/marketing_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./reklameManager.Hide()" )
	end
	
	--get loan
	button.EqualeTexture( 534, 255, "reklameCampanies", mainWindow:Self(), -1, "", "./reklameManager.ShowCampaniesManager()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "reklameManager.FadeEnterAction()" )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "reklameManager.FadeExitAction()" )	
end

function FadeEnterAction()
	mainWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function ShowCampaniesManager()
	company = applic.playerCompany
	plant = base.NrpGetPlant()
	
	if campaniesWindow == nil then
		campaniesWindow = guienv:AddWindow( "media/maps/marketing_select.png", 0, 0, scrWidth, 
									   scrHeight, -1, guienv:GetRootGUIElement() )
		campaniesWindow:GetCloseButton():SetVisible( false )
	end
		
		
	if #reklames == 0 then localCreateReklames() end
		
	--блок рекламы на листовках
	picflowReklames = guienv:AddPictureFlow( 60, 60, scrWidth - 10, 60 + scrHeight / 3, -1, campaniesWindow:Self() )
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
	
	for y=1, #reklames do
		picflowReklames:AddItem( reklames[ y ]:GetTexture(), reklames[ y ]:GetName(), reklames[ y ]:Self() )
	end
	
	lbxGames = guienv:AddComponentListBox( scrWidth / 2 + 10, scrHeight / 3 + 20, scrWidth - 10, scrHeight - 80, -1, campaniesWindow:Self() )
	localFillGamesListBox()
		
	btnApplyWork = guienv:AddButton( 10, scrHeight - 70, scrWidth / 2 - 10, scrHeight - 20, campaniesWindow:Self(), -1, base.STR_STARTREKLAME )
	btnApplyWork:SetAction( "./reklameManager.QuerryUserToApplyWork()" )
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

local function localGetGameFamous( value )
	if value >= 0 and value <= 10 then
		return base.ABOUT_GAME_KNOWN_NULL
	elseif value > 10 and value <= 25 then
		return base.ABOUT_GAME_KNOWN_LESS10
	elseif value > 25 and value <= 50 then
		return base.ABOUT_GAME_KNOWN_LESS25
	elseif value > 50 and value <= 75 then
		return base.ABOUT_GAME_KNOWN_LESS50
	elseif value > 75 and value <= 97 then
		return base.ABOUT_GAME_KNOWN_LESS75
	else
		return base.ABOUT_GAME_KNOWN_LESS97
	end
end

local function localUpdateLabels()
	labelName:SetText( currentWork:GetUniq() )
	local text = base.STR_DAYLEFT .. "  " .. realCampany:GetNumberDay()
	
	if addingDays > 0 then 
		text = text .. base.string.format( "( %s  %d )", base.STR_ADDDAYS, addingDays )
	end
	
	lableDayNumber:SetText( text )
	labelPrice:SetText( base.STR_MONEYLEFT .. "   $" .. realCampany:GetPrice() )
	labelCostInDay:SetText(  base.STR_DAYCOST .. "   $" .. currentWork:GetDayCost() )	
	labelPrefFamous:SetText( base.STR_PREFFAMOUS .. "   " .. realCampany:GetFamous() .. "%" ) 
	labelGameFamous:SetText( localGetGameFamous( selectedGame:GetFamous() ).."\n( dbg ".. selectedGame:GetFamous() .. " %)" ) 	
end

function SelectNewWork()
	currentWork = base.CLuaReklame( picflowReklames:GetSelectedObject() )
	selectedGame = base.CLuaDevelopProject( lbxGames:GetSelectedObject() )
	currentWork:SetReklameObject( selectedGame )
	addingDays = 0	
	
	local vis = selectedGame:Empty() == 0
	btnApplyWork:SetVisible( vis )
	btnIncDayNumber:SetVisible( vis )
	btnDecDayNumber:SetVisible( vis )

	realCampany = plant:GetReklame( currentWork:GetUniq(), selectedGame:GetName() )
	localUpdateLabels()		
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
	local parent = base.CLuaElement( base.NrpGetSender() ):GetParent()
	local parentCompany = applic:GetCompanyByName( currentWork:GetCompanyName() )
	
	plant:AddReklameWork( currentWork:Self() )
	addingDays = 0
	localUpdateLabels()
	
	--снимем деньги со счета компании для проведения рекламной акции
	parentCompany:AddBalance( -1 * currentWork:GetDayCost() * currentWork:GetNumberDay() )
	
	base.CLuaElement( parent ):Remove()
end

function QuerryUserToApplyWork()
	if addingDays > 0 then
		currentWork:SetNumberDay( addingDays )
		local text = "Рекламная кампания:"..currentWork:GetName().."\n"
		text = text .. "Стоимость:" .. currentWork:GetDayCost() * currentWork:GetNumberDay() .. "\n"
		text = text .. "Длительность:" .. currentWork:GetNumberDay()
		guienv:MessageBox( text, true, true, "./reklameManager.ApplyNewWork()", "./reklameManager.CancelNewWork()" )
	else
		guienv:MessageBox( "Выберите количество дней кампании", false, false, "", "" )
	end
end

