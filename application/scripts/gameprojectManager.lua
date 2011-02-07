local base = _G

module( "gameprojectManager" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local project = nil
local projectWindow = nil
local company = nil
local labelCodeVolume = nil
local gameName = "withoutName"
local editGameName = nil
local prgProjectQuality = nil

-- "name" "engine"
local step = "name"
local sizeLinkBox = 80
local mainWindow = nil
local links = nil
local dragLink = nil
local lastData = nil

local REMOVE = true
local ADD = false

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
	Log( "localCreateLigthingBetweenElements elements"..#links )
	--проидемся по вкладке
	for i=1, #links do
		--проверим их на соответствие с заданным типом
		if links[ i ]:GetModuleType() == link:GetModuleType() and not links[ i ]:IsDraggable() then
				guienv:AddLigthing( links[ i ], link, "media/textures/sphere.png", 10 )		
	    end	
	end
end

local function ShowParams()
	labelCodeVolume:SetText( "Код:" .. project:GetCodeVolume() )
	prgProjectQuality:SetPosition( project:GetCodeQuality() )
end

local function localUpdateProjectWindow( pageName )
    step = pageName
    links = nil --удаляем старую таблицу
    links = {} --создаем пустую таблицу
    local gw, gh = projectWindow:GetSize()
	base.CLuaElement( projectWindow ):RemoveChilds()
	button.StretchOne( 0, gh - 30, 30, gh, "", projectWindow, -1, "", "./gameprojectManager.PrevPage()" )
	button.StretchOne( gw - 30, gh - 30, gw, gh, "", projectWindow, -1, "", "./gameprojectManager.NextPage()" )
end

local function localShowChangeNamePage()
    localUpdateProjectWindow( "name" )
	editGameName = guienv:AddEdit( "Название игры", "55%", "50%", "90%", "60%", -1, projectWindow )
	editGameName:SetFont( "font_16" )
end

local function FlickLinkBox( parentr, textr, group, datar, draggable, enabled, lmouseAct, rmouseAct, defImage, luaFunc )
	
	local linkModule = guienv:AddLinkBox( textr, 0, 0, sizeLinkBox, sizeLinkBox, -1, parentr )
	
	local texture = ""
	if datar then
		texture = datar:GetTexture()
	end
	
	localSetLinkBoxOption( linkModule, group, datar, texture, draggable, enabled, defImage )
	
	if lmouseAct then
		linkModule:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, lmouseAct )
	end
	
	if rmouseAct then
		linkModule:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, lrouseAct )
	end
	
	if luaFunc then
		SetLuaFuncToLinkBox( linkModule, luaFunc )
	end
	
	base.table.insert( links, linkModule )
	
	return linkModule
end

local function localShowAvaibleEngines()
	local maxEngine = company:GetEnginesNumber()
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 4, -1, projectWindow )
	
	for i=1, maxEngine do
		local gameeng = company:GetEngine( i-1 )
		FlickLinkBox( flick, base.string.format( "Движок %d/%d \r( %s )", i, maxEngine, gameeng:GetName() ),
					  gameeng:GetTechGroup(), gameeng, true, not project:IsMyGameEngine( gameeng ), 
					  "./gameprojectManager.LeftMouseButtonUp()", nil, nil, nil )			   
	end
end

local function localShowEnginePage()
	localUpdateProjectWindow( "engine" )
	localShowAvaibleEngines()

	local ge = project:GetGameEngine()
	--create linkbox for gameEngine
	
	local lnk = FlickLinkBox( projectWindow, "",  base.PT_GAMEENGINE, ge, false, true, 
							  nil, nil,
							  "media/buttons/GameNoEngine.png",
							  "./gameprojectManager.SetVideoEngine()" )
	lnk:SetPosition( "25%", "33%" )
	
	lnk = FlickLinkBox( projectWindow, "Продолжение", base.PT_GAME, nil, false, true, 
						nil, nil, nil, nil )
	lnk:SetPosition( "25%", "50%" )
	
	ShowParams()
end

local function ShowAvaibleGenreModules()
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1) 
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if (techCompany:Empty() == 1 or techCompany:GetName() == companyName) and tech:GetTechGroup() == base.PT_GENRE then
			local lnk = FlickLinkBox( flick, tech:GetName(), tech:GetTechGroup(), tech, 
									  true, not project:IsMyTech( tech ),
									  "./gameprojectManager.LeftMouseButtonUp()", nil,  
									  "media/buttons/genreNoImage2.png" )												  
						
			if project:GetGameEngine():IsMyTech( tech ) then
				lnk:SetAlpha( 0xff )
			else
				lnk:SetAlpha( 0x80 )
			end
		end
	end	
end

