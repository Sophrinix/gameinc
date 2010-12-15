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
local projectWindow = nil

local labelCodeVolume = nil
local gameName = "withoutName"
local editGameName = nil
local prgProjectQuality = nil

-- "name" "engine"
local step = "name"
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

local function SetLuaFuncToLinkBox( lbo, funcName )
	lbo:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
	lbo:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./gameprojectManager.RightMouseButtonUp()" )
	lbo:AddLuaFunction( base.GUIELEMENT_SET_DATA, funcName )
end

local function localCreateLigthingBetweenElements( link )
	local parentE = base.CLuaElement( link:GetParent() )
	
	--проидемся по вкладке
	for i=1, parentE:GetChildCount() do
		--найдем все линкбоксы
		local child = base.CLuaElement( parentE:GetChild( i-1 ) )
		
		--проверим их на соответствие с заданным типом
		if child:GetTypeName() == base.ELEMENT_GUILINKBOX then
			local tmpLink = base.CLuaLinkBox( child )
		   
			if tmpLink:GetModuleType() == link:GetModuleType() and not tmpLink:IsDraggable() then
				guienv:AddLigthing( child, link, "media/textures/larrow.png", 10 )
			end	
		end	
	end
end

local function ShowParams()
	labelCodeVolume:SetText( "Код:" .. project:GetCodeVolume() )
	prgProjectQuality:SetPosition( project:GetCodeQuality() )
end

local function localUpdateProjectWindow( pageName )
    step = pageName
    local gw, gh = projectWindow:GetSize()
	base.CLuaElement( projectWindow ):RemoveChilds()
	button.StretchOne( 0, gh - 30, 30, gh, "", projectWindow, -1, "", "./gameprojectManager.PrevPage()" )
	button.StretchOne( gw - 30, gh - 30, gw, gh, "", projectWindow, -1, "", "./gameprojectManager.NextPage()" )
end

local function localShowChangeNamePage()
    localUpdateProjectWindow( "name" )
    local gw, gh = projectWindow:GetSize()
	editGameName = guienv:AddEdit( "Название игры", gw / 2 + 20, gh / 2, gw - 20,  gh / 2 + 40,  -1, projectWindow )
	editGameName:SetFont( "font_16" )
end

local function localShowAvaibleEngines()
	local maxEngine = company:GetEnginesNumber()
	local xoffset = 0
	local rowCount = 0	
	
	for i=1, maxEngine do
		local gameeng = company:GetEngine( i-1 )
		local linkModule = guienv:AddLinkBox( "Движок ".. i .."/"..maxEngine .. "\r(" .. gameeng:GetName() .. ")", 
															scrWidth / 2 + xoffset, sizeLinkBox * (rowCount+1), 
															scrWidth / 2 + xoffset + sizeLinkBox, sizeLinkBox * (rowCount+2), 
															-1, projectWindow )
		localSetLinkBoxOption( linkModule, base.PT_GAMEENGINE, gameeng, 
							   gameeng:GetTexture(), true, not project:IsMyGameEngine( gameeng ) )
							   
		linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
		rowCount = rowCount + 1
		if rowCount * 50 > 450 then
			xoffset = 0
			rowCount = 0
		end
	end
end

local function localShowEnginePage()
	localUpdateProjectWindow( "engine" )
	localShowAvaibleEngines()

	local ge = project:GetGameEngine()
	--create linkbox for gameEngine
	local linkModule = guienv:AddLinkBox( "", 80, 80, 80 + 80, 80 + 80, -1, projectWindow )
	localSetLinkBoxOption( linkModule, base.PT_GAMEENGINE, ge, ge:GetTexture(), 
	                       false, true, "media/buttons/GameNoEngine.png" )
	--set function for resolve input for linkbox	
	SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetVideoEngine()" )
	
	linkModule = guienv:AddLinkBox( "Продолжение", 80, 200, 80 + 80, 200 + 80, -1, projectWindow )
	localSetLinkBoxOption( linkModule, base.PT_GAME, nil, "", false, true )
	
	ShowParams()
end

