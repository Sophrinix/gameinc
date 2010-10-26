local base = _G

module( "gameprojectManager" )

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local Log = base.LogScript
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local project = nil

local labelCodeVolume = nil
local editGameName = nil
local prgProjectQuality = nil
local pageControl = nil

local platform = { "pc", "console", "gamebox", "mobile" }
local lang = { "Eng", "Den", "France", "Russian", "Jap" }

local pagesName = { }
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

local ID_COMPONENTLIST = 9012

local sizeLinkBox = 80

local mainWindow = nil

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

local function localCreateLigthingBetweenElements( link )
	local parentE = base.CLuaElement( link:GetParent() )
	
	--проидемся по вкладке
	for i=1, parentE:GetChildCount() do
		--найдем все линкбоксы
		local child = base.CLuaElement( parentE:GetChild( i-1 ) )
		
		--проверим их на соответствие с заданным типом
		if child:GetTypeName() == base.ELEMENT_GUILINKBOX then
			local tmpLink = base.CLuaLinkBox( child:Self() )
		   
			if tmpLink:GetModuleType() == link:GetModuleType() and not tmpLink:IsDraggable() then
				guienv:AddLigthing( child:Self(), link:Self(), "media/textures/larrow.png", 10 )
			end	
		end	
	end
end

local function ShowParams()
	labelCodeVolume:SetText( "Код:" .. project:GetCodeVolume() )
	prgProjectQuality:SetPosition( project:GetCodeQuality() )
end

local function ShowAvaibleEngines( tab )
	local maxEngine = company:GetEnginesNumber()
	local xoffset = 0
	local rowCount = 0	
	
	for i=1, maxEngine do
		local gameeng = company:GetEngine( i-1 )
		local linkModule = guienv:AddLinkBox( "Движок ".. i .."/"..maxEngine .. "\r(" .. gameeng:GetName() .. ")", 
															scrWidth / 2 + xoffset, sizeLinkBox * (rowCount+1), 
															scrWidth / 2 + xoffset + sizeLinkBox, sizeLinkBox * (rowCount+2), 
															-1, tab )
		localSetLinkBoxOption( linkModule, base.PT_GAMEENGINE, gameeng:Self(), 
							   gameeng:GetTexture(), true, not project:IsMyGameEngine( gameeng:Self() ) )
							   
		linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
		rowCount = rowCount + 1
		if rowCount * 50 > 450 then
			xoffset = 0
			rowCount = 0
		end
	end
end

local function ShowAvaibleGenreModules( tab )
	local maxCompanyTech = company:GetTechNumber()
	local showedTech = 0
	local yrOffset = 20

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1) 
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then	
			if tech:GetTechGroup() == base.PT_GENRE then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, yrOffset + showedTech * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, yrOffset + (showedTech + 1) * sizeLinkBox, -1, tab )
													  
				localSetLinkBoxOption( linkModule, base.PT_GENRE, tech:Self(), tech:GetTexture(), 
									   true, not project:IsGenreIncluded( tech:GetTechType() ), "media/buttons/genreNoImage2.png" )	
									   
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedTech = showedTech + 1
				yrOffset = yrOffset + 10
			end
		end
	end	
end

local function SetLuaFuncToLinkBox( lbo, funcName )
	lbo:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
	lbo:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./gameprojectManager.RightMouseButtonUp()" )
	lbo:AddLuaFunction( base.GUIELEMENT_SET_DATA, funcName )
end

local function CreateGenrePage( tab )
	ShowAvaibleGenreModules( tab )
	
	local maxModuleNumber = project:GetGenreModuleNumber()

	for i=1, maxModuleNumber do
		local genre = project:GetGenre( i-1 )
		local linkModule = guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber.."\n"..genre:GetName(), 
												 80, 20 + (i-1) * sizeLinkBox, 
												 80 + sizeLinkBox, 20 + i * sizeLinkBox, 
												 9100+(i-1), tab )
		localSetLinkBoxOption( linkModule, base.PT_GENRE, genre:Self(), genre:GetTexture(), 
						       false, true, "media/buttons/genreNoImage2.png" )		
		
		SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetGenre()" )
	end
end

