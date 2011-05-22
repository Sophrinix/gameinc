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

local function _ShowPage()
	browser:Show()
		
	if currentStep ~= nil then
		browser:Navigate( currentStep )
	end	
end

function ArrowToElm( elm )
	elm = base.button.ArrowToElement( elm, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.screenLeft, elm.screenBottom )
end

function Update( step )
	currentStep = "media/html/"..step..".htm"
	
	base.LogScript( currentStep )

	_ShowPage()
end

function ShowCurrentHelp()
	_ShowPage()
end