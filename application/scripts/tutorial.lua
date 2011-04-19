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

module('tutorial')

local guienv = base.guienv
local browser = base.browser
local table = base.table

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

steps = {}

table.insert( steps, STEP_OVERVIEW_RECEPTION, "media/html/TutorialReception.htm" )
table.insert( steps, STEP_OVERVIEW_DIRECTORS_ROOM, "media/html/TutorialDirector.htm" )
table.insert( steps, STEP_CREATE_NEW_PROJECT,"media/html/TutorialSelectProject.htm" )
table.insert( steps, STEP_OVERVIEW_UNIVER, "media/html/TutorialUniver.htm" )
table.insert( steps, STEP_OVERVIEW_LABORATORY, "media/html/TutorialLab.htm" )
table.insert( steps, STEP_OVERVIEW_PLANT, "media/html/TutorialPlant.htm" )
table.insert( steps, STEP_OVERVIEW_CITY, "media/html/TutorialCity.htm" )
table.insert( steps, STEP_OVERVIEW_REKLAME, "media/html/TutorialMedia.htm" )
table.insert( steps, STEP_OVERVIEW_BANK, "media/html/TutorialBank.htm" )
table.insert( steps, STEP_OVERVIEW_DEVELOPERS_ROOM, "media/html/TutorialDevRoom.htm" )

function Update( currentStep )
	base.LogScript( steps[ currentStep ] )
	if not startGameWithTutorial then
		return
	end
	
	base.LogScript( steps[ currentStep ] )
	browser:Show()
	
	if steps[ currentStep ] ~= nil then
		browser:Navigate( steps[ currentStep ] )
	end
	base.LogScript( steps[ currentStep ] )
end