local function ShowAvaibleGenreModules()
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
													  scrWidth / 2 + sizeLinkBox, yrOffset + (showedTech + 1) * sizeLinkBox, -1, projectWindow )
													  
				localSetLinkBoxOption( linkModule, base.PT_GENRE, tech, tech:GetTexture(), 
									   true, not project:IsGenreIncluded( tech ), "media/buttons/genreNoImage2.png" )	
									   
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				
				if project:GetGameEngine():IsMyGenre( tech ) then
					linkModule:SetAlpha( 0xff )
				else
					linkModule:SetAlpha( 0x80 )
				end
				showedTech = showedTech + 1
				yrOffset = yrOffset + 10
			end
		end
	end	
end

local function localShowGenrePage()
	localUpdateProjectWindow( "genre" )
	ShowAvaibleGenreModules()
	
	local maxModuleNumber = project:GetGenreModuleNumber()

	for i=1, maxModuleNumber do
		local genre = project:GetGenre( i-1 )
		local linkModule = guienv:AddLinkBox( "Модуль " .. i .. "/" .. maxModuleNumber.."\n"..genre:GetName(), 
												 80, 20 + (i-1) * sizeLinkBox, 
												 80 + sizeLinkBox, 20 + i * sizeLinkBox, 
												 9100+(i-1), projectWindow )
		localSetLinkBoxOption( linkModule, base.PT_GENRE, genre, genre:GetTexture(), 
						       false, true, "media/buttons/genreNoImage2.png" )		
		
		SetLuaFuncToLinkBox( linkModule, "./gameprojectManager.SetGenre()" )
	end
end

local function ShowAvaibleVideoQualityAndVideoTech()
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then		
			if tg == base.PT_VIDEOTECH or tg == base.PT_VIDEOQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 20 + showedTech * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 20 + (showedTech + 1) * sizeLinkBox, -1, projectWindow )
													  
				localSetLinkBoxOption( linkModule, tg, tech, tech:GetTexture(), 
									   true, not project:IsTechInclude( tech ), "" )			

				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedTech = showedTech + 1
			end
		end
	end	
	--Log("SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function ShowAvaibleSoundQualityAndSoundTech()
	local companyName = company:GetName()
	local showedTech = 0

	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == base.PT_SOUNDTECH or tg == base.PT_SOUNDQUALITY then
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 200 + showedTech * sizeLinkBox, 
													   scrWidth / 2 + sizeLinkBox, 200 + (showedTech +1) * sizeLinkBox, -1, projectWindow )
													   
				localSetLinkBoxOption( linkModule, tg, tech, tech:GetTexture(), 
									   true, not project:IsTechInclude( tech ), "" )	
									   								   
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedTech = showedTech + 1
			end
		end
	end	
	Log( "SCRIPT-CREATEVTP:ShowAvaibleSoundQualityAndSoundTech public = " .. showedTech )
end

local function localShowSoundPage()
	localUpdateProjectWindow( "sound" )
	ShowAvaibleSoundQualityAndSoundTech()
		
	local sq = project:GetSoundQuality()
	local linkModule = guienv:AddLinkBox( "", sizeLinkBox, 20, sizeLinkBox + sizeLinkBox, 20 + sizeLinkBox, -1, projectWindow )
	
	localSetLinkBoxOption( linkModule, base.PT_SOUNDQUALITY, sq, sq:GetTexture(), 
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
											xoffset + sizeLinkBox, 120 + sizeLinkBox + showeddLinks * sizeLinkBox, 
											9500 + i, projectWindow )
		localSetLinkBoxOption( linkAdv, base.PT_SOUNDTECH, tech, tech:GetTexture(), 
						       false, true, "media/buttons/soundTechNoImage.png" )

		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetSoundTech()" )
		showeddLinks = showeddLinks + 1
		
		if (20 + showeddLinks * sizeLinkBox) > 500 then
			showeddLinks = 0
			xoffset = xoffset + sizeLinkBox + 10
		end
	end	
	ShowParams()
end

