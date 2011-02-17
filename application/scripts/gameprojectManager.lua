local base = _G

IncludeScript( "linkbox" )
IncludeScript( "gpmFunctions" )
IncludeScript( "gpmScenarioPage" )
IncludeScript( "gpmPlatformPage" )
IncludeScript( "gpmGenrePage" )
IncludeScript( "gpmTechPage" )
IncludeScript( "gpmEndPage" )
IncludeScript( "gpmVideoPage" )
IncludeScript( "gpmSoundPage" )
IncludeScript( "gpmEnginePage" )

module( "gameprojectManager" )

projectWindow = nil
project = nil
links = nil

local button = base.button
local applic = base.applic
local guienv = base.guienv
local window = base.window
local Log = base.LogScript
local company = nil
local labelCodeVolume = nil
local gameName = "withoutName"
local editGameName = nil
local prgProjectQuality = nil

-- "name" "engine"
local step = "name"
local mainWindow = nil
local dragLink = nil
local lastData = nil

local REMOVE = true
local ADD = false

function FadeEnterAction()
	mainWindow.visible = true
	--guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	--guienv:FadeAction( base.FADE_TIME, true, true )
end

function ShowParams()
	labelCodeVolume.text = "Код:" .. project.codeVolume
	prgProjectQuality.progress = project.codeQuality
end

function Hide()
	project:Remove()
	mainWindow:Remove()
	mainWindow = nil
	
	base.package.loaded[ "linkbox" ] = nil
	base.package.loaded[ "gpmFunctions" ] = nil
	base.package.loaded[ "gpmScenarioPage" ] = nil
	base.package.loaded[ "gpmPlatformPage" ] = nil
	base.package.loaded[ "gpmTechPage" ] = nil
	base.package.loaded[ "gpmEndPage" ] = nil
	base.package.loaded[ "gpmVideoPage" ] = nil
	base.package.loaded[ "gpmSoundPage" ] = nil
	base.package.loaded[ "gpmEnginePage" ] = nil
	base.package.loaded[ "gpmGenrePage" ] = nil
end

local function localShowChangeNamePage()
    UpdateProjectWindow( "name" )
	editGameName = guienv:AddEdit( "Название игры", "55%", "50%", "90%", "60%", -1, projectWindow )
	editGameName.font = "font_16"
end

function NextPage()
	if step == "name" then 
		gameName = editGameName.text; 
		base.gpmEnginePage.Show(); return 
	elseif step == "engine" then base.gpmGenrePage.Show(); return 
	elseif step == "genre" then base.gpmScenarioPage.Show(); return
	elseif step == "scenario" then base.gpmTechPage.Show(); return
	elseif step == "tech" then base.gpmVideoPage.Show(); return 
	elseif step == "video" then base.gpmSoundPage.Show(); return
	elseif step == "sound" then base.gpmPlatformPage.Show(); return
	elseif step == "platform" then base.gpmEndPage.Show(); return end
end

function PrevPage()
	if step == "engine" then 
		localShowChangeNamePage(); 
		editGameName.text = gameName; return 
	elseif step == "genre" then base.gpmEnginePage.Show(); return
	elseif step == "scenario" then base.gpmGenrePage.Show(); return
	elseif step == "tech" then base.gpmScenarioPage.Show(); return 
	elseif step == "video" then base.gpmTechPage.Show(); return
	elseif step == "sound" then base.gpmVideoPage.Show(); return
	elseif step == "platform" then base.gpmSoundPage.Show(); return 
	elseif step == "end" then base.gpmPlatformPage.Show(); return end
end

function UpdateProjectWindow( pageName )
    step = pageName
	base.CLuaElement( projectWindow ):RemoveChilds()
	button.StretchOne( 0, "30e", 30, "0e", "", projectWindow, -1, "", PrevPage )
	button.StretchOne( "30e", "30e", "0e", "0e", "", projectWindow, -1, "", NextPage )
	links = nil
	links = {}
end

function CreateProjectGame()
	project.name = gameName
	
	if company:CreateDevelopGame( project ) ~= nil then
		--company:AddToPortfelle( prj )
		--sworkUpdateCompanyPortfelle()
		guienv:MessageBox( "Вы завершили создание проекта игры " .. gameName, false, false, "", "" )
	else
		guienv:MessageBox( "Уже есть проект с таким именем", false, false, "", "" )
	end
end

function ItemSelected( pSender )
	local lbx = base.CLuaPictureFlow( pSender )
	local selIndex = lbx.itemSelected
	
	if picFlowLang.object == lbx.object then
		local lang = base.CLuaTech( lbx.objectSelected )
		
		--уже есть язык, надо убрать его из списка
		if project:IsMyTech( lang ) then 
			project:RemoveTech( lang )
			lbx:SetItemBlend( selIndex, 0xC0 )	
		--такого языка нет в игре, надо бы добавить
		else 
			project:AddTech( lang )	
			lbx:SetItemBlend( selIndex, 0xff )
		end		
	elseif picFlowPlatform.object == lbx.object then
		local platform = base.CLuaPlatform( lbx.objectSelected )
		
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

function Show()
	company = applic.playerCompany
	if mainWindow == nil then
		project = base.CLuaGameProject():Create( "defaultGame" )
		
		mainWindow = window.fsWindow( "media/maps/director_cabinet_slider.png", Hide )
		
		dragLink = guienv:AddLinkBox( "", 0, 0, sizeLinkBox, sizeLinkBox, -1, mainWindow )
		projectWindow = guienv:AddWindow( "media/maps/newProject.png", 60, 90, "60e", "30e", -1, mainWindow )
		projectWindow.draggable = false
		projectWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, ItemSelected )
	end 
	
	prgProjectQuality = guienv:AddProgressBar( mainWindow, 10, 40, "140+", "20+", -1 )
	labelCodeVolume = guienv:AddLabel( "Код", "150e", 40, "10e", "20+", -1, mainWindow )

	localShowChangeNamePage()	
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end