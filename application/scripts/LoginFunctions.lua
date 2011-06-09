--Подключаем скрипт создания стартовых условий новой игры
IncludeScript("newGame")
IncludeScript("updates")
IncludeScript("button")

local base = _G

module( "LoginFunctions" )

local guienv = base.guienv
local applic = base.applic
local button = base.button
local updates = base.updates
local sceneManager = base.sceneManager
--переменные для работы
local plant = base.applic.plant

--размеры окна
local width = 200
local height = 100

local editName = nil
local editCompany = nil
local wndNewGame = nil

local function CloseNewGame( ptr )
	wndNewGame:Remove()
end

function About()
	base.browser:Show()
	base.browser:Navigate( "media/html/intro.htm" )
end

function ShowOptions()

end

local function localPlantLoadBaseReklame()
	base.updates.CheckNewReklames()
	plant:LoadReklamePrice( applic.profile )
end

--создание новой игры
function CreateNewProfileAndStartGame( ptr )
	--создаем новый профиль
	applic:CreateProfile( editName.text, editCompany.text )
	
	--убираем данные из памяти
	base.newGame.ResetData()
	
	applic:LoadScreenshots( updates.fileScreenshots )
	sceneManager:DrawProgress( 5, "Ищем изображения" )

	updates.CheckNewTechs()
	sceneManager:DrawProgress( 10, "Обновляем технологии" )

	updates.CheckLanguages()
	sceneManager:DrawProgress( 15, "Обновляем языки" )

	updates.CheckGameBoxAddons()
	sceneManager:DrawProgress( 20, "Ищем аддоны к коробкам" )

	updates.CheckPlatforms( false )
	sceneManager:DrawProgress( 26, "Ищем новые платформы" )

	updates.CheckDiskMachines()
	sceneManager:DrawProgress( 30, "Обновляем данные о роизводстве" )

	updates.CheckNewReklames()
	sceneManager:DrawProgress( 35, "Ищем рекламу" )

	updates.CheckNewGames()
	sceneManager:DrawProgress( 40, "Обновляем базу данных по играм" )

	updates.CheckNewCompanies()
	sceneManager:DrawProgress( 45, "Создаем компании" )
	
	--создание рабочих
	applic:CreateNewFreeUsers()
	sceneManager:DrawProgress( 50, "Загружаем новых пользователей" )
	
	--загружаем базовую рекламу
	localPlantLoadBaseReklame( false )
	sceneManager:DrawProgress( 55, "Создаем базовую рекламу" )
	
	--переходим на следующую сцену
	base.NrpSetNextScene( "sceneWork" )
end

function NewGame( ptr )
	wndNewGame = guienv:AddWindow( "", "25%", "33%", "50%+", "33%+", -1, guienv.root )
	wndNewGame.closeButton.visible = false
	
	editName = guienv:AddEdit(  "Player",  "10%", 40, "80%+", "20+", -1, wndNewGame )
	editCompany = guienv:AddEdit(  "CompanyName", "10%", 70, "80%+", "20+", -1, wndNewGame )				
	
	button.Stretch( "12%", "60e", "49%", "10e", "ok", wndNewGame, -1, "Готово", CreateNewProfileAndStartGame )
	button.Stretch( "51%", "60e", "88%", "10e", "cancel", wndNewGame, -1, "Отмена", CloseNewGame )
end

function LoadCity()
	base.NrpSetNextScene( "sceneWork" )
end

function Continue( ptr )
	local lastProfile = applic.profile
	
	local dd = base.io.open( "save/"..lastProfile.."/profile.ini", "r" )
	if dd == nil then
		guienv:MessageBox( "Нет сохраненных игр", false, false, button.CloseParent, button.NoFunction )
		return 0
	end
	--устанавливаем текущее времся
	applic:LoadGameTimeFromProfile( applic.profile )
	sceneManager:DrawProgress( 1, "Подгружаем данные из сохранения" )
	
	--загружаем скриншоты для игр
	applic:LoadScreenshots( updates.fileScreenshots )
	sceneManager:DrawProgress( 4, "Обновляем данные для игры" )
	
	--загружаем линки
	updates.LoadLinks()
	sceneManager:DrawProgress( 8, "Создаем ресурсы" )
	
	--загружаем текущие аддоны для коробки для текущего времени
	updates.CheckGameBoxAddons()
	sceneManager:DrawProgress( 20, "Ищем аддоны к коробкам" )

	applic:LoadBoxAddonsPrice()
	sceneManager:DrawProgress( 30, "Устанавливаем цены на аддоны" )
	
	--загружаем профиль
	applic:LoadProfile( applic.profile, applic:GetCurrentProfileCompany() )
	sceneManager:DrawProgress( 10, "Создам профиль игрока" )
	
	applic.pda:Load()
	sceneManager:DrawProgress( 40, "Загружаем историю" )
		
	--загружаем аппараты по производству дисков
	updates.CheckDiskMachines()
	sceneManager:DrawProgress( 50, "Ищем новые мощности производства" )
	
	--загружаем производство и рекламу
	plant:Load()
	sceneManager:DrawProgress( 60, "Загружаем данные производства" )

	--загружаем базовую рекламу
	localPlantLoadBaseReklame( false )
	sceneManager:DrawProgress( 70, "Загружаем базовую рекламу" )
	
	guienv:FadeAction( 1000, FADE_OUT, REMOVE_ON_END )
	guienv:AddTimer( 1010, LoadCity, nil );
end

function Quit()
	base.NrpApplicationClose()
end