local function localShowGenrePage()
	localUpdateProjectWindow( "genre" )
	ShowAvaibleGenreModules()
	
	local maxModuleNumber = project:GetGameEngine():GetGenreModuleNumber()
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )

	for i=1, maxModuleNumber do
		local genre = project:GetTech( i-1, base.PT_GENRE )
		FlickLinkBox( flick, "Модуль " .. i .. "/" .. maxModuleNumber.."\n"..genre:GetName(), 
					  base.PT_GENRE, genre, 
					  false, true, nil, nil,  
					  "media/buttons/genreNoImage2.png",
					  "./gameprojectManager.SetGenre()" )
	end
end

local function ShowAvaibleVideoQualityAndVideoTech()
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if (techCompany:Empty() == 1 or techCompany:GetName() == companyName) and 
			( tg == base.PT_VIDEOTECH or tg == base.PT_VIDEOQUALITY ) then
			 FlickLinkBox( flick, tech:GetName(), tech:GetTechGroup(), tech, 
						   true, not project:IsMyTech( tech ),
						   "./gameprojectManager.LeftMouseButtonUp()", nil, 
						   nil )
		end
	end	
	--Log("SCRIPT-CREATEVTP:ShowAvaibleVideoQualityAndVideoTech public = " .. showedTech )
end

local function ShowAvaibleSoundQualityAndSoundTech()
	local companyName = company:GetName()
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		local tg = tech:GetTechGroup()
		local techCompany = tech:GetCompany()
		
		if (techCompany:Empty() == 1 or techCompany:GetName() == companyName) and 
			(tg == base.PT_SOUNDTECH or tg == base.PT_SOUNDQUALITY ) then
				 FlickLinkBox( flick, tech:GetName(), tech:GetTechGroup(), tech, 
						   true, not project:IsMyTech( tech ),
						   "./gameprojectManager.LeftMouseButtonUp()", nil, 
						   nil )
		end
	end	
end

local function localShowSoundPage()
	localUpdateProjectWindow( "sound" )
	ShowAvaibleSoundQualityAndSoundTech()
		
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	
	local sq = project:GetSoundQuality()
	FlickLinkBox( flick, sq:GetName(), base.PT_SOUNDQUALITY, sq, false, true,
				  nil, nil, 
				  "media/buttons/SoundQualityNoImage.png",
				  "./gameprojectManager.SetSoundQuality()" )

	local maxProjectSoundTech = project:GetNumber( base.PT_SOUNDTECH )
	
	for i=0, maxProjectSoundTech do
		local tech = project:GetTech( i, base.PT_SOUNDTECH )
		local name = "Sound " .. i .. "/" .. maxProjectSoundTech
		
		if tech:Empty() == 0 then
			name = tech:GetName()
		end

		FlickLinkBox( flick, name, base.PT_SOUNDTECH, tech, false, true,
					  nil, nil, 
					  "media/buttons/soundTechNoImage.png",
					  "./gameprojectManager.SetSoundTech()" )
	end	
	ShowParams()
end

local function localShowVideoPage()
	localUpdateProjectWindow( "video" )
	ShowAvaibleVideoQualityAndVideoTech()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	
	local vq = project:GetVideoQuality()
	FlickLinkBox( flick, vq:GetName(), base.PT_VIDEOQUALITY, vq, false, true,
				  nil, nil, 
				  "media/buttons/SoundQualityNoImage.png",
				  "./gameprojectManager.SetVideoQuality()" )
							  	
	local maxProjectVideoTech = project:GetNumber( base.PT_VIDEOTECH )
	
	for i=0, maxProjectVideoTech do
		local tech = project:GetTech( i, base.PT_VIDEOTECH )
		local name = "Видео " .. i .. "/" .. maxProjectVideoTech
		
		if tech:Empty() == 0 then name = tech:GetName() end
		FlickLinkBox( flick, name,
					  base.PT_VIDEOTECH, tech, false, true, 
					  nil, nil,
					  "media/buttons/videoTechNoImage.png",
					  "./gameprojectManager.SetVideoTech()" )
	end	

	ShowParams()
end

