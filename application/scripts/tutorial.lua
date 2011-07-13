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
IncludeScript( "tutorialBridge" )

module("tutorial")

local guienv = base.guienv
local browser = base.browser
local table = base.table
local currentStep = nil
local _arrow = nil

startGameWithTutorial = true

local function _ShowPage()
	browser:Show()
		
	if currentStep ~= nil then
		browser:Navigate( currentStep )
	end	
end

function ArrowToElm( elm )
	if _arrow ~= nil then 
		_arrow:Remove()
	end
	
	_arrow = base.button.ArrowToElement( elm, "TutorialArrow.png", 52, 56, 5000 )
	guienv:AddSpringAnimator( _arrow, _arrow.left + 10, _arrow.top - 20, _arrow.right - 10, _arrow.bottom,
								   _arrow.left - 10, _arrow.top + 20, _arrow.right + 10, _arrow.bottom, 1 )
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