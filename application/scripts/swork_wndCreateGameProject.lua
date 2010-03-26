local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local project = CLuaGameProject( nil )
local applic = CLuaApplication( NrpGetApplication() )

local platform = { "pc", "console", "gamebox", "mobile" }
local lang = { "Eng", "Den", "France", "Russian", "Jap" }
local pagesName = {  }
local pagesID = { }
local pages = { }

pagesName[ "name" ] = "Название"; pagesID[ "name" ] = 9000 
pagesName[ "genre" ] = "Жанр"; pagesID[ "genre" ] = 9001
pagesName[ "scenario" ] = "Сценарий"; pagesID[ "scenario" ] = 9002
pagesName[ "advfunc" ] = "Доп.Возможности"; pagesID[ "advfunc" ] = 9003
pagesName[ "graphics" ] = "Графика"; pagesID[ "graphics" ] = 9004
pagesName[ "sound" ] = "Звук"; pagesID[ "sound" ] = 9005
pagesName[ "platforms" ] = "Платформы"; pagesID[ "platforms" ] = 9006
pagesName[ "end" ] = "Завершить"; pagesID[ "end" ] = 9007

local width = 800
local height = 600

local ID_CODEVOLUME = 9010

local function ShowParams()
	
	local label = CLuaLabel( guienv:GetElementByID( ID_CODEVOLUME ) )
	local codeVol = project:GetCodeVolume()
	
	label:SetText( "Код:" .. codeVol )
	
end

local function ShowAvaibleEngines( tab )

	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxEngine = company:GetEnginesNumber()
	
	if maxEngine > 0 then
		for i=0, maxEngine-1 do
			local  eg = CLuaGameEngine( company:GetEngine( i ) )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:AvaibleEngines  " .. maxEngine )
			
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x1 " .. width / 2 + 10 + 50 * i / 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y1 " .. 50 * i % 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x2 " .. width / 2 + 10 + 50 + 50 * i / 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y2 " ..  50 * i % 2 + 50 )
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Движок ".. i+1 .."/"..maxEngine .. "\r(" .. eg:GetName() .. ")", 
																width / 2 + 10 + 50 * i / 2, 50 * i % 2, 
																width / 2 + 10 + 50 + 50 * i / 2, 50 * i % 2 + 50, 
																-1, tab ) )
			linkModule:SetData( eg:Self() )
			linkModule:SetModuleType( PT_GAMEENGINE )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
		end
	end
end

local function ShowAvaibleGenreModules( tab )

	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyTech = company:GetTechNumber()
	local showedTech = 0
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleGenreModules  company = " .. maxCompanyTech )
	if maxCompanyTech > 0 then
		for i=0, maxCompanyTech-1 do
			local tech = CLuaTech( company:GetTech( i ) )
			
			if tech:GetTechGroup() == PT_GENRE then
				local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 10 + i * 50, width / 2 + 50, 10 + 50 + i * 50, -1, tab ) )
				linkModule:SetModuleType( PT_GENRE )
				linkModule:SetData( tech:Self() )		
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
				
				showedTech = showedTech + 1
			end
		end
	end
	
	local maxPublicTech = applic:GetTechNumber()
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleGenreModules  public = " .. maxPublicTech )
	
	if maxPublicTech > 0 then
		for i=0, maxPublicTech-1 do
			local tech = CLuaTech( applic:GetTech( i ) )
			
			if tech:GetTechGroup() == PT_GENRE then
				local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 200 + (i + showedTech) * 50, 
																				   width / 2 + 50, 200 + 50 + (i + showedTech) * 50, -1, tab ) )
				linkModule:SetModuleType( PT_GENRE )
				linkModule:SetData( tech:Self() )		
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
			end
		end	
	end
end

