startGameWithTutorial = false
STEP_OVERVIEW_OFFICE = 1
STEP_OVERVIEW_RECEPTION = 2
STEP_CREATE_NEW_PROJECT = 3
STEP_OVERVIEW_DIRECTORS_ROOM = 4
STEP_OVERVIEW_UNIVER = 5

function AddDelayRemImage( x1, y1, x2, y2, textureName, time )
	local image = guienv:AddImage( x1, y1, x2, y2, guienv:GetRootGUIElement(), -1, "" );
	image:SetImage( textureName )
	image:SetScaleImage( true )
	image:SetUseAlphaChannel( true )
	guienv:AddDestructor( image:Self(), time )
end

function UpdateTutorialState( currentStep )
	if not startGameWithTutorial then
		return
	end
	
	browser:Show()
	if currentStep == STEP_OVERVIEW_RECEPTION then
		browser:Navigate( "media/html/TutorialReceptionOverviewEng.htm" )
	elseif currentStep == STEP_OVERVIEW_DIRECTORS_ROOM then
		browser:Navigate( "media/html/TutorialDirectorsRoomEng.htm" )
	elseif currentStep == STEP_CREATE_NEW_PROJECT then
		browser:Navigate( "media/html/TutorialSelectProjectTypeSceneEng.htm" )
	elseif currentStep == STEP_OVERVIEW_UNIVER then
		browser:Navigate( "media/html/TutorialUniverSceneEng.htm" )
	else
	end
end

function StartDescriptionGame()
	browser:Show()
	browser:Navigate( "media/html/TutorialCityMapOverviewEng.htm" )
end

function tutorial_CityMapShowOffice() 
	AddDelayRemImage( 730, 50, 730 + 128, 50 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowWhereStartProjectButton()
	AddDelayRemImage( 93, 265, 93 + 128, 265 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowWhereManageEmployersButton()
	AddDelayRemImage( 850, 300, 850 + 128, 300 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowWhereComputerButton()
	AddDelayRemImage( 653, 278, 653 + 128, 278 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_OnReceptionShowDirectorsRoom()
	AddDelayRemImage( 450, 240, 450 + 128, 240 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowWhereCaseButton()
	AddDelayRemImage( 93, 470, 93 + 128, 470 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowNewEngineProjectButton()
	browser:Move( scrWidth / 2, 0 )
	AddDelayRemImage( scrWidth / 4, scrHeight / 2, scrWidth / 4 + 128, scrHeight / 2 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowNewGameProjectButton()
	browser:Move( 0, 0 )
	AddDelayRemImage( scrWidth * 0.75, scrHeight / 2, scrWidth * 0.75 + 128, scrHeight / 2 + 64, "media/buttons/mouse_left.png", 5000 )
end