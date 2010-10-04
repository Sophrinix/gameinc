local project = CLuaGameProject( nil )

local platform = { "pc", "console", "gamebox", "mobile" }
local lang = { "Eng", "Den", "France", "Russian", "Jap" }

local pagesName = {  }
local pagesID = { }
local pages = { }

pagesName[ "name" ] = "Название";		pagesID[ "name" ] = 9000 
pagesName[ "genre" ] = "Жанр";			pagesID[ "genre" ] = 9001
pagesName[ "scenario" ] = "Сценарий";	pagesID[ "scenario" ] = 9002
pagesName[ "advfunc" ] = "Доп.Возможности"; pagesID[ "advfunc" ] = 9003
pagesName[ "graphics" ] = "Графика";	pagesID[ "graphics" ] = 9004
pagesName[ "sound" ] = "Звук";			pagesID[ "sound" ] = 9005
pagesName[ "platforms" ] = "Платформы"; pagesID[ "platforms" ] = 9006
pagesName[ "end" ] = "Завершить";		pagesID[ "end" ] = 9007

local ID_CODEVOLUME = 9010
local ID_PROJECTQUALITY = 9011
local ID_COMPONENTLIST = 9012

local sizeLinkBox = 80

local windowGameProjectCreating = nil

local function localSetLinkBoxOption( lnkx, typer, userdata, textureName, draggable, enabled, defTexture )
	lnkx:SetModuleType( typer )
	lnkx:SetData( userdata )
	lnkx:SetTexture( textureName )
	lnkx:SetDraggable( draggable )
	lnkx:SetEnabled( enabled )
	
	if defTexture and defTexture ~= "" then
		lnkx:SetDefaultTexture( defTexture )
	else
		lnkx:SetDefaultTexture( "media/buttons/linboxNoImage.png" )
	end
end

local function ShowParams()
	local label = CLuaLabel( guienv:GetElementByID( ID_CODEVOLUME ) )
	local prg = CLuaProgressBar( guienv:GetElementByID( ID_PROJECTQUALITY ) )
	
	label:SetText( "Код:" .. project:GetCodeVolume() )
	prg:SetPosition( project:GetCodeQuality() )
end

local function ShowAvaibleEngines( tab )
	local company = applic:GetPlayerCompany()
	local maxEngine = company:GetEnginesNumber()
	local xoffset = 0
	local rowCount = 0	
	
	for i=1, maxEngine do
		local gameeng = company:GetEngine( i-1 )
		local linkModule = guienv:AddLinkBox( "Движок ".. i .."/"..maxEngine .. "\r(" .. gameeng:GetName() .. ")", 
															scrWidth / 2 + xoffset, sizeLinkBox * (rowCount+1), 
															scrWidth / 2 + xoffset + sizeLinkBox, sizeLinkBox * (rowCount+2), 
															-1, tab )
		localSetLinkBoxOption( linkModule, PT_GAMEENGINE, gameeng:Self(), 
							   gameeng:GetTexture(), true, not project:IsMyGameEngine( gameeng:Self() ) )
		linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		rowCount = rowCount + 1
		if rowCount * 50 > 450 then
			xoffset = 0
			rowCount = 0
		end
	end
end

local function ShowAvaibleGenreModules( tab )
	local company = applic:GetPlayerCompany()
	local maxCompanyTech = company:GetTechNumber()
	local showedTech = 0
	local yrOffset = 20

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1) 
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then	
			if tech:GetTechGroup() == PT_GENRE then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, yrOffset + showedTech * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, yrOffset + (showedTech + 1) * sizeLinkBox, -1, tab )
				localSetLinkBoxOption( linkModule, PT_GENRE, tech:Self(), tech:GetTexture(), 
									   true, not project:IsGenreIncluded( tech:GetTechType() ), "media/buttons/genreNoImage2.png" )	
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showedTech = showedTech + 1
				yrOffset = yrOffset + 10
			end
		end
	end	
end

local function SetLuaFuncToLinkBox( lbo, funcName )
	lbo:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
	lbo:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	lbo:AddLuaFunction( GUIELEMENT_SET_DATA, funcName )
end