local function ShowAvaibleVideoQualityAndVideoTech( tab )
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then		
			if tg == base.PT_VIDEOTECH or tg == base.PT_VIDEOQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 20 + showedTech * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 20 + (showedTech + 1) * sizeLinkBox, -1, tab )
													  
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )			

				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedTech = showedTech + 1
			end
		end
	end	
	--Log("SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function ShowAvaibleSoundQualityAndSoundTech( tab )
	local companyName = company:GetName()
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == base.PT_SOUNDTECH or tg == base.PT_SOUNDQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 200 + showedTech * sizeLinkBox, 
													   scrWidth / 2 + sizeLinkBox, 200 + (showedTech +1) * sizeLinkBox, -1, tab )
													   
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )	
									   								   
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedTech = showedTech + 1
			end
		end
	end	
	Log( "SCRIPT-CREATEVTP:ShowAvaibleSoundQualityAndSoundTech public = " .. showedTech )
end

local function CreateSoundContentPage( tab )
	Log( "Start Sound" )
	ShowAvaibleSoundQualityAndSoundTech( tab )
		
	local sq = project:GetSoundQuality()
	local linkModule = guienv:AddLinkBox( "", sizeLinkBox, 20, sizeLinkBox + sizeLinkBox, 20 + sizeLinkBox, -1, tab )
	
	localSetLinkBoxOption( linkModule, base.PT_SOUNDQUALITY, sq:Self(), sq:GetTexture(), 
						   false, true, "media/buttons/SoundQualityNoImage.png" )
						   
	if sq:Empty() == 0 then	linkModule:SetText( sq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetSoundQuality()" )

	local xoffset = 20
	local maxProjectSoundTech = project:GetSoundTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectSoundTech do
		local tech = project:GetSoundTech( i )
		local linkAdv = guienv:AddLinkBox( "Sound " .. i .. "/" .. maxProjectSoundTech, 
														xoffset, 120 + showeddLinks * sizeLinkBox, 
														xoffset + sizeLinkBox, 120 + sizeLinkBox + showeddLinks * sizeLinkBox, 9500 + i, tab )
		localSetLinkBoxOption( linkAdv, base.PT_SOUNDTECH, tech:Self(), tech:GetTexture(), 
						       false, true, "media/buttons/soundTechNoImage.png" )

		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetSoundTech()" )
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
	localSetLinkBoxOption( linkModule, base.PT_VIDEOQUALITY, vq:Self(), vq:GetTexture(), 
						   false, true, "media/buttons/qualityGraphicNoImage.png" )
	if vq:Empty() == 0 then	linkModule:SetText( vq:GetName() )	end
	SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetVideoQuality()" )
	
	local xoffset = 20
	local maxProjectVideoTech = project:GetVideoTechNumber()
	local showeddLinks = 0
	
	for i=0, maxProjectVideoTech do
		local tech = project:GetVideoTech( i )
		local linkAdv = guienv:AddLinkBox( "Видео " .. i .. "/" .. maxProjectVideoTech,
														xoffset, 120 + showeddLinks * sizeLinkBox, 
														xoffset + sizeLinkBox, 120 + (showeddLinks + 1) * sizeLinkBox, 9400 + i, tab )

		localSetLinkBoxOption( linkAdv, base.PT_VIDEOTECH, tech:Self(), tech:GetTexture(),		
							   false, true, "media/buttons/videoTechNoImage.png" )
		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetVideoTech()" )
		
		showeddLinks = showeddLinks + 1
		
		if (20 + showeddLinks * sizeLinkBox) > 500 then
			showeddLinks = 0
			xoffset = xoffset + 90
		end
	end	
end

local function CreateGameNamePage( tab )
	ShowAvaibleEngines( tab )

	editGameName = guienv:AddEdit( "Название игры", 10, 20, 10 + 180, 20 + 20,  -1, tab )
	
	local ge = project:GetGameEngine()
	--create linkbox for gameEngine
	local linkModule = guienv:AddLinkBox( "", 80, 80, 80 + 80, 80 + 80, -1, tab )
	localSetLinkBoxOption( linkModule, base.PT_GAMEENGINE, ge:Self(), ge:GetTexture(), 
	                       false, true, "media/buttons/GameNoEngine.png" )
	--set function for resolve input for linkbox	
	SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetVideoEngine()" )
	
	linkModule = guienv:AddLinkBox( "Продолжение", 80, 200, 80 + 80, 200 + 80, -1, tab )
	localSetLinkBoxOption( linkModule, base.PT_GAME, nil, "", false, true )
end

local function CreateEndPage( tab )
	local linkModule = guienv:AddLinkBox( base.STR_ENGINE, 10, 50, 10 + 50, 50 + 50, -1, tab )
	linkModule:SetData( project:GetGameEngine() )
	linkModule:SetTexture( project:GetGameEngine():GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_GENRE, 10, 100, 10 + 50, 100 + 50, -1, tab ) )
	linkModule:SetData( project:GetGenre( 0 ) )
	linkModule:SetTexture( project:GetGenre( 0 ):GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_CONTENT, 10, 150, 10 + 50, 150 + 50, -1, tab ) )
	if project:HaveLicense() or project:HaveScenario() then 
	   if project:HaveLicense() then
			linkModule:SetTexture( project:GetLicense():GetTexture() ) 
	   else
			linkModule:SetTexture( project:GetScenario():GetTexture() ) 
	   end
	end
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_PLATFORMS, 10, 200, 10 + 50, 200 + 50, -1, tab ) )
	if project:GetPlatformsNumber() > 0 then 
		linkModule:SetData( linkModule:Self()  ) 
	end
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_LOCALIZATION, 10, 250, 10 + 50, 250 + 50, -1, tab ) )
	if project:GetLanguagesNumber() > 0 then 
		linkModule:SetData( linkModule:Self() ) 
	end
	
	local button = guienv:AddButton( 10, 10, 10 + 50, 10 + 50, tab, -1, base.STR_FINISH )
	button:SetAction( "./gameprojectManager.CreateProjectGame()" )	
	button:SetEnabled( project:IsProjectReady() )
