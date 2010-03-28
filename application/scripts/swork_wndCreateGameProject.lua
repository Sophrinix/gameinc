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
			local gameeng = CLuaGameEngine( company:GetEngine( i ) )
				
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:AvaibleEngines  " .. maxEngine )
				
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x1 " .. width / 2 + 10 + 50 * i / 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y1 " .. 50 * i % 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:x2 " .. width / 2 + 10 + 50 + 50 * i / 2 )
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:y2 " ..  50 * i % 2 + 50 )
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Движок ".. i+1 .."/"..maxEngine .. "\r(" .. gameeng:GetName() .. ")", 
																width / 2 + 10 + 50 * i / 2, 50 * i % 2, 
																width / 2 + 10 + 50 + 50 * i / 2, 50 * i % 2 + 50, 
																-1, tab ) )
			linkModule:SetData( gameeng:Self() )
			linkModule:SetModuleType( PT_GAMEENGINE )
			linkModule:SetDraggable( true )
			linkModule:SetEnabled( not project:IsMyGameEngine( gameeng:Self() ) )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
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
					local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 10 + showedTech * 50, 
																					   width / 2 + 50, 10 + 50 + showedTech * 50, -1, tab ) )
					linkModule:SetModuleType( PT_GENRE )
					linkModule:SetData( tech:Self() )		
					linkModule:SetEnabled( not project:IsGenreIncluded( tech:GetTechType() ) ) 
					linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
					linkModule:SetDraggable( true )
					showedTech = showedTech + 1
			end
		end
	end
	
	local maxPublicTech = applic:GetTechNumber()
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleGenreModules public = " .. maxPublicTech )
	
	if maxPublicTech > 0 then
		for i=0, maxPublicTech-1 do
			local tech = CLuaTech( applic:GetTech( i ) )
			
			if tech:GetTechGroup() == PT_GENRE then
				local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 200 + showedTech * 50, 
																				   width / 2 + 50, 200 + 50 + showedTech * 50, -1, tab ) )
				linkModule:SetModuleType( PT_GENRE )
				linkModule:SetData( tech:Self() )
				linkModule:SetEnabled( not project:IsGenreIncluded( tech:GetTechType() ) )
				linkModule:SetDraggable( true )
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showedTech = showedTech + 1
			end
		end	
	end
end

