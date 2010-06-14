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
local ID_PROJECTQUALITY = 9011
local ID_COMPONENTLIST = 9012

local function ShowParams()
	local label = CLuaLabel( guienv:GetElementByID( ID_CODEVOLUME ) )
	local prg = CLuaProgressBar( guienv:GetElementByID( ID_PROJECTQUALITY ) )
	
	label:SetText( "Код:" .. project:GetCodeVolume() )
	prg:SetPosition( project:GetCodeQuality() )
end

local function ShowAvaibleEngines( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxEngine = company:GetEnginesNumber()
	local xoffset = 0
	local rowCount = 0	
	
	for i=1, maxEngine do
		local gameeng = CLuaGameEngine( company:GetEngine( i-1 ) )			
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Движок ".. i .."/"..maxEngine .. "\r(" .. gameeng:GetName() .. ")", 
															width / 2 + xoffset, 50 * rowCount, 
															width / 2 + xoffset + 50, 50 * rowCount + 50, 
															-1, tab ) )
		linkModule:SetData( gameeng:Self() )
		linkModule:SetModuleType( PT_GAMEENGINE )
		linkModule:SetDraggable( true )
		linkModule:SetEnabled( not project:IsMyGameEngine( gameeng:Self() ) )
		linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		rowCount = rowCount + 1
		if rowCount * 50 > 450 then
			xoffset = 0
			rowCount = 0
		end
	end
end

local function ShowAvaibleGenreModules( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyTech = company:GetTechNumber()
	local maxPublicTech = applic:GetTechNumber()
	local arGenres = { }
	local showedTech = 0

	for i=1, maxCompanyTech do arGenres[ i ] = conmpany:GetGenre( i-1 )	end
	for i=1, maxPublicTech do arGenres[ maxCompanyTech + i ] = applic:GetTech( i-1 ) end

	Log({src=SCRIPT, dev=ODS|CON}, "ShowAvaibleGenreModules:#arGenres " .. #arGenres )
	for i=1, #arGenres do
		local tech = CLuaTech( arGenres[ i ] )
		
		if tech:GetTechGroup() == PT_GENRE then
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 200 + showedTech * 50, 
																			   width / 2 + 50, 200 + 50 + showedTech * 50, -1, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetData( tech:Self() )
			linkModule:SetEnabled( not project:IsGenreIncluded( tech:GetTechType() ) )
			linkModule:SetDraggable( true )
			linkModule:SetTexture( tech:GetTexture() )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
			showedTech = showedTech + 1
		end
	end	
end

local function SetLuaFuncToLinkBox( lb, funcName )
	lb:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	lb:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	lb:AddLuaFunction( GUIELEMENT_SET_DATA, funcName )
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
		SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetGenre" )
		if genre:Empty() == 0 then linkModule:SetText( genre:GetName() ) end


		for i=1, maxModuleNumber-1 do
			genre:SetObject( project:GetGenre( i ) )
			linkModule:SetObject( guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber, 
													 10, 200 + i * 50, 
													 10 + 50, 200 + 50 + i * 50, 
													 9100+i, tab ) )
			linkModule:SetModuleType( PT_GENRE )
			linkModule:SetDraggable( false )
			
			if genre:Empty() == 0 then	linkModule:SetData( genre:GetName() ) end
			SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetGenre" )
		end
	else
		
	end
end

local function ShowAvaibleVideoQualityAndVideoTech( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local showedTech = 0
	local techs = { }

	local maxCompanyTech = company:GetTechNumber()
	local maxPublicTech = applic:GetTechNumber()

	for i=1, maxCompanyTech do techs[ i ] = company:GetTech( i-1 ) end
	for i=1, maxPublicTech do	techs[ maxCompanyTech + i ] = applic:GetTech( i-1 ) end
	
	for i=1, #techs do
		local tech = CLuaTech( techs[ i ] )
		local tg = tech:GetTechGroup()
		if tg == PT_VIDEOTECH or tg == PT_VIDEOQUALITY then
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 200 + showedTech * 50, 
																			   width / 2 + 50, 200 + 50 + showedTech * 50, -1, tab ) )
			linkModule:SetModuleType( tg )
			linkModule:SetData( tech:Self() )
			linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) )
			linkModule:SetDraggable( true )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
			showedTech = showedTech + 1
		end
	end	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function ShowAvaibleSoundQualityAndSoundTech( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local showedTech = 0
	local techs = { }

	local maxCompanyTech = company:GetTechNumber()
	local maxPublicTech = applic:GetTechNumber()

	for i=1, maxCompanyTech do techs[ i ] = company:GetTech( i-1 ) end
	for i=1, maxPublicTech do	techs[ maxCompanyTech + i ] = applic:GetTech( i-1 ) end
	
	for i=1, #techs do
		local tech = CLuaTech( techs[ i ] )
		local tg = tech:GetTechGroup()
		if tg == PT_SOUNDTECH or tg == PT_SOUNDQUALITY then
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 200 + showedTech * 50, 
																			   width / 2 + 50, 200 + 50 + showedTech * 50, -1, tab ) )
			linkModule:SetModuleType( tg )
			linkModule:SetData( tech:Self() )
			linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) )
			linkModule:SetDraggable( true )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
			showedTech = showedTech + 1
		end
	end	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEVTP:ShowAvaibleSoundQualityAndSoundTech public = " .. showedTech )
