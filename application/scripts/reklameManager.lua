local base = _G

module( "reklameManager" ) 

btnReklame = nil

local company = nil
local plant = nil
local applic = base.applic
local window = base.window
local button = base.button
local tutorial = base.tutorial
local guienv = base.guienv

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
	for i=1, company.gameNumber do
		game = company:GetGame( i-1 )
		lbxGames:AddItem( game.name, game.object )
	end	
	
	local project = nil
	for i=1, company.devProjectNumber do
		project = company:GetDevProject( i-1 )
		lbxGames:AddItem( project.name, project.object )
	end	
end

local function localAddLabel( textr, x1, y1, x2, y2 )
	local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, campaniesWindow )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	label.color = base.toColor( 0xff, 0xff, 0xff, 0xff )

	return label
end

local function localCreateReklames()
	for i=1, plant.reklameNumber do
		reklames[ i ] = plant:GetBaseReklame( i-1 )
	end
end

local function _Hide()

end

local function _IncDay()
	addingDays = addingDays + 10
	
	localUpdateLabels()
end

local function _DecDay()
	if addingDays - 10 >= 0 then
		addingDays = addingDays - 10
	end

	localUpdateLabels()
end

local function _SelectNewWork()
	currentWork = base.CLuaReklame( picflowReklames.selectedObject )
	selectedGame = base.CLuaDevelopProject( lbxGames.selectedObject )
	currentWork:SetReklameObject( selectedGame )
	addingDays = 0	
	
	local vis = selectedGame.empty
	btnApplyWork.visible = vis
	btnIncDayNumber.visible = vis
	btnDecDayNumber.visible = vis

	realCampany = plant:GetReklame( currentWork.uniq, selectedGame.name )
	localUpdateLabels()		
end

local function _QuerryUserToApplyWork()
	if addingDays > 0 then
		currentWork:SetNumberDay( addingDays )
		local text = "Рекламная кампания:"..currentWork.name.."\n"
		text = text .. "Стоимость:" .. currentWork.dayCost * currentWork.numberDay .. "\n"
		text = text .. "Длительность:" .. currentWork.numberDay
		guienv:MessageBox( text, true, true, ApplyNewWork, CancelNewWork )
	else
		guienv:MessageBox( "Выберите количество дней кампании", false, false, nil, nil )
	end
end

local function _HideCampaniesWindow()
	campaniesWindow:Remove()
	campaniesWindow = nil
end

local function _ShowCampaniesManager()
	company = applic.playerCompany
	plant = applic.plant
	
	if campaniesWindow == nil then
		campaniesWindow = guienv:AddWindow( "media/maps/marketing_select.png", 0, 0, "0e", "0e", -1, guienv.root )
		campaniesWindow.closeButton.visible = false
	end
		
		
	if #reklames == 0 then localCreateReklames() end
		
	--блок рекламы на листовках
	picflowReklames = guienv:AddPictureFlow( 60, 60, "10e", "33%+", -1, campaniesWindow )
	picflowReklames:SetDrawBorder( false )

	btnDecDayNumber = guienv:AddButton( 10, "33%", 70, "80+", campaniesWindow, -1, "-" )
	btnDecDayNumber.action = _DecDay
	btnDecDayNumber.visible = false
	
	btnIncDayNumber = guienv:AddButton( "45%", "33%", "20+", "20+", campaniesWindow, -1, "+" ) 										
	btnIncDayNumber.action = _IncDay
	btnIncDayNumber.visible = false
	
	labelName = localAddLabel( "", 60, "30%", "45%", "40+" )
	lableDayNumber = localAddLabel( "", 60, "35%", "45%", "40+" )
	labelPrice = localAddLabel( "0", 60, "40%", "45%", "40+" ) 
	labelCostInDay = localAddLabel( "0", 60, "45%", "45%", "40+" ) 
	labelPrefFamous = localAddLabel( "0", 60, "50%", "45%", "40+" ) 
	labelGameFamous = localAddLabel( "0", 60, "55%", "45%", "40+" ) 
	
	for y=1, #reklames do
		picflowReklames:AddItem( reklames[ y ].texture, reklames[ y ].name, reklames[ y ].object )
	end
	
	lbxGames = guienv:AddComponentListBox( "51%", "33%", "10e", "80e", -1, campaniesWindow )
	localFillGamesListBox()
		
	btnApplyWork = guienv:AddButton( 10, "70e", "45%", "20e", campaniesWindow, -1, base.STR_STARTREKLAME )
	btnApplyWork.action = _QuerryUserToApplyWork
	btnApplyWork.visible = false
	
	local btnExit = guienv:AddButton( "55%", "70e", "10e", "20e", campaniesWindow, -1, base.STR_EXIT )
	btnExit.action = _HideCampaniesWindow
	
	--заказать статью в игровом журнале
		--список журналов
			
	--картинка с отображением игры
	campaniesWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, _SelectNewWork )
end

function Show()
	mainWindow = window.fsWindow( "media/maps/marketing_normal.png", _Hide )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_REKLAME )
	
	--get loan
	btnReklame = button.EqualeTexture( 534, 255, "reklameCampanies", mainWindow, -1, "", _ShowCampaniesManager )
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

function ApplyNewWork( mp )
	local parent = base.CLuaElement( mp ).parent
	local parentCompany = applic:GetCompanyByName( currentWork.companyName )
	
	plant:AddReklameWork( currentWork )
	addingDays = 0
	localUpdateLabels()
	
	--снимем деньги со счета компании для проведения рекламной акции
	parentCompany:AddBalance( -1 * currentWork:GetDayCost() * currentWork.numberDay )
	
	base.CLuaElement( parent ):Remove()
end