local function CreateGenrePage( tab )
	ShowAvaibleGenreModules( tab )
	
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxModuleNumber = project:GetGenreModuleNumber()

	if maxModuleNumber > 0 then
		local genre = CLuaTech( project:GetGenre( 0 ) )
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Жанр", 10, 50, 10 + 50, 50 + 50, 9100, tab ) )
		linkModule:SetModuleType( PT_GENRE )
		linkModule:SetData( genre:Self() )
		linkModule:SetDraggable( false )
		
		if genre:Empty() == 0 then
			linkModule:SetText( genre:GetName() )
		end
		linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		linkModule:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetGenre" )
		linkModule:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )

		for i=1, maxModuleNumber-1 do
			genre:SetObject( project:GetGenre( i ) )
			linkModule:SetObject( guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber, 10, 200 + i * 50, 10 + 50, 200 + 50 + i * 50, 9100+i, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetDraggable( false )
			
			if genre:Empty() == 0 then
				linkModule:SetData( genre:GetName() )
			end
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
			linkModule:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetGenre" )
			linkModule:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
		end
	else
		
	end
end

local function CreateVideoContentPage( tab )
	
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Уровень графики", 10, 10, 10 + 50, 10 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GRAPHICLEVEL )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
	linkModule:SetObject( guienv:AddLinkBox( "Технология 1/2", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_VIDEOTECH )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkLeftMouseButtonDown" )
	
end

local function CreateGameNamePage( tab )
	local edit = CLuaEdit( guienv:AddEdit(  "Название игры",
				 						    10, 20, 10 + 180, 20 + 20,
											-1,
											tab ) )
	edit:SetName( WNDGMWIZ_NAME_EDIT )

	local ge = CLuaGameEngine( project:GetGameEngine() )
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Игровой движок", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GAMEENGINE )
	linkModule:SetData( ge:Self() )
	linkModule:SetDraggable( false )
	if ge:Empty() == 0 then
		linkModule:SetText( ge:GetName() )
	end
	linkModule:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkModule:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetVideoEngine" )
	
	ShowAvaibleEngines( tab )
	
	linkModule:SetObject( guienv:AddLinkBox( "Продолжение", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_GAME )
	linkModule:SetDraggable( false )
	--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_DOWN, "sworkGameProjectWizSetPrevGame" )
end

local function CreateEndPage( tab )
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Игровой движок", 10, 50, 10 + 50, 50 + 50, -1, tab ) )
	linkModule:SetData( project:GetGameEngine() )
	
	linkModule:SetObject( guienv:AddLinkBox( "Жанр", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetData( project:GetGenre( 0 ) )
	
	linkModule:SetObject( guienv:AddLinkBox( "Контент", 10, 150, 10 + 50, 150 + 50, -1, tab ) )
	linkModule:SetData( project:GetLicense() )	
	
	linkModule:SetObject( guienv:AddLinkBox( "Платформы", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	linkModule:SetData( project:GetPlatformsNumber() )
	
	linkModule:SetObject( guienv:AddLinkBox( "Локализация", 10, 250, 10 + 50, 250 + 50, -1, tab ) )
	linkModule:SetData( project:GetLanguagesNumber() )
	
	local button = CLuaButton( guienv:AddButton( 10, 10, 10 + 50, 10 + 50, tab, -1, "Завершить" ) )
	button:SetAction( "sworkCreateGameProject" )	
	button:SetEnabled( project:IsProjectReady() )
end

local function sworkRecreatePagesDependedEngine()
	local elm = CLuaElement( pages[ "genre" ] )
	elm:RemoveChilds()
	CreateGenrePage( elm:Self() )
	
	elm:SetObject( pages[ "graphics" ] )
	elm:RemoveChilds()
	CreateVideoContentPage( elm:Self() )
	
	elm:SetObject( pages[ "name" ] )
	elm:RemoveChilds()
	CreateGameNamePage( elm:Self() )
	
	elm:SetObject( pages[ "end" ] )
	elm:RemoveChilds()
	CreateEndPage( elm:Self() )
end

function sworkGameProjectWizzardSetVideoEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	local ge = CLuaGameEngine( sender:GetData() )
	project:SetGameEngine( sender:GetData() )
	sender:SetText( ge:GetName() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetScriptEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	local tech = CLuaTech( sender:GetData() )
	project:SetScriptEngine( tech:Self() )
	sender:SetText( tech:GetName() )
	ShowParams()
end

function sworkGameProjectWizzardSetMiniGameEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	local tech = CLuaTech( sender:GetData() )
	project:SetMiniGameEngine( tech:Self() )
	sender:SetText( tech:GetName() )
	ShowParams()
end

function sworkGameProjectWizzardSetPhysicEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	local tech = CLuaTech( sender:GetData() )
	project:SetPhysicEngine( tech:Self() )
	sender:SetText( tech:GetName() )
	ShowParams()
end

function sworkGameProjectWizzardSetGenre( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9100
	
	local tech = CLuaTech( sender:GetData() )
	project:SetGenre( sender:GetData(), id )
	sender:SetText( tech:GetName() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

local function ShowAvaibleScenarioAndLicense( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyScenario = company:GetTechNumber()
	local showedLinks = 0
	
	if maxCompanyScenario > 0 then
		for i=0, maxCompanyScenario-1 do
			local scenario = CLuaTech( company:GetTech( i ) )
			
			if scenario:GetTechGroup() == PT_SCENARIO then
					local linkModule = CLuaLinkBox( guienv:AddLinkBox( scenario:GetName(), width / 2, 10 + showedLinks * 50, 
																						   width / 2 + 50, 10 + 50 + showedLinks * 50, -1, tab ) )
					linkModule:SetModuleType( PT_SCENARIO )
					linkModule:SetData( scenario:Self() )		
					linkModule:SetEnabled( not project:IsScenarioIncluded( scenario:GetName() ) ) 
					linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
					linkModule:SetDraggable( true )
					showedLinks = showedLinks + 1
			end
		end
	end
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:ShowAvaibleScenarioAndLicense  company scenario = " .. showedLinks )
	
	local licenseNumber = 0
	if maxCompanyScenario > 0 then
		for i=0, maxCompanyScenario-1 do
			local license = CLuaTech( company:GetTech( i ) )
			
			if license:GetTechGroup() == PT_LICENSE then
					local linkModule = CLuaLinkBox( guienv:AddLinkBox( license:GetName(), width / 2, 10 + showedLinks * 50,
																						   width / 2 + 50, 10 + 50 + showedLinks * 50, -1, tab ) )
					linkModule:SetModuleType( PT_LICENSE )
					linkModule:SetData( license:Self() )		
					linkModule:SetEnabled( not project:IsLicenseIncluded( license:GetName() ) ) 
					linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
					linkModule:SetDraggable( true )
					showedLinks = showedLinks + 1
					licenseNumber = licenseNumber + 1
			end
		end
	end
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:ShowAvaibleScenarioAndLicense company license = " .. licenseNumber )
end

local function CreateScenarioLicensePage( tab )
	ShowAvaibleScenarioAndLicense( tab )
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = CLuaTech( project:GetScenario() )
	local linkScenario = CLuaLinkBox( guienv:AddLinkBox( "Сценарий", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkScenario:SetModuleType( PT_SCENARIO )
	linkScenario:SetData( tech:Self() )
	
	if tech:Empty() == 0 then
		linkScenatio:SetText( tech:GetName() )
	end
	linkScenario:SetDraggable( false )
	linkScenario:SetVisible( not project:HaveLicense() )
	linkScenario:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkScenario:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkScenario:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetScenario" )
	
	local lic = CLuaTech( project:GetLicense() )
	local linkLicense = CLuaLinkBox( guienv:AddLinkBox( "Лицензия", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	linkLicense:SetModuleType( PT_LICENSE )
	linkLicense:SetData( lic:Self() )
	
	if lic:Empty() == 0 then
		linkLicense:SetText( lic:GetName() )
	end
	linkLicense:SetDraggable( false )
	linkLicense:SetVisible( not project:HaveScenario() )
	linkLicense:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkLicense:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkLicense:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetLicense" )
end

local function ShowAvaibleScriptAndMiniGames( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyScriptTech = company:GetTechNumber()
	local showeddLinks = 0
	
	if maxCompanyScriptTech > 0 then
		for i=0, maxCompanyScriptTech-1 do
			local tech = CLuaTech( company:GetTech( i ) )
			
			local tg = tech:GetTechGroup()
			if  tg == PT_SCRIPTS or tg == PT_MINIGAME or tg == PT_PHYSIC or tg == PT_ADVTECH then
					local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 10 + showeddLinks * 50, 
																					   width / 2 + 50, 10 + 50 + showeddLinks * 50, -1, tab ) )
					linkModule:SetModuleType( tg )
					linkModule:SetData( tech:Self() )		
					linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
					linkModule:SetDraggable( true )
					showeddLinks = showeddLinks + 1
			end
		end
	end
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:ShowAvaibleScriptLevel company script = " .. showeddLinks )
	
	local maxPublicTech = applic:GetTechNumber()
	if maxPublicTech > 0 then
		for i=0, maxPublicTech-1 do
			local tech = CLuaTech( applic:GetTech( i ) )
			
			local tg = tech:GetTechGroup()
			if tg == PT_SCRIPTS or tg == PT_MINIGAME or tg == PT_PHYSIC or tg == PT_ADVTECH then
				Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleScriptLevel element = " .. i .. " " .. 20 + showeddLinks * 50 )
				local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 20 + showeddLinks * 50, 
																				   width / 2 + 50, 20 + 50 + showeddLinks * 50, -1, tab ) )
				linkModule:SetModuleType( tg )
				linkModule:SetData( tech:Self() )
				linkModule:SetDraggable( true )
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showeddLinks = showeddLinks + 1
			end
		end	
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

local function CreateAdvContentPage( tab )
	ShowAvaibleScriptAndMiniGames( tab )
	
	local se = CLuaTech(  project:GetScriptEngine() )
	local linkScript = CLuaLinkBox( guienv:AddLinkBox( "Скрипты", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkScript:SetModuleType( PT_SCRIPTS )
	linkScript:SetDraggable( false )
	linkScript:SetData( se:Self() )
	if se:Empty() == 0 then linkScript:SetText( se:GetName() )	end
	linkScript:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkScript:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkScript:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetScriptEngine" )
	
	local mg = CLuaTech( project:GetMiniGameEngine() )
	local linkMiniGames = CLuaLinkBox( guienv:AddLinkBox( "Миниигры", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	linkMiniGames:SetModuleType( PT_MINIGAME )
	linkMiniGames:SetDraggable( false )
	linkMiniGames:SetData( mg:Self() )
	if mg:Empty() == 0 then	linkMiniGames:SetText( mg:GetName() ) end
	linkMiniGames:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkMiniGames:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkMiniGames:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetMiniGameEngine" )

	local ph = CLuaTech( project:GetPhysicEngine() )
	local linkPhis = CLuaLinkBox( guienv:AddLinkBox( "Физика", 10, 300, 10 + 50, 300 + 50, -1, tab ) )
	linkPhis:SetModuleType( PT_PHYSIC )
	linkPhis:SetDraggable( false )
	linkPhis:SetData( mg:Self() )
	if ph:Empty() == 0 then linkPhis:SetText( ph:GetName() ) end
	linkPhis:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	linkPhis:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	linkPhis:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetPhysicEngine" )
	
	local showeddLinks = 0
	local xoffset = 70
	local maxProjectAdvTech = project:GetAdvTechNumber()
	for i=0, maxProjectAdvTech do
		local tech = CLuaTech( project:GetAdvTech( i ) )
		local linkAdv = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), xoffset, 20 + showeddLinks * 50, 
																		xoffset + 50, 20 + 50 + showeddLinks * 50, 9200 + i, tab ) )

		linkAdv:SetModuleType( PT_ADVTECH )
		linkAdv:SetData( tech:Self() )
		linkAdv:SetDraggable( true )
		linkAdv:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
		linkAdv:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		linkAdv:AddLuaFunction( GUIELEMENT_SET_DATA, "sworkGameProjectWizzardSetAdvTech" )
		showeddLinks = showeddLinks + 1
		
		if showeddLinks * 50 > 450 then
			showeddLinks = 0
			xoffset = xoffset + 70
		end
	end
end

function sworkGameProjectWizzardSetAdvTech( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9200
	local maxProjectAdvTech = project:GetAdvTechNumber()

	if id == maxProjectAdvTech then
		
	else
	
	end
	local tech = CLuaTech( sender:GetData() )
	project:SetGenre( sender:GetData(), id )
	sender:SetText( tech:GetName() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
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
	
	windowg:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkWindowLeftMouseButtonUp" )
end

function sworkWindowLeftMouseButtonUp( ptr )
	guienv:SetDragObject( nil )
end

function sworkRigthMouseButtonUp( ptr )
	local link = CLuaLinkBox( ptr ) 
	link:SetData( nil )
end

function sworkLeftMouseButtonUp( ptr )
	local elm = CLuaElement( ptr )
	
	if elm:Empty() == 0 and elm:GetTypeName() == "CNrpGuiLinkBox" then
		local link = CLuaLinkBox( elm:Self() ) 
		local linkRecv = CLuaLinkBox( guienv:GetDragObject() )
		
		if link:IsDraggable() then
			if link:IsEnabled() then
				guienv:SetDragObject( link:Self() )
			end
		else
			if link:GetModuleType() == linkRecv:GetModuleType() then
				link:SetData( linkRecv:GetData() )
			end
			
			guienv:SetDragObject( nil )
		end
	end
end