end

function CreateProjectGame()
	project:SetName( editGameName:GetText() )
	
	local prj = company:GetProjectByName( project:GetName() )
	
	if prj:Empty() == 1 then
		prj = company:CreateDevelopGame( project:Self() )
		company:AddToPortfelle( prj:Self() )
		sworkUpdateCompanyPortfelle()
	else
		guienv:ShowMessage( "Уже есть проект с таким именем" )
	end
end

local function ShowAvaibleScriptAndMiniGames( tab )
	local companyName = company:GetName()
	local showeddLinks = 0
	local maxPublicTech = applic:GetTechNumber()
	
	for i=1, maxPublicTech do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == base.PT_SCRIPTS or tg == base.PT_MINIGAME or tg == base.PT_PHYSIC or tg == base.PT_ADVTECH then
				--Log("SCRIPT-CREATEGP:ShowAvaibleScriptLevel element = " .. i .. " " .. 20 + showeddLinks * sizeLinkBox )
				
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 20 + showeddLinks * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 20 + (showeddLinks + 1 ) * sizeLinkBox, -1, tab )
													  
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(), 
									   true, not project:IsTechInclude( tech:GetTechType() ), "" )												  
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showeddLinks = showeddLinks + 1
			end
		end
	end	
	--Log("SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

local function CreateAdvContentPage( tab )
	ShowAvaibleScriptAndMiniGames( tab )
	
	local se = project:GetScriptEngine()
	local linkScript = guienv:AddLinkBox( "Скрипты", 20, 20, 20 + sizeLinkBox, 20 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkScript, base.PT_SCRIPTS, se:Self(), se:GetTexture(), 
	                       false, true, "media/buttons/scriptsNoImage.png" )
	                       
	if se:Empty() == 0 then linkScript:SetText( se:GetName() )	end
	SetLuaFuncToLinkBox( linkScript, "./gameprojectManager.SetScriptEngine()" )
	
	local mg = project:GetMiniGameEngine()
	local linkMiniGames = guienv:AddLinkBox( "", 20, 120, 20 + sizeLinkBox, 120 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkMiniGames, base.PT_MINIGAME, mg:Self(), mg:GetTexture(), 
	                       false, true, "media/buttons/minigameNoImage.png" )

	if mg:Empty() == 0 then	linkMiniGames:SetText( mg:GetName() ) end
	SetLuaFuncToLinkBox( linkMiniGames, "./gameprojectManager.SetMiniGameEngine()" )


	local ph = project:GetPhysicEngine()
	local linkPhis = guienv:AddLinkBox( "", 20, 220, 20 + sizeLinkBox, 220 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkPhis, base.PT_PHYSIC, ph:Self(), ph:GetTexture(), 
	                       false, true, "media/buttons/physicNoImage.tga" )

	if ph:Empty() == 0 then linkPhis:SetText( ph:GetName() ) end
	SetLuaFuncToLinkBox( linkPhis, "./gameprojectManager.SetPhysicEngine()" )

	local showeddLinks = 0
	local xoffset = 120
	local maxProjectAdvTech = project:GetGameEngine():GetAdvancedTechNumber()
    
	for i=1, maxProjectAdvTech do
		local tech = project:GetAdvTech( i-1 )
		local linkAdv = guienv:AddLinkBox( tech:GetName(), xoffset, 20 + showeddLinks * sizeLinkBox, 
										   xoffset + sizeLinkBox, 20 + (showeddLinks + 1) * sizeLinkBox, 9200 + i, tab )

		localSetLinkBoxOption( linkAdv, base.PT_ADVTECH, tech:Self(), tech:GetTexture(), 
	                           false, true, "media/buttons/advTechNoImage.png" )

		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetAdvTech()" )
		showeddLinks = showeddLinks + 1
		
		if showeddLinks * sizeLinkBox > 300 then
			showeddLinks = 0
			xoffset = xoffset + sizeLinkBox
		end
	end
end

local function CreatePlatformLangPage( tab )
	for i=1, #lang do
		local btnF = guienv:AddButton( 10 + 100 * (i-1), 50, 10 + 100 * i, 50 + 100, tab, 9600 + i, "" )
		if project:IsLangAvaible( lang[ i ] ) then
			btnF:SetText( lang[ i ] )
		else
			--Log( "not " .. lang[ i ]  )
			btnF:SetText( "not " .. lang[ i ] )
		end
		
		btnF:SetAction( "./gameprojectManager.SetLang()" )
	end

	for i=1, #platform do
		local btnF = guienv:AddButton( 10 + 100 * (i-1), 200, 10 + 100 * i, 200 + 100, tab, 9700 + i,  "" )
		
		if project:IsPlatformAvaible( platform[ i ] ) then
			btnF:SetText( platform[ i ] )
		else
			btnF:SetText( "not " .. platform[ i ] )
		end
		btnF:SetAction( "./gameprojectManager.SetPlatform()" )
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
			if tg == base.PT_SCENARIOQUALITY or tg == base.PT_LICENSE then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 10 + showedLinks * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 10 + (showedLinks + 1) * sizeLinkBox, -1, tab )
													   
				localSetLinkBoxOption( linkModule, tg, tech:Self(), tech:GetTexture(),		
									   true, true, "" )
									  
				if tg == base.PT_SCENARIOQUALITY then
					linkModule:SetEnabled( not project:IsTechInclude( tech:GetTechType() ) ) 
				else
					linkModule:SetEnabled( not project:IsLicenseIncluded( tech:GetName() ) ) 
				end
				
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedLinks = showedLinks + 1
			end
		end
	end