local function localShowEndPage()
	localUpdateProjectWindow( "end" )

	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	FlickLinkBox( flick, base.STR_ENGINE, 0, project:GetGameEngine(),
				  false, false, nil, nil, nil, nil )
	
	FlickLinkBox( flick, base.STR_GENRE, 0, project:GetTech( 0, base.PT_GENRE ), 
				  false, false, nil, nil, nil, nil ) 
	
	local lnk = FlickLinkBox( flick, base.STR_CONTENT, 0, nil,
							   false, false, nil, nil, nil, nil )
	if project:HaveLicense() or project:HaveScenario() then 
	   if project:HaveLicense() then
			lnk:SetTexture( project:GetLicense():GetTexture() ) 
	   else
			lnk:SetTexture( project:GetScenario():GetTexture() ) 
	   end
	end
	
	lnk = FlickLinkBox( flick, base.STR_PLATFORMS, 0, nil, 
						 false, false, nil, nil, nil, nil )
						 
	if project:GetNumber( base.PT_PLATFORM ) > 0 then 
		lnk:SetTexture( "media/textures/havePlatform.png"  ) 
	end
	
	lnk = FlickLinkBox( flick, base.STR_LOCALIZATION, 0, nil, 
						 false, false, nil, nil, nil, nil )
	if project:GetNumber( base.PT_LANGUAGE ) > 0 then 
		lnk:SetTexture( "media/textures/haveLangs.png" ) 
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
	local maxPublicTech = applic.techNumber
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	for i=1, maxPublicTech do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if (techCompany:Empty() == 1 or techCompany:GetName() == companyName) and
			(tg == base.PT_SCRIPTS or tg == base.PT_MINIGAME or tg == base.PT_PHYSIC or tg == base.PT_ADVTECH) then	
				FlickLinkBox( flick, tech:GetName(), tech:GetTechGroup(), tech, 
						      true, not project:IsMyTech( tech ),
						      "./gameprojectManager.LeftMouseButtonUp()", nil, 
						      nil )
		end
	end	
	--Log("SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

local function localShowTechPage()
	localUpdateProjectWindow( "tech" )
	ShowAvaibleScriptAndMiniGames()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )

	local se = project:GetScriptEngine()
	local lnk = FlickLinkBox(  flick, "Скрипты", base.PT_SCRIPTS, se,  
							   false, true, 
							   nil, nil, 
							   "media/buttons/scriptsNoImage.png",
							   "./gameprojectManager.SetScriptEngine()"  )
	                       
	if se:Empty() == 0 then lnk:SetText( se:GetName() )	end
	
	local mg = project:GetMiniGameEngine()
	lnk = FlickLinkBox(  flick, "Миниигры", base.PT_MINIGAME, mg,  
						 false, true, 
						 nil, nil, 
						 "media/buttons/minigameNoImage.png",
						 "./gameprojectManager.SetMiniGameEngine()"  )
							   
	if mg:Empty() == 0 then	lnk:SetText( mg:GetName() ) end


	local ph = project:GetPhysicEngine()
	lnk = FlickLinkBox( flick, "Физика", base.PT_PHYSIC, ph,  
						false, true, 
						nil, nil, 
						"media/buttons/physicNoImage.png",
						"./gameprojectManager.SetPhysicEngine()"  )
						 
	if ph:Empty() == 0 then lnk:SetText( ph:GetName() ) end

	local maxProjectAdvTech = project:GetGameEngine():GetTechNumber()    
	for i=1, maxProjectAdvTech do
		local tech = project:GetTech( i-1, base.PT_ADVTECH )
		FlickLinkBox( flick, tech:GetName(), base.PT_ADVTECH, tech, 
					  false, true, 
					  nil, nil, 
					  "media/buttons/advTechNoImage.png",
					  "./gameprojectManager.SetAdvTech()" )
	end
	
	ShowParams()
end

local function localShowPlatformPage()
	localUpdateProjectWindow( "platform" )

	local ge = project:GetGameEngine()
	picFlowLang = guienv:AddPictureFlow( "5%", "5%", "95%", "45%", -1, projectWindow )	
	picFlowPlatform = guienv:AddPictureFlow( "5%", "55%", "95%", "95%", -1, projectWindow )
		
	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == base.PT_LANGUAGE then
			picFlowLang:AddItem( tech:GetTexture(), tech:GetName(), tech )
			
			if not project:IsMyTech( tech ) then
				picFlowLang:SetItemBlend( index, 0xC0 )
			else
				picFlowLang:SetItemBlend( index, 0xFF )
			end
		end
	end
	
	for i=1, applic.platformNumber do
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
	local companyName = applic.playerCompany:GetName()
	local maxScenarioNum = applic.techNumber
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )	
	
	for i=1, maxScenarioNum do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech:GetCompany()
		local tg = tech:GetTechGroup()
		
		if (techCompany:Empty() == 1 or techCompany:GetName() == companyName) and
			( tg == base.PT_SCENARIO or tg == base.PT_LICENSE ) then
				FlickLinkBox( flick, tech:GetName(), tech:GetTechGroup(), tech,
							  true, not project:IsMyTech( tech ), 
							  "./gameprojectManager.LeftMouseButtonUp()", nil,
							  nil, nil )							  
		end
	end
end