local function CreateGenrePage( tab )
	ShowAvaibleGenreModules( tab )
	
	local company = applic:GetPlayerCompany()
	local maxModuleNumber = project:GetGenreModuleNumber()

	if maxModuleNumber > 0 then
		local genre = project:GetGenre( 0 )
		local linkModule = guienv:AddLinkBox( "Жанр", 80, 20, 80 + sizeLinkBox, 20 + sizeLinkBox, 9100, tab )
		localSetLinkBoxOption( linkModule, PT_GENRE, genre:Self(), genre:GetTexture(), 
							   false, true, "media/buttons/genreNoImage2.png" )		
		SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetGenre" )
		if genre:Empty() == 0 then linkModule:SetText( genre:GetName() ) end


		for i=1, maxModuleNumber-1 do
			genre:SetObject( project:GetGenre( i ) )
			linkModule:SetObject( guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber, 
													 80, 200 + i * sizeLinkBox, 
													 80 + sizeLinkBox, 200 + (i+1) * sizeLinkBox, 
													 9100+i, tab ) )
			localSetLinkBoxOption( linkModule, PT_GENRE, genre:Self(), genre:GetTexture(), 
							       false, true, "media/buttons/genreNoImage2.png" )		
			
			if genre:Empty() == 0 then	linkModule:SetData( genre:GetName() ) end
			SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetGenre" )
		end
	else
		
	end
end

local function ShowAvaibleVideoQualityAndVideoTech( tab )
	local company = applic:GetPlayerCompany()
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then		
			if tg == PT_VIDEOTECH or tg == PT_VIDEOQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 20 + showedTech * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 20 + (showedTech + 1) * sizeLinkBox, -1, tab )
													  
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )			

				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showedTech = showedTech + 1
			end
		end
	end	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function ShowAvaibleSoundQualityAndSoundTech( tab )
	local companyName = applic:GetPlayerCompany():GetName()
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == PT_SOUNDTECH or tg == PT_SOUNDQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 200 + showedTech * sizeLinkBox, 
													   scrWidth / 2 + sizeLinkBox, 200 + (showedTech +1) * sizeLinkBox, -1, tab )
													   
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )									   
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showedTech = showedTech + 1
			end
		end
	end	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEVTP:ShowAvaibleSoundQualityAndSoundTech public = " .. showedTech )
end