local function CreateGenrePage( tab )
	
	ShowAvaibleGenreModules( tab )
	
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxModuleNumber = project:GetGenreModuleNumber()

	if maxModuleNumber > 0 then
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Жанр", 10, 50, 10 + 50, 50 + 50, 9100, tab ) )
		linkModule:SetModuleType( PT_GENRE )
		linkModule:SetData( project:GetGenre( 0 ) )
		linkModule:SetDraggable( false )
		linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameProjectWizzardSetGenre" )

		for i=1, maxModuleNumber-1 do
			linkModule:SetObject( guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber, 10, 200 + i * 50, 10 + 50, 200 + 50 + i * 50, 9100+i, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetDraggable( false )
			linkModule:SetData( project:GetGenre( i ) )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameProjectWizzardSetGenre" )
		end
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

local function sworkRecreatePagesDependedEngine()
	
	local elm = CLuaElement( pages[ "genre" ] )
	elm:RemoveChilds()
	CreateGenrePage( elm:Self() )
	
	elm:SetObject( pages[ "graphics" ] )
	elm:RemoveChilds()
	CreateVideoContentPage( elm:Self() )
	
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
			
			sworkRecreatePagesDependedEngine()
		end
	end
	
	ShowParams()
	
end

function sworkGameProjectWizzardSetGenre( ptr )

	local recv = CLuaLinkBox( ptr )
	local id = recv:GetID() - 9100
	local dragObj = CLuaElement( guienv:GetDragObject() )
	
	if dragObj:Empty() == 0 and dragObj:GetTypeName() == "CNrpGuiLinkBox" then
		local sendLink = CLuaLinkBox( dragObj:Self() )
		
		if sendLink:GetModuleType() == PT_GENRE then
			project:SetGenre( sendLink:GetData(), id )
			recv:SetData( sendLink:GetData() )
			recv:SetText( sendLink:GetText() )
			sendLink:Remove()
			
			sworkRecreatePagesDependedGenres()
		end
	end
	
	ShowParams()
	
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

	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Сценарий", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCENARIO )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Лицензия", 60, 100, 60 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_LICENSE )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )

end

local function CreateAdvContentPage( tab )

	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Скрипты", 10, 150, 10 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCRIPTS )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Миниигры", 60, 150, 60 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_MINIGAME )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Физика", 110, 150, 110 + 50, 150 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SCRIPTS )
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
	project:SetObject( applic:CreateGameProject( "defaultGame" ) )
	windowg:SetObject( guienv:AddWindow( "GameWizzard", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_GAME_WIZZARD )
	
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 10, 20, 10 + 140, 20 + 20, -1 ) )
	local volCodeLabel = CLuaLabel( guienv:AddLabel( "Код", width / 2, 20, width, 20 + 20, ID_CODEVOLUME, windowg:Self() ) )
	
	local tabContol = guienv:AddTabControl( 10, 40, 790, 590, -1, windowg:Self() )
	pages[ "name" ] = guienv:AddTab( tabContol, pagesName[ "name" ], pagesID[ "name" ] ) --name
	CreateGameNamePage( pages[ "name" ] )
	
	pages[ "genre" ] = guienv:AddTab( tabContol, pagesName[ "genre" ], pagesID[ "genre" ] ) --genre and module
	CreateGenrePage( pages[ "genre" ] )
	
	pages[ "scenario" ] = guienv:AddTab( tabContol, pagesName[ "scenario"  ], pagesID[ "scenario"  ] ) -- scenario and license
	CreateScenarioLicensePage( pages[ "scenario" ] )
	
	pages[ "advfunc" ] = guienv:AddTab( tabContol, pagesName[ "advfunc" ], pagesID[ "advfunc" ] ) --adv function
	CreateAdvContentPage( pages[ "advfunc" ] )
	
	pages[ "graphics" ] = guienv:AddTab( tabContol, pagesName[ "graphics"  ], pagesID[ "graphics"  ] ) --graphics
	CreateVideoContentPage( pages[ "graphics" ] )
	
	pages[ "sound" ] = guienv:AddTab( tabContol, pagesName[ "sound" ], pagesID[ "sound" ] ) --sound
	CreateSoundContentPage( pages[ "sound" ] )
	
	pages[ "platforms" ] = guienv:AddTab( tabContol, pagesName[ "platforms" ], pagesID[ "platforms" ] ) --platforms
	CreatePlatformLangPage( pages[ "platforms" ] )
	
	pages[ "end" ] = guienv:AddTab( tabContol, pagesName[ "end" ], pagesID[ "end" ] ) --end
	CreateEndPage( pages[ "end" ] )
	
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