local function localShowVideoPage()
	localUpdateProjectWindow( "video" )
	ShowAvaibleVideoQualityAndVideoTech()
	
	local vq = project:GetVideoQuality()
	local linkModule = guienv:AddLinkBox( "", 80, 20, 80 + sizeLinkBox, 20 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkModule, base.PT_VIDEOQUALITY, vq, vq:GetTexture(), 
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
											xoffset + sizeLinkBox, 120 + (showeddLinks + 1) * sizeLinkBox, 
											9400 + i, projectWindow )

		localSetLinkBoxOption( linkAdv, base.PT_VIDEOTECH, tech, tech:GetTexture(),		
							   false, true, "media/buttons/videoTechNoImage.png" )
		if tech:Empty() == 0 then linkAdv:SetText( tech:GetName() )	end
		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetVideoTech()" )
		
		showeddLinks = showeddLinks + 1
		
		if (20 + showeddLinks * sizeLinkBox) > 500 then
			showeddLinks = 0
			xoffset = xoffset + 90
		end
	end	
	ShowParams()
end

local function localShowEndPage()
	localUpdateProjectWindow( "end" )
	
	local pw, ph = projectWindow:GetSize()
	local sizel = "10%+"
	local linkModule = guienv:AddLinkBox( base.STR_ENGINE, "55%", "20%", sizel, sizel, -1, projectWindow )
	linkModule:SetData( project:GetGameEngine() )
	linkModule:SetTexture( project:GetGameEngine():GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_GENRE, "55%", "40%", sizel, sizel, -1, projectWindow ) )
	linkModule:SetData( project:GetGenre( 0 ) )
	linkModule:SetTexture( project:GetGenre( 0 ):GetTexture() )
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_CONTENT, "55%", "60%", sizel, sizel, -1, projectWindow ) )
	if project:HaveLicense() or project:HaveScenario() then 
	   if project:HaveLicense() then
			linkModule:SetTexture( project:GetLicense():GetTexture() ) 
	   else
			linkModule:SetTexture( project:GetScenario():GetTexture() ) 
	   end
	end
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_PLATFORMS, "75%", "20%", sizel, sizel, -1, projectWindow ) )
	if project:GetPlatformsNumber() > 0 then 
		linkModule:SetData( linkModule  ) 
	end
	
	linkModule:SetObject( guienv:AddLinkBox( base.STR_LOCALIZATION, "75%", "40%", sizel, sizel, -1, projectWindow ) )
	if project:GetLanguagesNumber() > 0 then 
		linkModule:SetData( linkModule ) 
	end
	
	local button = guienv:AddButton( "55%", "80%", "95%", "90%", projectWindow, -1, base.STR_FINISH )
	button:SetAction( "./gameprojectManager.CreateProjectGame()" )	
	button:SetEnabled( project:IsProjectReady() )
end

function CreateProjectGame()
	project:SetName( gameName ) 
	
	if company:CreateDevelopGame( project ) ~= nil then
		--company:AddToPortfelle( prj )
		--sworkUpdateCompanyPortfelle()
		guienv:MessageBox( "Вы завершили создание проекта игры " .. gameName, false, false, "", "" )
	else
		guienv:MessageBox( "Уже есть проект с таким именем", false, false, "", "" )
	end
end