local function CreateSoundContentPage( tab )

	ShowAvaibleSoundQualityAndSoundTech( tab )
	
	local sq = project:GetSoundQuality()
	local linkModule = guienv:AddLinkBox( "", sizeLinkBox, 20, sizeLinkBox + sizeLinkBox, 20 + sizeLinkBox, -1, tab )
	
	localSetLinkBoxOption( linkModule, PT_SOUNDQUALITY, sq:Self(), sq:GetTexture(), 
						   false, true, "media/buttons/SoundQualityNoImage.png" )
	if sq:Empty() == 0 then	linkModule:SetText( sq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetSoundQuality" )

	local xoffset = 20
	local maxProjectSoundTech = project:GetSoundTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectSoundTech do
		local tech = project:GetSoundTech( i )
		local linkAdv = guienv:AddLinkBox( "Sound " .. i .. "/" .. maxProjectSoundTech, 
														xoffset, 120 + showeddLinks * sizeLinkBox, 
														xoffset + sizeLinkBox, 120 + sizeLinkBox + showeddLinks * sizeLinkBox, 9500 + i, tab )
		localSetLinkBoxOption( linkAdv, PT_SOUNDTECH, tech:Self(), tech:GetTexture(), 
						       false, true, "media/buttons/soundTechNoImage.png" )

		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetSoundTech" )
		showeddLinks = showeddLinks + 1
		
		if (20 + showeddLinks * sizeLinkBox) > 500 then
			showeddLinks = 0
			xoffset = xoffset + sizeLinkBox + 10
		end
	end	
end

local function CreateVideoContentPage( tab )
	ShowAvaibleVideoQualityAndVideoTech( tab )
	
	local vq = project:GetVideoQuality()
	local linkModule = guienv:AddLinkBox( "", 80, 20, 80 + sizeLinkBox, 20 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkModule, PT_VIDEOQUALITY, vq:Self(), vq:GetTexture(), 
						   false, true, "media/buttons/qualityGraphicNoImage.png" )
	if vq:Empty() == 0 then	linkModule:SetText( vq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetVideoQuality" )
	
	local xoffset = 20
	local maxProjectVideoTech = project:GetVideoTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectVideoTech do
		local tech = project:GetVideoTech( i )
		local linkAdv = guienv:AddLinkBox( "Видео " .. i .. "/" .. maxProjectVideoTech,
														xoffset, 120 + showeddLinks * sizeLinkBox, 
														xoffset + sizeLinkBox, 120 + (showeddLinks + 1) * sizeLinkBox, 9400 + i, tab )

		localSetLinkBoxOption( linkAdv, PT_VIDEOTECH, tech:Self(), tech:GetTexture(),		
							   false, true, "media/buttons/videoTechNoImage.png" )
		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetVideoTech" )
		
		showeddLinks = showeddLinks + 1
		
		if (20 + showeddLinks * sizeLinkBox) > 500 then
			showeddLinks = 0
			xoffset = xoffset + 90
		end
	end	
end

local function CreateGameNamePage( tab )
	ShowAvaibleEngines( tab )

	local edit = guienv:AddEdit( "Название игры", 10, 20, 10 + 180, 20 + 20,  -1, tab )
	edit:SetName( WNDGMWIZ_NAME_EDIT )

	local ge = project:GetGameEngine()
	--create linkbox for gameEngine
	local linkModule = guienv:AddLinkBox( "", 80, 80, 80 + 80, 80 + 80, -1, tab )
	localSetLinkBoxOption( linkModule, PT_GAMEENGINE, ge:Self(), ge:GetTexture(), 
	                       false, true, "media/buttons/GameNoEngine.png" )
	--set function for resolve input for linkbox	
	SetLuaFuncToLinkBox( linkModule, "sworkGameProjectWizzardSetVideoEngine" )
	
	linkModule = guienv:AddLinkBox( "Продолжение", 80, 200, 80 + 80, 200 + 80, -1, tab )
	localSetLinkBoxOption( linkModule, PT_GAME, nil, "", false, true )
end

local function CreateEndPage( tab )
	local linkModule = guienv:AddLinkBox( "Игровой движок", 10, 50, 10 + 50, 50 + 50, -1, tab )
	linkModule:SetData( project:GetGameEngine() )
	linkModule:SetTexture( project:GetGameEngine():GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( "Жанр", 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetData( project:GetGenre( 0 ) )
	linkModule:SetTexture( project:GetGenre( 0 ):GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( "Контент", 10, 150, 10 + 50, 150 + 50, -1, tab ) )
	if project:HaveLicense() or project:HaveScenario() then 
	   if project:HaveLicense() then
			linkModule:SetTexture( project:GetLicense():GetTexture() ) 
	   else
			linkModule:SetTexture( project:GetScenario():GetTexture() ) 
	   end
	end
	
	linkModule:SetObject( guienv:AddLinkBox( "Платформы", 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	if project:GetPlatformsNumber() > 0 then 
		linkModule:SetData( linkModule:Self()  ) 
	end
	
	linkModule:SetObject( guienv:AddLinkBox( "Локализация", 10, 250, 10 + 50, 250 + 50, -1, tab ) )
	if project:GetLanguagesNumber() > 0 then 
		linkModule:SetData( linkModule:Self() ) 
	end
	
	local button = guienv:AddButton( 10, 10, 10 + 50, 10 + 50, tab, -1, "Завершить" )
	button:SetAction( "sworkCreateProjectGameToCompany" )	
	button:SetEnabled( project:IsProjectReady() )
end

function sworkCreateProjectGameToCompany( ptr )
	local company = applic:GetPlayerCompany()
	local edit = CLuaEdit( guienv:GetElementByName( WNDGMWIZ_NAME_EDIT ) )
	project:SetName( edit:GetText() )
	
	local prj = CLuaGameProject( company:GetProjectByName( project:GetName() ) )
	
	if prj:Empty() == 1 then
		prj:SetObject( company:CreateDevelopGame( project:Self() ) )
		company:AddToPortfelle( prj:Self() )
		sworkUpdateCompanyPortfelle()
	else
		guienv:ShowMessage( "Уже есть проект с таким именем" )
	end
end

local function ShowAvaibleScriptAndMiniGames( tab )
	local companyName = applic:GetPlayerCompany():GetName()
	local showeddLinks = 0
	local maxPublicTech = applic:GetTechNumber()
	
	for i=1, maxPublicTech do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == PT_SCRIPTS or tg == PT_MINIGAME or tg == PT_PHYSIC or tg == PT_ADVTECH then
				Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleScriptLevel element = " .. i .. " " .. 20 + showeddLinks * sizeLinkBox )
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 20 + showeddLinks * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 20 + (showeddLinks + 1 ) * sizeLinkBox, -1, tab )
													  
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )												  
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showeddLinks = showeddLinks + 1
			end
		end
	end	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

local function CreateAdvContentPage( tab )
	ShowAvaibleScriptAndMiniGames( tab )
	
	local se = project:GetScriptEngine()
	local linkScript = guienv:AddLinkBox( "Скрипты", 20, 20, 20 + sizeLinkBox, 20 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkScript, PT_SCRIPTS, se:Self(), se:GetTexture(), 
	                       false, true, "media/buttons/scriptsNoImage.png" )
	                       
	if se:Empty() == 0 then linkScript:SetText( se:GetName() )	end
	SetLuaFuncToLinkBox( linkScript, "sworkGameProjectWizzardSetScriptEngine" )
	
	local mg = project:GetMiniGameEngine()
	local linkMiniGames = guienv:AddLinkBox( "", 20, 120, 20 + sizeLinkBox, 120 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkMiniGames, PT_MINIGAME, mg:Self(), mg:GetTexture(), 
	                       false, true, "media/buttons/minigameNoImage.png" )

	if mg:Empty() == 0 then	linkMiniGames:SetText( mg:GetName() ) end
	SetLuaFuncToLinkBox( linkMiniGames, "sworkGameProjectWizzardSetMiniGameEngine" )


	local ph = project:GetPhysicEngine()
	local linkPhis = guienv:AddLinkBox( "", 20, 220, 20 + sizeLinkBox, 220 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkPhis, PT_PHYSIC, ph:Self(), ph:GetTexture(), 
	                       false, true, "media/buttons/physicNoImage.tga" )

	if ph:Empty() == 0 then linkPhis:SetText( ph:GetName() ) end
	SetLuaFuncToLinkBox( linkPhis, "sworkGameProjectWizzardSetPhysicEngine" )

	local showeddLinks = 0
	local xoffset = 120
	local maxProjectAdvTech = project:GetGameEngine():GetAdvancedTechNumber()
    
	for i=1, maxProjectAdvTech do
		local tech = project:GetAdvTech( i-1 )
		local linkAdv = guienv:AddLinkBox( tech:GetName(), xoffset, 20 + showeddLinks * sizeLinkBox, 
										   xoffset + sizeLinkBox, 20 + (showeddLinks + 1) * sizeLinkBox, 9200 + i, tab )

		localSetLinkBoxOption( linkAdv, PT_ADVTECH, tech:Self(), tech:GetTexture(), 
	                           false, true, "media/buttons/advTechNoImage.png" )

		SetLuaFuncToLinkBox( linkAdv, "sworkGameProjectWizzardSetAdvTech" )
		showeddLinks = showeddLinks + 1
		
		if showeddLinks * sizeLinkBox > 300 then
			showeddLinks = 0
			xoffset = xoffset + sizeLinkBox
		end
	end
end

local function CreatePlatformLangPage( tab )

	for i=1, #lang do
		local button = guienv:AddButton( 10 + 100 * (i-1), 50, 10 + 100 * i, 50 + 100, tab, 9600 + i, "" )
		if project:IsLangAvaible( lang[ i ] ) then
			button:SetText( lang[ i ] )
		else
			button:SetText( "not " .. lang[ i ] )
		end
		
		button:SetAction( "sworkGameProjectWizzardSetLang" )
	end

	for i=1, #platform do
		local button = guienv:AddButton( 10 + 100 * (i-1), 200, 10 + 100 * i, 200 + 100, tab, 9700 + i,  "" )
		
		if project:IsPlatformAvaible( platform[ i ] ) then
			button:SetText( platform[ i ] )
		else
			button:SetText( "not " .. platform[ i ] )
		end
		button:SetAction( "sworkGameProjectWizzardSetPlatform" )
	end
end

local function ShowAvaibleScenarioAndLicense( tab )
	local companyName = applic:GetPlayerCompany():GetName()
	local maxScenarioNum = applic:GetTechNumber()
	local showedLinks = 0
	
	for i=1, maxScenarioNum do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == PT_SCENARIOQUALITY or tg == PT_LICENSE then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 10 + showedLinks * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 10 + (showedLinks + 1) * sizeLinkBox, -1, tab )
													   
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(),		
									   true, true, "" )
									  
				if tg == PT_SCENARIOQUALITY then
					linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) ) 
				else
					linkModule:SetEnabled( not project:IsLicenseIncluded( tech:GetName() ) ) 
				end
				
				linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
				showedLinks = showedLinks + 1
			end
		end
	end
