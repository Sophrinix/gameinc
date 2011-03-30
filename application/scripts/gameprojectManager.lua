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
gameName = "withoutName"
local editGameName = nil
local prgProjectQuality = nil

-- "name" "engine"
local step = "name"
local mainWindow = nil
local dragLink = nil
local lastData = nil

local REMOVE = true
local ADD = false

function ShowParams()
	labelCodeVolume.text = "Код:" .. project.codeVolume
	prgProjectQuality.position = project.codeQuality
end

function Hide()
	project:Remove()
	
	base.package.loaded[ "linkbox" ] = false
	base.package.loaded[ "gpmFunctions" ] = false
	base.package.loaded[ "gpmScenarioPage" ] = false
	base.package.loaded[ "gpmPlatformPage" ] = false
	base.package.loaded[ "gpmTechPage" ] = false
	base.package.loaded[ "gpmEndPage" ] = false
	base.package.loaded[ "gpmVideoPage" ] = false
	base.package.loaded[ "gpmSoundPage" ] = false
	base.package.loaded[ "gpmEnginePage" ] = false
	base.package.loaded[ "gpmGenrePage" ] = false
end

function UpdateProjectWindow( pageName )
    step = pageName
	base.CLuaElement( projectWindow ):RemoveChilds()
	button.StretchOne( 0, "30e", 30, "0e", "", projectWindow, -1, "", PrevPage )
	button.StretchOne( "30e", "30e", "0e", "0e", "", projectWindow, -1, "", NextPage )
	links = nil
	links = {}
end

local function _ShowChangeNamePage()
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

local function _ClearDragObject()
	guienv:SetDragObject( nil, "" )
end

function Show()
	company = applic.playerCompany

	project = base.CLuaGameProject():Create( "defaultGame" )
		
	local blurTexture = base.driver:CreateBlur( "director_cabinet.png", 2, 4 )	
	mainWindow = window.fsWindow( blurTexture.path, Hide )
		
	projectWindow = guienv:AddWindow( "media/maps/newProject.png", 60, 90, "60e", "30e", -1, mainWindow )
	projectWindow.draggable = false
	--projectWindow:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, _ClearDragObject )
	
	prgProjectQuality = guienv:AddProgressBar( mainWindow, 10, 40, "140+", "20+", -1 )
	labelCodeVolume = guienv:AddLabel( "Код", "150e", 40, "10e", "20+", -1, mainWindow )

	_ShowChangeNamePage()	
	--guienv:FadeAction( base.FADE_TIME, false, false )			
	--guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end