local function localShowScenarioPage()
	localUpdateProjectWindow( "scenario" )
	ShowAvaibleScenarioAndLicense()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )
	--Log("SCRIPT-CREATESL:CreateScenarioLicensePage start " )

	local tech = project:GetScenario()
	local lnk = FlickLinkBox( flick, base.STR_SCENARIO, base.PT_SCENARIO, tech, 	
							  false, not project:HaveLicense(), 
							  nil, nil,
							  "media/buttons/scenarioNoImage.jpg",
							  "./gameprojectManager.SetScenario()" )

	if tech:Empty() == 0 then lnk:SetText( tech:GetName() ) end
	lnk:SetVisible( not project:HaveLicense() )
	
	local lic = project:GetLicense()
	local lnk = FlickLinkBox( flick, base.STR_LICENSE, base.PT_LICENSE, lic, 		
							  false, not project:HaveScenario(), 
							  nil, nil, 
							  "media/buttons/licenseNoImage.jpg",
							  "./gameprojectManager.SetLicense()" )
						   
	if lic:Empty() == 0 then linkLicense:SetText( lic:GetName() ) end
	lnk:SetVisible( not project:HaveScenario() )
	
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
	
	if sender:GetData() == nil then
		project:RemoveTech( lastData )
	else 
		project:AddTech( sender:GetData() )
	end
	localShowVideoPage()
end

function SetSoundTech()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
		
	if sender:GetData() == nil then
		project:RemoveTech( lastData )
	else 
		project:AddTech( sender:GetData() )
	end
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

	if sender:GetData() == nil then
		project:RemoveTech( lastData )
	else 
		project:AddTech( sender:GetData() )
	end
	
	localShowTechPage()
end

function SetScenario()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	project:SetScenario( sender:GetData() )
	localShowScenarioPage()
end

function SetGenre()
	local sender = base.CLuaLinkBox( base.NrpGetSender() )
	if sender:GetData() == nil then
		project:RemoveTech( lastData )
	else 
		project:AddTech( sender:GetData() )
	end
	localShowGenrePage()
end

local function localShowMistakeForPlatform( video, sound, cpuPercent, memPercent )
	guienv:MessageBox( "Not all tech from engine are avaible", false, false, "", "" )
end

local function localCheckPlatformsForProject( pl )
	local result = true
	local vTechAv = {}
	for i=1, project:GetNumber( base.PT_VIDEOTECH ) do
		local tech = project:GetTech( i-1, base.PT_VIDEOTECH )
		vTechAv[ i ] = { name=tech:GetName(), avaible=false }
		vTechAv[ i ].avaible = pl:IsMyTech( tech )
		result = result and vTechAv[ i ].avaible
	end
	
	local sTechAv = {}
	for i=1, project:GetNumber( base.PT_SOUNDTECH ) do
		local tech = project:GetTech( i-1, base.PT_SOUNDTECH )
		sTechAv[ i ] = { name=tech:GetName(), avaible=false }
		sTechAv[ i ].avaible = pl:IsMyTech( tech )
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
		if project:IsMyTech( lang ) then 
			project:RemoveTech( lang )
			lbx:SetItemBlend( selIndex, 0xC0 )	
		--такого языка нет в игре, надо бы добавить
		else 
			project:AddTech( lang )	
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
				project:AddPlatform( platform )
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
	company = applic.playerCompany
	if mainWindow == nil then
		project = base.CLuaGameProject():Create( "defaultGame" )
		
		mainWindow = guienv:AddWindow( "media/maps/director_cabinet_slider.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		dragLink = guienv:AddLinkBox( "", 0, 0, sizeLinkBox, sizeLinkBox, -1, mainWindow )
		
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
	lastData = link:GetData()
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
				dragLink:SetData( link:GetData() )
				dragLink:SetTexture( link:GetTexture() )
				dragLink:SetModuleType( link:GetModuleType() )
				guienv:SetDragObject( dragLink )
				dragLink:SetVisible( true )
				
				localCreateLigthingBetweenElements( dragLink )
			end
		else
			if link:GetModuleType() == linkRecv:GetModuleType() then
				link:SetData( linkRecv:GetData() )
				link:SetTexture( linkRecv:GetTexture() )
			end
			
			guienv:SetDragObject( nil )
			dragLink:SetVisible( false )
		end
	else
		guienv:SetDragObject( nil )
		dragLink:SetVisible( false )
	end
end

function NextPage()
	if step == "name" then 
		gameName = editGameName:GetText(); localShowEnginePage(); return 
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
		localShowChangeNamePage(); editGameName:SetText( gameName ); return 
	elseif step == "genre" then localShowEnginePage(); return
	elseif step == "scenario" then localShowGenrePage(); return
	elseif step == "tech" then localShowScenarioPage(); return 
	elseif step == "video" then localShowTechPage(); return
	elseif step == "sound" then localShowVideoPage(); return
	elseif step == "platform" then localShowSoundPage(); return 
	elseif step == "end" then localShowPlatformPage(); return end
end
