local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local project = CLuaGameProject( nil )
local applic = CLuaApplication( NrpGetApplication() )

local platform = { "pc", "console", "gamebox", "mobile" }
local lang = { "Eng", "Den", "France", "Russian", "Jap" }

local width = 800
local height = 600

local function ShowAvaibleEngines( tab )

	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxEngine = company:GetEnginesNumber()
	
	for i=0, maxEngine-1 do
		local  eg = CLuaGameEngine( company:GetEngine( i ) )
	    Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:AvaibleEngines  " .. maxEngine )
		
		Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x1 " .. width / 2 + 10 + 50 * i / 2 )
		Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y1 " .. 50 * i % 2 )
		Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x2 " .. width / 2 + 10 + 50 + 50 * i / 2 )
		Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y2 " ..  50 * i % 2 + 50 )
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Движок ".. i+1 .."/"..maxEngine .. "(" .. eg:GetName() .. ")", 
															width / 2 + 10 + 50 * i / 2, 50 * i % 2, 
															width / 2 + 10 + 50 + 50 * i / 2, 50 * i % 2 + 50, 
															-1, tab ) )
		linkModule:SetData( eg:Self() )
		linkModule:SetModuleType( PT_GAMEENGINE )
		linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	end

end

function sworkGameProjectWizzardSetVideoEngine( ptr )

	local recv = CLuaLinkBox( ptr )
	local dragObj = CLuaElement( guienv:GetDragObject() )
	
	if dragObj:Empty() == 0 and dragObj:GetTypeName() == "CNrpGuiLinkBox" then
	
		local sendLink = CLuaLinkBox( dragObj:Self() )
		
		if sendLink:GetModuleType() == PT_GAMEENGINE then
			project:SetGameEngine( sendLink:GetData() )
			recv:SetData( sendLink:GetData() )
			recv:SetText( sendLink:GetText() )
			sendLink:Remove()
		end
		
	end
	
end