end

local function CreateScenarioLicensePage( tab )
	ShowAvaibleScenarioAndLicense( tab )
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = project:GetScenario()
	local linkScenario = guienv:AddLinkBox( "Сценарий", 100, 40, 100 + sizeLinkBox, 40 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkScenario, PT_SCENARIOQUALITY, tech:Self(), tech:GetTexture(),		
						   false, not project:HaveLicense(), "media/buttons/scenarioNoImage.jpg" )
	if tech:Empty() == 0 then linkScenario:SetText( tech:GetName() ) end
	linkScenario:SetDraggable( false )
	SetLuaFuncToLinkBox( linkScenario, "sworkGameProjectWizzardSetScenario" )
	
	local lic = project:GetLicense()
	local linkLicense = guienv:AddLinkBox( "Лицензия", 100, 200, 100 + sizeLinkBox, 200 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkLicense, PT_LICENSE, lic:Self(), lic:GetTexture(),		
						   false, not project:HaveScenario(), "media/buttons/licenseNoImage.jpg" )
	if lic:Empty() == 0 then linkLicense:SetText( lic:GetName() ) end
	linkLicense:SetVisible( not project:HaveScenario() )
	SetLuaFuncToLinkBox( linkLicense, "sworkGameProjectWizzardSetLicense" )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-CREATESL:CreateScenarioLicensePage end " )
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
	project:Remove()
	windowGameProjectCreating:Remove()
	windowGameProjectCreating = nil
