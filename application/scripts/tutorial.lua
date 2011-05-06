local base = _G

IncludeScript( "tutorialCity" )
IncludeScript( "tutorialLab" )
IncludeScript( "tutorialDirector" )
IncludeScript( "tutorialProjectSelect" )
IncludeScript( "tutorialOffice" )
IncludeScript( "tutorialPlant" )
IncludeScript( "tutorialUniver" )
IncludeScript( "tutorialMedia" )
IncludeScript( "tutorialBank" )
IncludeScript( "tutorialNewGame" )
IncludeScript( "tutorialDevRoom" )
IncludeScript( "tutorialReklame" )
IncludeScript( "tutorialShop" )
IncludeScript( "tutorialProjectManager" )

module('tutorial')

local guienv = base.guienv
local browser = base.browser
local table = base.table
local currentStep = nil

startGameWithTutorial = true
STEP_OVERVIEW_RECEPTION = 1
STEP_CREATE_NEW_PROJECT = 2
STEP_OVERVIEW_DIRECTORS_ROOM = 3
STEP_OVERVIEW_UNIVER = 4
STEP_OVERVIEW_LABORATORY = 5
STEP_OVERVIEW_PLANT	= 6
STEP_OVERVIEW_CITY=7
STEP_OVERVIEW_REKLAME=8
STEP_OVERVIEW_BANK=9
STEP_OVERVIEW_DEVELOPERS_ROOM = 10
STEP_OVERVIEW_REKLAMECAMPANY = 11
STEP_OVERVIEW_SHOP = 12
STEP_OVERVIEW_PROJECTMANAGER = 13

steps = {}

steps[ STEP_OVERVIEW_RECEPTION ] = "media/html/TutorialReception.htm"
steps[ STEP_OVERVIEW_DIRECTORS_ROOM ] = "media/html/TutorialDirector.htm"
steps[ STEP_CREATE_NEW_PROJECT ] = "media/html/TutorialSelectProject.htm"
steps[ STEP_OVERVIEW_UNIVER ] = "media/html/TutorialUniver.htm"
steps[ STEP_OVERVIEW_LABORATORY ] = "media/html/TutorialLab.htm"
steps[ STEP_OVERVIEW_PLANT ] = "media/html/TutorialPlant.htm"
steps[ STEP_OVERVIEW_CITY ] = "media/html/TutorialCity.htm"
steps[ STEP_OVERVIEW_REKLAME ] = "media/html/TutorialMedia.htm" 
steps[ STEP_OVERVIEW_BANK ] = "media/html/TutorialBank.htm"
steps[ STEP_OVERVIEW_DEVELOPERS_ROOM ] = "media/html/TutorialDevRoom.htm"
steps[ STEP_OVERVIEW_REKLAMECAMPANY ] = "media/html/TutorialReklameCampany.htm"
steps[ STEP_OVERVIEW_SHOP ] = "media/html/TutorialShop.htm"
steps[ STEP_OVERVIEW_PROJECTMANAGER ] = "media/html/TutorialProjectManager.htm"

local function _ShowPage()
	browser:Show()
		
	if steps[ currentStep ] ~= nil then
		browser:Navigate( steps[ currentStep ] )
	else
		browser:Navigate( steps[ NO_STEP ] )	
	end	
end

function Update( step )
	currentStep = step
	
	base.LogScript( steps[ currentStep ] )

	if not startGameWithTutorial then
		return
	end

	_ShowPage()
end

function ShowCurrentHelp()
	_ShowPage()
end