end

local function CreateScenarioLicensePage( tab )
	ShowAvaibleScenarioAndLicense( tab )
	--Log("SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = project:GetScenario()
	local linkScenario = guienv:AddLinkBox( base.STR_SCENARIO, 100, 40, 100 + sizeLinkBox, 40 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkScenario, base.PT_SCENARIOQUALITY, tech:Self(), tech:GetTexture(),		
						   false, not project:HaveLicense(), "media/buttons/scenarioNoImage.jpg" )
	if tech:Empty() == 0 then linkScenario:SetText( tech:GetName() ) end
	linkScenario:SetDraggable( false )
	SetLuaFuncToLinkBox( linkScenario, "./gameprojectManager.SetScenario()" )
	
	local lic = project:GetLicense()
	local linkLicense = guienv:AddLinkBox( base.STR_LICENSE, 100, 200, 100 + sizeLinkBox, 200 + sizeLinkBox, -1, tab )
	localSetLinkBoxOption( linkLicense, base.PT_LICENSE, lic:Self(), lic:GetTexture(),		
						   false, not project:HaveScenario(), "media/buttons/licenseNoImage.jpg" )
						   
	if lic:Empty() == 0 then linkLicense:SetText( lic:GetName() ) end
	linkLicense:SetVisible( not project:HaveScenario() )
	SetLuaFuncToLinkBox( linkLicense, "./gameprojectManager.SetLicense()" )
	
	--Log( "SCRIPT-CREATESL:CreateScenarioLicensePage end " )
end

local function sworkRecreatePagesDependedEngine()
	pages[ "genre" ]:RemoveChilds()
	CreateGenrePage( pages[ "genre" ]:Self() )
	
	pages[ "graphics" ]:RemoveChilds()
	CreateVideoContentPage( pages[ "graphics" ]:Self() )
	
	pages[ "name" ]:RemoveChilds()
	CreateGameNamePage( pages[ "name" ]:Self() )
	
	pages[ "advfunc" ]:RemoveChilds() 
	CreateAdvContentPage( pages[ "advfunc" ]:Self() )
	
	pages[ "graphics" ]:RemoveChilds()
	CreateVideoContentPage( pages[ "graphics" ]:Self() )
	
	pages[ "sound" ]:RemoveChilds()
	CreateSoundContentPage( pages[ "sound" ]:Self() )
	
	pages[ "scenario" ]:RemoveChilds()
	CreateScenarioLicensePage( pages[ "scenario" ]:Self() )
	
	pages[ "platforms" ]:RemoveChilds()
	CreatePlatformLangPage( pages[ "platforms" ]:Self() )
	
	pages[ "end" ]:RemoveChilds()
	CreateEndPage( pages[ "end" ]:Self() )
end

function SetVideoEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetGameEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetVideoQuality()

	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetVideoQuality( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetSoundQuality()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetSoundQuality( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetVideoTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() - 9400
	project:SetVideoTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetSoundTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() - 9500
	project:SetSoundTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetScriptEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetScriptEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetMiniGameEngine()
	local sender = bsae.CLuaLinkBox( base.NrpGetSender() )
	project:SetMiniGameEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetPhysicEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetPhysicEngine( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetAdvTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() - 9200
	project:SetAdvTech( sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetScenario()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetScenario( sender:GetData() )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetGenre()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() - 9100
	project:SetGenre(sender:GetData(), id )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetLang( )
	local button = base.CLuaButton( base.NrpGetSender() )
	local id = button:GetID() - 9600
	project:ToggleLanguage( lang[ id ] )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function SetPlatform()
	local button = base.CLuaButton( base.NrpGetSender() )
	local id = button:GetID() - 9700
	project:TogglePlatform( platform[ id ] )
	sworkRecreatePagesDependedEngine()
	ShowParams()
end

function Hide()
	project:Remove()
	mainWindow:Remove()
	mainWindow = nil
end

local function AddTab( name )
	pages[ name ] = guienv:AddTab( pageControl, pagesName[ name ], pagesID[ name ] ) --name
	pages[ name ]:SetFontFromSize( 16 )
	pages[ name ]:SetTextColor( 0xff, 0xff, 0xff, 0xff )
	
	return pages[ name ]
end

function Show()
	company = applic:GetPlayerCompany()
	if mainWindow == nil then
		project = base.CLuaGameProject():Create( "defaultGame" )
		
		mainWindow = guienv:AddWindow( "media/textures/monitor.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 60, scrWidth - 20, scrHeight, 
		 			    "button_poweroff", mainWindow:Self(), -1, "",
						"./gameprojectManager.Hide()" )
	end 
	
	prgProjectQuality = guienv:AddProgressBar( mainWindow:Self(), 10, 20, 10 + 140, 20 + 20, -1 )
	labelCodeVolume = guienv:AddLabel( "Код", scrWidth / 2, 20, scrWidth, 20 + 20, -1, mainWindow:Self() )
	
	pageControl = guienv:AddTabControl( 150, 80, scrWidth - 150, scrHeight - 80, -1, mainWindow:Self() )
  	CreateGameNamePage( AddTab( "name" ):Self() )
	CreateGenrePage( AddTab( "genre" ):Self() )
	CreateScenarioLicensePage( AddTab( "scenario" ):Self() )
	CreateAdvContentPage( AddTab( "advfunc" ):Self() )
	CreateVideoContentPage( AddTab( "graphics" ):Self() )
	CreateSoundContentPage( AddTab( "sound" ):Self() )
	CreatePlatformLangPage( AddTab( "platforms" ):Self() )
	CreateEndPage( AddTab( "end" ):Self() )
	
	mainWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "gameprojectManager.FadeEnterAction()" )
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

function RightMouseButtonUp()
	local link = base.CLuaLinkBox( base.NrpGetSender() ) 
	link:SetData( nil )
	link:SetTexture( "media/buttons/LinkBoxNoImage.png" )
end

function LeftMouseButtonUp()
	local elm = base.CLuaElement( base.NrpGetSender() )
	
	if elm:Empty() == 0 and elm:GetTypeName() == base.ELEMENT_GUILINKBOX then
		local link = base.CLuaLinkBox( elm:Self() ) 
		local linkRecv = base.CLuaLinkBox( guienv:GetDragObject() )
		
		if link:IsDraggable() then
			if link:IsEnabled() then
				guienv:SetDragObject( link:Self() )
				localCreateLigthingBetweenElements( link )
			end
		else
			if link:GetModuleType() == linkRecv:GetModuleType() then
				link:SetData( linkRecv:GetData() )
				link:SetTexture( linkRecv:GetTexture() )
			end
			
			guienv:SetDragObject( nil )
		end
	else
		guienv:SetDragObject( nil )
	end
end