local function CreateGameNamePage( tab )

	local edit = CLuaEdit( guienv:AddEdit(  "Название игры",
				 						    10, 20, 10 + 180, 20 + 20,
											-1,
											tab ) )
	edit:SetName( WNDGMWIZ_NAME_EDIT )

	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Игровой движок", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GAMEENGINE )
	linkModule:SetDraggable( false )
	linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameProjectWizzardSetVideoEngine" )
	
	ShowAvaibleEngines( tab )
	
	linkModule:SetObject( guienv:AddLinkBox( "Продолжение", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GAME )
	linkModule:SetDraggable( false )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkGameProjectWizSetPrevGame" )
	
end

local function CreateScenarioLicensePage( tab )

	local linkModule = CLuaLinBox( guienv:AddLinkBox( "Сценарий", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCENARIO )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Лицензия", 60, 100, 60 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_LICENSE )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )

end

local function CreateAdvContentPage( tab )

	linkModule:SetObject( guienv:AddLinkBox( "Скрипты", 10, 150, 10 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCRIPTS )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Миниигры", 60, 150, 60 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_MINIGAME )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Физика", 110, 150, 110 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCRIPTS )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
end

local function ShowAvaibleGenreModules( tab )

	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyTech = company:GetTechCounter()
	
	for i=0, maxCompanyTech do
		local tech = CLuaTech( company:GetTech( i ) )
		
		if tech:GetTechType() == PT_GENRE then
			local linkModule = CLuaTech( guienv:AddLinkBox( tech:GetName(), 10, 10 + i * 50, 10 + 50, 10 + 50 + i * 50, -1, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetData( tech:Self() )		
		end
	end
	
	local maxPublicTech = app:GetTechCounter()
	
	for i=0, maxPublicTech do
		local tech = CLuaTech( app:GetTech( i ) )
		
		if tech:GetTechType() == PT_GENRE then
			local linkModule = CLuaTech( guienv:AddLinkBox( tech:GetName(), 10, 200 + i * 50, 10 + 50, 200 + 50 + i * 50, -1, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetData( tech:Self() )		
		end
	end	
	
end

local function CreateGenrePage( tab )
	
	ShowAvaibleGenreModules( tab )
	
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxModuleNumber = project:GetGenreModuleNumber()

	if maxModuleNumber > 0 then
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Жанр", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
		linkModule:SetModuleType( PT_GENRE )
		linkModule:SetData( project:GetGenre( 0 ) )
		linkModule:SetDraggable( false )

		for i=1, maxModuleNumber-1 do
			linkModule:SetObject( guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber, 10, 200 + i * 50, 10 + 50, 200 + 50 + i * 50, -1, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetDraggable( false )
			linkModule:SetData( project:GetGenre( i ) )
		end
		
		--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	else
		
	end
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
end

local function CreateVideoContentPage( tab )
	
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Уровень графики", 10, 10, 10 + 50, 10 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GRAPHICLEVEL )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Технология 1/2", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_VIDEOTECH )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
end

local function CreateSoundContentPage( tab )

	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Звуковое оформление", 10, 10, 10 + 50, 10 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SOUNDLEVEL )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )

	linkModule:SetObject( guienv:AddLinkBox( "Технология 1/2", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SOUNDTECH )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
end

local function CreatePlatformLangPage( tab )

	for i=1, #lang do
		local button = CLuaButton( guienv:AddButton( 10 + 40 * i, 10, 10 + 50 + 40 * i, 10 + 50, tab, -1, lang[ i ] ) )
		button:SetAction( "sworkSetLang" )
	end

	for i=1, #platform do
		local button = CLuaButton( guienv:AddButton( 10 + 40 * i, 50, 10 + 50 + 40 * i, 50 + 50, tab, -1,  platform[ i ] ) )
		button:SetAction( "sworkSetPlatform" )
	end

end

local function CreateEndPage( tab )
	
	local button = CLuaButton( guienv:AddButton( 10, 10, 10 + 50, 10 + 50, tab, -1, "Завершить" ) )
	button:SetAction( "sworkCreateGameProjectt" )
	
end

function sworkCreateGameProject( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_PROJECTWIZ_NAME ) )
	windowg:Remove()
	
	local applic = CLuaApplication( NrpGetApplication() )
	local gameProject = applic:CreateGameProject( "defaultGame" )
	windowg:SetObject( guienv:AddWindow( "GameWizzard", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_GAME_WIZZARD )
	
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 10, 20, 10 + 140, 20 + 20, -1 ) )
	
	local tabContol = guienv:AddTabControl( 10, 40, 790, 590, -1, windowg:Self() )
	local tabPage = guienv:AddTab( tabContol, "Название", 1 )
	CreateGameNamePage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Жанр", 2 )
	CreateGenrePage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Сценарий", 3 )
	CreateScenarioLicensePage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Доп.Возможности", 4 )
	CreateAdvContentPage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Графика", 5 )
	CreateVideoContentPage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Звук", 6 )
	CreateSoundContentPage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Платформы", 7 )
	CreatePlatformLangPage( tabPage )
	
	tabPage = guienv:AddTab( tabContol, "Завершить", 8 )
	CreateEndPage( tabPage )
	
	--guienv:AddGameComponentTable( 10, height - 200, width - 10, height - 10, -1, windowg:Self() )
	
	windowg:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	windowg:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	
end

function sworkLeftMouseButtonDown( ptr )

	local elm = CLuaElement( ptr )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:sworkLeftMouseButtonDown  " .. elm:GetText() )
	if elm:Empty() == 0 and elm:GetTypeName() == "CNrpGuiLinkBox" then
		guienv:SetDragObject( elm:Self() )
	end
	
end

function sworkLeftMouseButtonUp( ptr )

	local elm = CLuaElement( ptr )
	
	guienv:SetDragObject( nil )
	
end