end

local function CreateSoundContentPage( tab )

	ShowAvaibleSoundQualityAndSoundTech( tab )
	
	local sq = CLuaTech( project:GetSoundQuality() )
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Звуковое оформление", 10, 10, 10 + 50, 10 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_SOUNDQUALITY )
	linkModule:SetData( sq:Self() )
	linkModule:SetDraggable( false )
	if sq:Empty() == 0 then	linkModule:SetText( sq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetSoundQuality" )

	local xoffset = 70
	local maxProjectSoundTech = project:GetSoundTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectSoundTech do
		local tech = CLuaTech( project:GetSoundTech( i ) )
		local linkAdv = CLuaLinkBox( guienv:AddLinkBox( "Звук " .. i .. "/" .. maxProjectSoundTech, 
														xoffset, 60 + showeddLinks * 50, 
														xoffset + 50, 60 + 50 + showeddLinks * 50, 9500 + i, tab ) )

		linkAdv:SetModuleType( PT_SOUNDTECH )
		linkAdv:SetData( tech:Self() )
		linkAdv:SetDraggable( false )
		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetSoundTech" )
		showeddLinks = showeddLinks + 1
		
		if (60 + showeddLinks * 50) > 450 then
			showeddLinks = 0
			xoffset = xoffset + 70
		end
	end	
end

local function CreateVideoContentPage( tab )

	ShowAvaibleVideoQualityAndVideoTech( tab )
	
	local vq = CLuaTech( project:GetVideoQuality() )
	local linkModule = CLuaLinkBox( guienv:AddLinkBox( "Уровень графики", 10, 10, 10 + 50, 10 + 50, -1, tab ) )
	linkModule:SetModuleType( PT_VIDEOQUALITY )
	linkModule:SetData( vq:Self() )
	linkModule:SetDraggable( false )
	if vq:Empty() == 0 then	linkModule:SetText( vq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetVideoQuality" )
	
	local xoffset = 70
	local maxProjectVideoTech = project:GetVideoTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectVideoTech do
		local tech = CLuaTech( project:GetVideoTech( i ) )
		local linkAdv = CLuaLinkBox( guienv:AddLinkBox( "Видео " .. i .. "/" .. maxProjectVideoTech,
														xoffset, 60 + showeddLinks * 50, 
														xoffset + 50, 60 + 50 + showeddLinks * 50, 9400 + i, tab ) )

		linkAdv:SetModuleType( PT_VIDEOTECH )
		linkAdv:SetData( tech:Self() )
		linkAdv:SetDraggable( false )
		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetVideoTech" )
		showeddLinks = showeddLinks + 1
		
		if (60 + showeddLinks * 50) > 450 then
			showeddLinks = 0
			xoffset = xoffset + 70
		end
	end	
end

local function CreateGameNamePage( tab )
	ShowAvaibleEngines( tab )

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
	if ge:Empty() == 0 then	linkModule:SetText( ge:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetVideoEngine" )
	
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
	if project:HaveLicense() or project:HaveScenario() then linkModule:SetData( linkModule:Self() ) end
	
	linkModule:SetObject( guienv:AddLinkBox( "Платформы", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	if project:GetPlatformsNumber() > 0 then linkModule:SetData( linkModule:Self()  ) end
	
	linkModule:SetObject( guienv:AddLinkBox( "Локализация", 10, 250, 10 + 50, 250 + 50, -1, tab ) )
	if project:GetLanguagesNumber() > 0 then linkModule:SetData( linkModule:Self() ) end
	
	local button = CLuaButton( guienv:AddButton( 10, 10, 10 + 50, 10 + 50, tab, -1, "Завершить" ) )
	button:SetAction( "sworkCreateProjectGameToCompany" )	
	button:SetEnabled( project:IsProjectReady() )
end

function sworkCreateProjectGameToCompany( ptr )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local edit = CLuaEdit( guienv:GetElementByName( WNDGMWIZ_NAME_EDIT ) )
	project:SetName( edit:GetText() )
	
	local prj = CLuaGameProject( company:GetProjectByName( project:GetName() ) )
	
	if prj:Empty() == 1 then
		prj:SetObject( company:CreateGameProject( project:Self() ) )
		company:AddToPortfelle( prj:Self() )
		sworkUpdateCompanyPortfelle()
	else
		guienv:ShowMessage( "Уже есть проект с таким именем" )
	end
end

local function ShowAvaibleScriptAndMiniGames( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxCompanyScriptTech = company:GetTechNumber()
	local techs = { }
	local showeddLinks = 0
	local maxPublicTech = applic:GetTechNumber()
	
	for i=1, maxCompanyScriptTech do techs[ i ] = company:GetTech( i-1 ) end
	for i=1, maxPublicTech do techs[ maxCompanyScriptTech + i ] = applic:GetTech( i-1 ) end
	
	for i=0, #techs do
		local tech = CLuaTech( techs[ i ] )
		
		local tg = tech:GetTechGroup()
		if tg == PT_SCRIPTS or tg == PT_MINIGAME or tg == PT_PHYSIC or tg == PT_ADVTECH then
			Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleScriptLevel element = " .. i .. " " .. 20 + showeddLinks * 50 )
			local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 20 + showeddLinks * 50, 
																			   width / 2 + 50, 20 + 50 + showeddLinks * 50, -1, tab ) )
			linkModule:SetModuleType( tg )
			linkModule:SetData( tech:Self() )
			linkModule:SetDraggable( true )
			linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) )
			linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
			showeddLinks = showeddLinks + 1
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
	SetLuaFuncToLinkBox( linkScript, "sworkGameProjectWizzardSetScriptEngine" )
	
	local mg = CLuaTech( project:GetMiniGameEngine() )
	local linkMiniGames = CLuaLinkBox( guienv:AddLinkBox( "Миниигры", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	linkMiniGames:SetModuleType( PT_MINIGAME )
	linkMiniGames:SetDraggable( false )
	linkMiniGames:SetData( mg:Self() )
	if mg:Empty() == 0 then	linkMiniGames:SetText( mg:GetName() ) end
	SetLuaFuncToLinkBox( linkMiniGames, "sworkGameProjectWizzardSetMiniGameEngine" )

	local ph = CLuaTech( project:GetPhysicEngine() )
	local linkPhis = CLuaLinkBox( guienv:AddLinkBox( "Физика", 10, 300, 10 + 50, 300 + 50, -1, tab ) )
	linkPhis:SetModuleType( PT_PHYSIC )
	linkPhis:SetDraggable( false )
	linkPhis:SetData( ph:Self() )
	if ph:Empty() == 0 then linkPhis:SetText( ph:GetName() ) end
	SetLuaFuncToLinkBox( linkPhis, "sworkGameProjectWizzardSetPhysicEngine" )
	
	local showeddLinks = 0
	local xoffset = 70
	local maxProjectAdvTech = project:GetAdvTechNumber()
	
	for i=0, maxProjectAdvTech do
		local tech = CLuaTech( project:GetAdvTech( i ) )
		local linkAdv = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), xoffset, 20 + showeddLinks * 50, 
																		xoffset + 50, 20 + 50 + showeddLinks * 50, 9200 + i, tab ) )

		linkAdv:SetModuleType( PT_ADVTECH )
		linkAdv:SetData( tech:Self() )
		linkAdv:SetDraggable( false )
		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetAdvTech" )
		showeddLinks = showeddLinks + 1
		
		if showeddLinks * 50 > 450 then
			showeddLinks = 0
			xoffset = xoffset + 70
		end
	end
end

local function CreatePlatformLangPage( tab )

	for i=1, #lang do
		local button = CLuaButton( guienv:AddButton( 10 + 100 * (i-1), 50, 10 + 100 * i, 50 + 100, tab, 9600 + i, "" ) )
		if project:IsLangAvaible( lang[ i ] ) then
			button:SetText( lang[ i ] )
		else
			button:SetText( "not " .. lang[ i ] )
		end
		
		button:SetAction( "sworkGameProjectWizzardSetLang" )
	end

	for i=1, #platform do
		local button = CLuaButton( guienv:AddButton( 10 + 100 * (i-1), 200, 10 + 100 * i, 200 + 100, tab, 9700 + i,  "" ) )
		
		if project:IsPlatformAvaible( platform[ i ] ) then
			button:SetText( platform[ i ] )
		else
			button:SetText( "not " .. platform[ i ] )
		end
		button:SetAction( "sworkGameProjectWizzardSetPlatform" )
	end
end

local function ShowAvaibleScenarioAndLicense( tab )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local applic = CLuaApplication( NrpGetApplication() )
	local maxScenarioNum = applic:GetTechNumber()
	local showedLinks = 0
	
	for i=1, maxScenarioNum do
		local tech = CLuaTech( applic:GetTech( i-1 ) )
		
		if tech:GetTechGroup() == PT_SCENARIOQUALITY then
				local linkModule = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), width / 2, 10 + showedLinks * 50, 
																					   width / 2 + 50, 10 + 50 + showedLinks * 50, -1, tab ) )
				linkModule:SetModuleType( PT_SCENARIOQUALITY )
				linkModule:SetData( tech:Self() )		
				linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) ) 
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				linkModule:SetDraggable( true )
				showedLinks = showedLinks + 1
		end
	end
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:ShowAvaibleScenarioAndLicense  company scenario = " .. showedLinks )
	
	local licenseNumber = 0
	local maxLincenseNum = company:GetTechNumber()
	for i=1, maxLincenseNum do
		local license = CLuaTech( company:GetTech( i-1 ) )
		
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
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:ShowAvaibleScenarioAndLicense company license = " .. licenseNumber )
end

