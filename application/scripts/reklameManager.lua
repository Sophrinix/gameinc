local base = _G

IncludeScript( "reklameCampanies" )

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

local lableDayNumber = nil

local labelCostInDay = nil
local labelPrefFamous = nil
local labelGameFamous = nil
local labelPrice = nil
local labelName = nil

local addingDays = 0

local currentWork = nil
local realCampany = nil

local selectedGame = nil
local linkSelectedGame = nil

local reklames = {}

--переход на окно заказа кампаний
btnReklame = nil
--здесь отображаются типы рекламы
picflowReklames = nil
--здесь отображаются доступные игры и проекты компании
lbxGames = nil
--кнопки изменения длительности рекламной кампании
btnDecDayNumber = nil
btnIncDayNumber = nil
--кнопка заказа работы
btnApplyWork = nil

local function _GetGameFamous( value )
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

local function _UpdateLabels()
	labelName.text = currentWork.uniq
	local text = base.STR_DAYLEFT .. "  " .. realCampany.numberDay
	
	if addingDays > 0 then 
		text = text .. base.string.format( "( %s  %d )", base.STR_ADDDAYS, addingDays )
	end
	
	lableDayNumber.text = text
	labelPrice.text = base.STR_MONEYLEFT .. "   $" .. realCampany.price
	labelCostInDay.text = base.STR_DAYCOST .. "   $" .. currentWork.dayCost
	labelPrefFamous.text = base.STR_PREFFAMOUS .. "   " .. realCampany.famous .. "%"
	labelGameFamous.text = _GetGameFamous( selectedGame.famous ).."\n( dbg ".. selectedGame.famous .. " %)"
end

local function _FillGamesListBox()
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

local function _AddLabel( textr, x1, y1, x2, y2 )
	local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, campaniesWindow )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	label.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff )

	return label
end

local function _CreateReklames()
	for i=1, plant.reklameNumber do
		reklames[ i ] = plant:GetBaseReklame( i-1 )
	end
end

local function _Hide()
	base.package.loaded[ "reklameCampanies" ] = false
end

local function _IncDay()
	addingDays = addingDays + 10
	
	_UpdateLabels()
end

local function _DecDay()
	if addingDays - 10 >= 0 then
		addingDays = addingDays - 10
	end

	_UpdateLabels()
end

local function _SelectNewWork()
	currentWork = base.CLuaReklame( picflowReklames.selectedObject )
	selectedGame = base.CLuaDevelopProject( lbxGames.selectedObject )
	
	base.LogScript( "work = " ..currentWork.uniq .. "  game = ".. selectedGame.name )

	currentWork.reklameObject = selectedGame.object
	addingDays = 0	
	
	local vis = not selectedGame.empty
	btnApplyWork.visible = vis
	btnIncDayNumber.visible = vis
	btnDecDayNumber.visible = vis

	realCampany = plant:GetReklame( currentWork.uniq, selectedGame.name )
	_UpdateLabels()		
end

local function _ApplyNewWork( mp )
	local parent = base.CLuaElement( mp ).parent
	local parentCompany = applic:GetCompanyByName( currentWork.company )
	
	plant:AddReklameWork( currentWork )
	addingDays = 0
	_UpdateLabels()
	
	--снимем деньги со счета компании для проведения рекламной акции
	parentCompany:AddBalance( "Проведение рекламной кампании", -1 * currentWork.dayCost * currentWork.numberDay )
	
	base.CLuaElement( parent ):Remove()
end

local function _QuerryUserToApplyWork()
	if addingDays > 0 then
		currentWork.numberDay = addingDays
		local text = "Рекламная кампания:"..currentWork.name.."\n"
		text = text .. "Стоимость:" .. currentWork.dayCost * currentWork.numberDay .. "\n"
		text = text .. "Длительность:" .. currentWork.numberDay
		guienv:MessageBox( text, true, true, _ApplyNewWork, button.CloseParent )
		_UpdateLabels()
	else
		guienv:MessageBox( "Выберите количество дней кампании", false, false, nil, nil )
	end
end

local function _HideCampaniesWindow()

end

local function _ShowCampaniesManager()
	company = applic.playerCompany
	plant = applic.plant

	local txsBlur = base.driver:CreateBlur( "marketing.png", 2, 4 )
	campaniesWindow = window.fsWindow( txsBlur.path, _HideCampaniesWindow )	
	
	tutorial.Update( "reklame/campany" )
		
	if #reklames == 0 then _CreateReklames() end
	
	lbxGames = guienv:AddComponentListBox( "51%", "33%", "10e", "80e", -1, campaniesWindow )
	lbxGames.onChangeSelect = _SelectNewWork
	_FillGamesListBox()
		
	--блок рекламы на листовках
	picflowReklames = guienv:AddPictureFlow( "5%", "5%", "95%", "25%+", -1, campaniesWindow )
	picflowReklames.drawBody = false
	picflowReklames.onSelect = _SelectNewWork

	labelName = _AddLabel( "", 60, "30%", "45%", "40+" )
	lableDayNumber = _AddLabel( "", 60, "35%", "45%", "40+" )
	labelPrice = _AddLabel( "0", 60, "40%", "45%", "40+" ) 
	labelCostInDay = _AddLabel( "0", 60, "45%", "45%", "40+" ) 
	labelPrefFamous = _AddLabel( "0", 60, "50%", "45%", "40+" ) 
	labelGameFamous = _AddLabel( "0", 60, "55%", "45%", "40+" ) 
	
	--кнопки для изменения длительности кампании
	btnDecDayNumber = guienv:AddButton( "5%", "30%", "60+", "60+", campaniesWindow, -1, "-" )
	btnDecDayNumber.action = _DecDay
	btnDecDayNumber.visible = false
	
	btnIncDayNumber = guienv:AddButton( "40%", "30%", "60+", "60+", campaniesWindow, -1, "+" ) 										
	btnIncDayNumber.action = _IncDay
	btnIncDayNumber.visible = false
	
	--обновление данных для реклам
	guienv:AddLoopTimer( 1000, _UpdateLabels, campaniesWindow )
	
	for y=1, #reklames do
		picflowReklames:AddItem( reklames[ y ].texture, reklames[ y ].name, reklames[ y ].object )
	end

	btnApplyWork = guienv:AddButton( "12%", "70e", "26%", "20e", campaniesWindow, -1, base.STR_STARTREKLAME )
	btnApplyWork.action = _QuerryUserToApplyWork
	btnApplyWork.visible = false
	
	--выберем первую работу
	if lbxGames.count > 0 then
		lbxGames.itemIndex = 0
		_SelectNewWork()
	end
	
	--заказать статью в игровом журнале
	--список журналов
end

local function _ShowReklameCampanies()
	base.reklameCampanies.Show()
end

function ShowHelp()
	tutorial.Update( "reklame/main" )
end

function Show()
	mainWindow = window.fsWindow( "marketing.png", _Hide )
	
	base.rightPanel.AddYesNo( "Хотите больше узнать о рекламе?", ShowHelp, button.CloseParent )
	
	--get loan
	btnReklame = button.EqualeTexture( 534, 255, "reklameCampanies", mainWindow, -1, "", _ShowCampaniesManager )
	btnCampanies = button.EqualeTexture( 142, 457, "reklameCampanies", mainWindow, -1, "", _ShowReklameCampanies )
end

