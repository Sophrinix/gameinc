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
IncludeScript( "gameNames" )

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

lbCodeVolume = nil
gameName = "withoutName"
editGameName = nil
lbProjectQuality = nil

-- "name" "engine"
local step = "name"
local mainWindow = nil
local dragLink = nil
local lastData = nil

local REMOVE = true
local ADD = false

local function _CodeVolumeToText( kode )
	if kode < 10000 then
		return kode .. " bytes"
	elseif kode >= 10000 then
		return ( kode / 10000 ) * 10 .. "Kb"
	elseif kode >= 1000000 then
		return ( kode / 1000000 ) .. "Mb"
	elseif kode >= 1000000000 then
		return ( kode / 1000000000 ) .. "Gb"
	end
end

local function _CodeQualityToText( qua )
	if qua == 0 then
		return "А есть ли концепция?"
	elseif qua > 0 and qua < 10 then
		return "Треш"
	elseif qua >= 10 and qua < 25 then
		return "Второсортная игра"
	elseif qua >= 25 and qua < 50 then
		return "Ниже среднего"
	elseif qua >= 50 and qua < 75 then
		return "Середнячок"
	elseif qua >= 75 and qua < 90 then
		return "Крутая игра"
	elseif qua >= 90 and qua <= 100 then
		return "Потенциальный хит"
	end	
end

function ShowParams()
	lbCodeVolume.text = "Сложность: " .. _CodeVolumeToText( project.codeVolume )
	lbProjectQuality.position = "Рейтинг: " .. _CodeQualityToText( project.codeQuality )
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

local function _SwitchName()
	editGameName.text = base.gameNames.GetName()
end

function ShowHelp()
	base.tutorial.Update( "creator/name" )
end

local function _ShowChangeNamePage()
    UpdateProjectWindow( "name" )
	editGameName = guienv:AddEdit( "", "55%", "50%", "95%", "60%", -1, projectWindow )
	editGameName.font = "font_16"
	editGameName.text = base.gameNames.GetName()
		
	local hh = editGameName.height
	button.StretchOne( editGameName.right - hh, editGameName.top, hh.."+", hh.."+", "", projectWindow, -1, "", _SwitchName )
	
	base.rightPanel.AddYesNo( "Хотите узнать больше о выборе имени?", ShowHelp, button.CloseParent )
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

local function _KeyEvent( _, ptr )
	local event = base.CLuaEvent( ptr )

	base.LogScript( "key event "..event.key )
	if  event.keyDown and event.key == 0x1B then
		_ClearDragObject()
	end
end

function Show()
	company = applic.playerCompany

	project = base.CLuaGameProject():Create( "defaultGame" )
		
	local blurTexture = base.driver:CreateBlur( "director_cabinet.png", 2, 4 )	
	mainWindow = window.fsWindow( blurTexture.path, Hide )
		
	projectWindow = guienv:AddWindow( "media/maps/newProject.png", 60, 90, "60e", "30e", -1, mainWindow )
	projectWindow.draggable = false
	projectWindow.onLmbClick = _ClearDragObject
	projectWindow.onKeyEvent = _KeyEvent 
	
	lbProjectQuality = guienv:AddLabel( "Рейтинг:", "25%", 60, "50%", "40+", -1, mainWindow )
	lbCodeVolume = guienv:AddLabel( "Сложность:", "50%", 60, "75%", "40+", -1, mainWindow )

	_ShowChangeNamePage()	
end