local function CreateScenarioLicensePage( tab )
	ShowAvaibleScenarioAndLicense( tab )
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = CLuaTech( project:GetScenario() )
	local linkScenario = CLuaLinkBox( guienv:AddLinkBox( "Сценарий", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkScenario:SetModuleType( PT_SCENARIOQUALITY )
	linkScenario:SetData( tech:Self() )
	if tech:Empty() == 0 then linkScenario:SetText( tech:GetName() ) end
	linkScenario:SetDraggable( false )
	linkScenario:SetVisible( not project:HaveLicense() )
	SetLuaFuncToLinkBox( linkScenario, "sworkGameProjectWizzardSetScenario" )
	
	local lic = CLuaTech( project:GetLicense() )
	local linkLicense = CLuaLinkBox( guienv:AddLinkBox( "Лицензия", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	linkLicense:SetModuleType( PT_LICENSE )
	linkLicense:SetData( lic:Self() )
	if lic:Empty() == 0 then linkLicense:SetText( lic:GetName() ) end
	linkLicense:SetDraggable( false )
	linkLicense:SetVisible( not project:HaveScenario() )
	SetLuaFuncToLinkBox( linkLicense, "sworkGameProjectWizzardSetLicense" )
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
	
	elm:SetObject( pages[ "advfunc" ] )
	elm:RemoveChilds() 
	CreateAdvContentPage( elm:Self() )
	
	elm:SetObject( pages[ "graphics" ] )
	elm:RemoveChilds()
	CreateVideoContentPage( elm:Self() )
	
	elm:SetObject( pages[ "sound" ] )
	elm:RemoveChilds()
	CreateSoundContentPage( elm:Self() )
	
	elm:SetObject( pages[ "scenario" ] )
	elm:RemoveChilds()
	CreateScenarioLicensePage( elm:Self() )
	
	elm:SetObject( pages[ "platforms" ] )
	elm:RemoveChilds()
	CreatePlatformLangPage( elm:Self() )
	
	elm:SetObject( pages[ "end" ] )
	elm:RemoveChilds()
	CreateEndPage( elm:Self() )
end

function sworkGameProjectWizzardSetVideoEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetGameEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetVideoQuality( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetVideoQuality( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetSoundQuality( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetSoundQuality( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetVideoTech( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9400
	project:SetVideoTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetSoundTech( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9500
	project:SetSoundTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetScriptEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetScriptEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetMiniGameEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetMiniGameEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetPhysicEngine( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetPhysicEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetAdvTech( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9200
	project:SetAdvTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetScenario( ptr )
	local sender = CLuaLinkBox( ptr )
	project:SetScenario( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetGenre( ptr )
	local sender = CLuaLinkBox( ptr )
	local id = sender:GetID() - 9100
	project:SetGenre(sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetLang( ptr )
	local button = CLuaButton( ptr )
	local id = button:GetID() - 9600
	project:ToggleLanguage( lang[ id ] )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkGameProjectWizzardSetPlatform( ptr )
	local button = CLuaButton( ptr )
	local id = button:GetID() - 9700
	project:TogglePlatform( platform[ id ] )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function sworkWindowCreateGameProjectClose( ptr )
	LogScript("Удаление временного объекта")
	project:Remove()
end

function sworkCreateGameProject( ptr )
	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_PROJECTWIZ_NAME ) )
	windowg:Remove()
	
	local applic = CLuaApplication( NrpGetApplication() )
	project:Create( "defaultGame" )
	windowg:SetObject( guienv:AddWindow( "GameWizzard", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_PROJECTWIZ_NAME )
	
	local btn = CLuaButton( windowg:GetCloseButton() )
	btn:SetAction( "sworkWindowCreateGameProjectClose" )
	
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 10, 20, 10 + 140, 20 + 20, ID_PROJECTQUALITY ) )
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