local function ShowAvaibleScriptAndMiniGames()
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
													  scrWidth / 2 + sizeLinkBox, 20 + (showeddLinks + 1 ) * sizeLinkBox, -1, projectWindow )
													  
				localSetLinkBoxOption( linkModule, tg, tech, tech:GetTexture(), 
									   true, not project:IsTechInclude( tech ), "" )												  
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showeddLinks = showeddLinks + 1
			end
		end
	end	
	--Log("SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

local function localShowTechPage()
	localUpdateProjectWindow( "tech" )
	ShowAvaibleScriptAndMiniGames()
	
	local se = project:GetScriptEngine()
	local linkScript = guienv:AddLinkBox( "Скрипты", 20, 20, 20 + sizeLinkBox, 20 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkScript, base.PT_SCRIPTS, se, se:GetTexture(), 
	                       false, true, "media/buttons/scriptsNoImage.png" )
	                       
	if se:Empty() == 0 then linkScript:SetText( se:GetName() )	end
	SetLuaFuncToLinkBox( linkScript, "./gameprojectManager.SetScriptEngine()" )
	
	local mg = project:GetMiniGameEngine()
	local linkMiniGames = guienv:AddLinkBox( "", 20, 120, 20 + sizeLinkBox, 120 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkMiniGames, base.PT_MINIGAME, mg, mg:GetTexture(), 
	                       false, true, "media/buttons/minigameNoImage.png" )

	if mg:Empty() == 0 then	linkMiniGames:SetText( mg:GetName() ) end
	SetLuaFuncToLinkBox( linkMiniGames, "./gameprojectManager.SetMiniGameEngine()" )


	local ph = project:GetPhysicEngine()
	local linkPhis = guienv:AddLinkBox( "", 20, 220, 20 + sizeLinkBox, 220 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkPhis, base.PT_PHYSIC, ph, ph:GetTexture(), 
	                       false, true, "media/buttons/physicNoImage.tga" )

	if ph:Empty() == 0 then linkPhis:SetText( ph:GetName() ) end
	SetLuaFuncToLinkBox( linkPhis, "./gameprojectManager.SetPhysicEngine()" )

	local showeddLinks = 0
	local xoffset = 120
	local maxProjectAdvTech = project:GetGameEngine():GetAdvancedTechNumber()
    
	for i=1, maxProjectAdvTech do
		local tech = project:GetAdvTech( i-1 )
		local linkAdv = guienv:AddLinkBox( tech:GetName(), xoffset, 20 + showeddLinks * sizeLinkBox, 
										   xoffset + sizeLinkBox, 20 + (showeddLinks + 1) * sizeLinkBox, 9200 + i, projectWindow )

		localSetLinkBoxOption( linkAdv, base.PT_ADVTECH, tech, tech:GetTexture(), 
	                           false, true, "media/buttons/advTechNoImage.png" )

		SetLuaFuncToLinkBox( linkAdv, "./gameprojectManager.SetAdvTech()" )
		showeddLinks = showeddLinks + 1
		
		if showeddLinks * sizeLinkBox > 300 then
			showeddLinks = 0
			xoffset = xoffset + sizeLinkBox
		end
	end
	ShowParams()
end

local function localShowPlatformPage()
	localUpdateProjectWindow( "platform" )

	local ge = project:GetGameEngine()
	local sw, sh = projectWindow:GetSize()
	picFlowLang = guienv:AddPictureFlow( 20, 20, sw - 20, sh / 2 - 20, -1, projectWindow )	
	picFlowPlatform = guienv:AddPictureFlow( 20, sh / 2 + 20, sw - 20, sh - 20, -1, projectWindow )
		
	for i=1, applic:GetTechNumber() do
		local tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == base.PT_LANGUAGE then
			picFlowLang:AddItem( tech:GetTexture(), tech:GetName(), tech )
			
			if not project:IsMyLanguage( tech ) then
				picFlowLang:SetItemBlend( index, 0xC0 )
			else
				picFlowLang:SetItemBlend( index, 0xFF )
			end
		end
	end
	
	for i=1, applic:GetPlatformNumber() do
		local platform = applic:GetPlatform( i-1 )
		local index = picFlowPlatform:AddItem( platform:GetTexture(), platform:GetName(), platform )
		
		if not project:IsMyPlatform( platform ) then
			picFlowPlatform:SetItemBlend( index, 0xC0 )
		else
			picFlowPlatform:SetItemBlend( index, 0xFF )
		end
	end
end

local function ShowAvaibleScenarioAndLicense()
	local companyName = applic:GetPlayerCompany():GetName()
	local maxScenarioNum = applic:GetTechNumber()
	local showedLinks = 0
	
	
	for i=1, maxScenarioNum do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if techCompany:Empty() == 1 or techCompany:GetName() == companyName then
			if tg == base.PT_SCENARIO or tg == base.PT_LICENSE then
				Log("SCRIPT:ShowAvaibleScenarioAndLicense public scenario = " .. tech:GetName() )
				
				local linkModule = guienv:AddLinkBox( tech:GetName(), scrWidth / 2, 10 + showedLinks * sizeLinkBox, 
													  scrWidth / 2 + sizeLinkBox, 10 + (showedLinks + 1) * sizeLinkBox, -1, projectWindow )
													   
				localSetLinkBoxOption( linkModule, tg, tech, tech:GetTexture(), true, true, "" )
									  
				if tg == base.PT_SCENARIO then
					linkModule:SetEnabled( not project:IsTechInclude( tech ) ) 
				else
					linkModule:SetEnabled( not project:IsLicenseIncluded( tech:GetName() ) ) 
				end
				
				linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
				showedLinks = showedLinks + 1
			end
		end
	end
end

local function localShowScenarioPage()
	localUpdateProjectWindow( "scenario" )
	ShowAvaibleScenarioAndLicense()
	--Log("SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = project:GetScenario()
	local linkScenario = guienv:AddLinkBox( base.STR_SCENARIO, 100, 40, 100 + sizeLinkBox, 40 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkScenario, base.PT_SCENARIO, tech, tech:GetTexture(),		
						   false, not project:HaveLicense(), "media/buttons/scenarioNoImage.jpg" )
	if tech:Empty() == 0 then linkScenario:SetText( tech:GetName() ) end
	linkScenario:SetDraggable( false )
	SetLuaFuncToLinkBox( linkScenario, "./gameprojectManager.SetScenario()" )
	
	local lic = project:GetLicense()
	local linkLicense = guienv:AddLinkBox( base.STR_LICENSE, 100, 200, 100 + sizeLinkBox, 200 + sizeLinkBox, -1, projectWindow )
	localSetLinkBoxOption( linkLicense, base.PT_LICENSE, lic, lic:GetTexture(),		
						   false, not project:HaveScenario(), "media/buttons/licenseNoImage.jpg" )
						   
	if lic:Empty() == 0 then linkLicense:SetText( lic:GetName() ) end
	linkLicense:SetVisible( not project:HaveScenario() )
	SetLuaFuncToLinkBox( linkLicense, "./gameprojectManager.SetLicense()" )
	
	--Log( "SCRIPT-CREATESL:CreateScenarioLicensePage end " )
	ShowParams()
end

function SetVideoEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetGameEngine( sender:GetData() )
	localShowEnginePage()
end

function SetVideoQuality()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetVideoQuality( sender:GetData() )
	localShowVideoPage()
end

function SetSoundQuality()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetSoundQuality( sender:GetData() )
	localShowSoundPage()
end

function SetVideoTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() % 100
	project:SetVideoTech( sender:GetData(), id )
	localShowVideoPage()
end

function SetSoundTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() % 100
	project:SetSoundTech( sender:GetData(), id )
	localShowSoundPage()
end

function SetScriptEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetScriptEngine( sender:GetData() )
	localShowTechPage()
end

function SetMiniGameEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetMiniGameEngine( sender:GetData() )
	localShowTechPage()
end

function SetPhysicEngine()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetPhysicEngine( sender:GetData() )
	localShowTechPage()
end

function SetAdvTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = (sender:GetID() % 100) - 1
	project:SetAdvTech( sender:GetData(), id )
	localShowTechPage()
end

function SetScenario()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetScenario( sender:GetData() )
	localShowScenarioPage()
end

function SetGenre()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	local id = sender:GetID() - 9100
	project:SetGenre(sender:GetData(), id )
	localShowGenrePage()
end

local function localShowMistakeForPlatform( video, sound, cpuPercent, memPercent )
	guienv:MessageBox( "Not all tech from engine are avaible", false, false, "", "" )
end

local function localCheckPlatformsForProject( pl )
	local result = true
	local vTechAv = {}
	for i=1, project:GetVideoTechNumber() do
		local tech = project:GetVideoTech( i-1 )
		vTechAv[ i ] = { name=tech:GetName(), avaible=false }
		vTechAv[ i ].avaible = pl:IsTechAvaible( tech )
		result = result and vTechAv[ i ].avaible
	end
	
	local sTechAv = {}
	for i=1, project:GetSoundTechNumber() do
		local tech = project:GetSoundTech( i-1 )
		sTechAv[ i ] = { name=tech:GetName(), avaible=false }
		sTechAv[ i ].avaible = pl:IsTechAvaible( tech )
		result = result and sTechAv[ i ].avaible
	end
	
	local indexCpu = project:GetCpuUse() / pl:GetCpu()
	local indexMem = project:GetMemoryUse() / pl:GetRam()
	
	if not result then localShowMistakeForPlatform( vTechAv, sTechAv, indexCpu, indexMem ) end
	return result
end

function ItemSelected()
	local lbx = base.CLuaPictureFlow( base.NrpGetSender() )
	local selIndex = lbx:GetSelected()
	
	if picFlowLang:Self() == lbx:Self() then
		local lang = base.CLuaTech( lbx:GetSelectedObject() )
		
		--уже есть язык, надо убрать его из списка
		if project:IsMyLanguage( lang ) then 
			project:RemoveLanguage( lang )
			lbx:SetItemBlend( selIndex, 0xC0 )	
		--такого языка нет в игре, надо бы добавить
		else 
			project:AddLanguage( lang, nil )	
			lbx:SetItemBlend( selIndex, 0xff )
		end		
	elseif picFlowPlatform:Self() == lbx:Self() then
		local platform = base.CLuaPlatform( lbx:GetSelectedObject() )
		
		--такая платформа заявлена в игре, игрок хочет её убрать
		if project:IsMyPlatform( platform ) then
			project:RemovePlatform( platform )
			lbx:SetItemBlend( selIndex, 0xC0 )				
		else
			if localCheckPlatformsForProject( platform ) then
				project:AddPlatform( platform, nil )
				lbx:SetItemBlend( selIndex, 0xff )			
			end
		end	
	end
end

function Hide()
	project:Remove()
	mainWindow:Remove()
	mainWindow = nil
end

function Show()
	company = applic:GetPlayerCompany()
	if mainWindow == nil then
		project = base.CLuaGameProject():Create( "defaultGame" )
		
		mainWindow = guienv:AddWindow( "media/maps/director_cabinet_slider.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 60, scrWidth - 20, scrHeight, 
		 			    "button_poweroff", mainWindow, -1, "",
						"./gameprojectManager.Hide()" )
		
		projectWindow = guienv:AddWindow( "media/maps/newProject.png", 60, 90, scrWidth - 60, scrHeight - 30, -1, mainWindow )
		projectWindow:SetDraggable( false )
		projectWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameprojectManager.LeftMouseButtonUp()" )
		projectWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./gameprojectManager.ItemSelected()" )
	end 
	
	prgProjectQuality = guienv:AddProgressBar( mainWindow, 10, 40, 10 + 140, 40 + 20, -1 )
	labelCodeVolume = guienv:AddLabel( "Код", scrWidth - 150, 40, scrWidth - 10, 40 + 20, -1, mainWindow )

	localShowChangeNamePage()	
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
		local link = base.CLuaLinkBox( elm ) 
		local linkRecv = base.CLuaLinkBox( guienv:GetDragObject() )
		
		if link:IsDraggable() then
			if link:IsEnabled() then
				guienv:SetDragObject( link )
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

function NextPage()
	if step == "name" then 
		gameName = editGameName:GetText()
		localShowEnginePage(); 
		return 
	elseif step == "engine" then localShowGenrePage(); return 
	elseif step == "genre" then localShowScenarioPage(); return
	elseif step == "scenario" then localShowTechPage(); return
	elseif step == "tech" then localShowVideoPage(); return 
	elseif step == "video" then localShowSoundPage(); return
	elseif step == "sound" then localShowPlatformPage(); return
	elseif step == "platform" then localShowEndPage(); return end
end

function PrevPage()
	if step == "engine" then 
		localShowChangeNamePage()
		editGameName:SetText( gameName )
		return 
	elseif step == "genre" then localShowEnginePage(); return
	elseif step == "scenario" then localShowGenrePage(); return
	elseif step == "tech" then localShowScenarioPage(); return 
	elseif step == "video" then localShowTechPage(); return
	elseif step == "sound" then localShowVideoPage(); return
	elseif step == "platform" then localShowSoundPage(); return 
	elseif step == "end" then localShowPlatformPage(); return end
end