end

function sworkCreateGameProject( ptr )
	--удалим окно с кнопками выбора проектов
	sworkCloseWindowWizardProject()
	
	--
	if windowGameProjectCreating == nil then
		project:Create( "defaultGame" )
		
		windowGameProjectCreating = guienv:AddWindow( "media/monitor.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		windowGameProjectCreating:SetDraggable( false )
		windowGameProjectCreating:GetCloseButton():SetVisible( false )
		
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 60, scrWidth - 20, scrHeight, 
		 			    "button_poweroff", windowGameProjectCreating:Self(), -1, "",
						"sworkWindowCreateGameProjectClose" )
	end 
	
	local prg = guienv:AddProgressBar( windowGameProjectCreating:Self(), 10, 20, 10 + 140, 20 + 20, ID_PROJECTQUALITY )
	local volCodeLabel = guienv:AddLabel( "Код", scrWidth / 2, 20, scrWidth, 20 + 20, ID_CODEVOLUME, windowGameProjectCreating:Self() )
	
	local tabContol = guienv:AddTabControl( 150, 80, scrWidth - 150, scrHeight - 80, -1, windowGameProjectCreating:Self() )
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
	
	windowGameProjectCreating:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkWindowLeftMouseButtonUp" )
end

function sworkWindowLeftMouseButtonUp( ptr )
	guienv:SetDragObject( nil )
end

function sworkRigthMouseButtonUp( ptr )
	local link = CLuaLinkBox( ptr ) 
	link:SetData( nil )
	link:SetTexture( "media/buttons/LinkBoxNoImage.png" )
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
				link:SetTexture( linkRecv:GetTexture() )
			end
			
			guienv:SetDragObject( nil )
		end
	end
end