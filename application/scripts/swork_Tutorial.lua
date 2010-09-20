startGameWithTutorial = false
STEP_OVERVIEW_RECEPTION = 1
STEP_CREATE_NEW_PROJECT = 2
STEP_OVERVIEW_DIRECTORS_ROOM = 3
STEP_OVERVIEW_UNIVER = 4
STEP_OVERVIEW_LABORATORY = 5
STEP_OVERVIEW_PLANT	= 6

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
	elseif currentStep == STEP_OVERVIEW_LABORATORY then
		browser:Navigate( "media/html/TutorialLaboratorySceneEng.htm" )
	elseif currentStep == STEP_OVERVIEW_PLANT then
		browser:Navigate( "media/html/TutorialPlantSceneEng.htm" )
	end
end

function tutorial_ShowPlantTableButton()
	AddDelayRemImage( 150, 0, 150 + 128, 0 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( scrWidth / 2, 0 )
end

function tutorial_ShowPlantProduceButton()
	AddDelayRemImage( 850, 0, 850 + 128, 0 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, 0 )
end

function tutorial_ShowLaboratoryVideoTechButton()
	AddDelayRemImage( 545, 330, 545 + 128, 330 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, 0 )
end

function tutorial_ShowLaboratorySoundTechButton()
	AddDelayRemImage( 372, 213, 372 + 128, 213 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, scrHeight / 2 )
end

function tutorial_ShowLaboratoryGenreTechButton()
	AddDelayRemImage( 749, 222, 749 + 128, 222 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, 0 )
end

function tutorial_ShowLaboratoryAdvancedTechButton()
	AddDelayRemImage( 73, 202, 73 + 128, 202 + 64, "media/buttons/mouse_left.png", 5000 )
	browser:Move( scrWidth / 2, 0 )
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

function tutorial_CityMapShowLaboratory()
	AddDelayRemImage( 811, 198, 811 + 128, 198 + 64, "media/buttons/mouse_left.png", 5000 )	
	browser:Move( 0, 0 )
end

function tutorial_CityMapShowPlant()
	AddDelayRemImage( 517, 326, 517 + 128, 326 + 64, "media/buttons/mouse_left.png", 5000 )	
	browser:Move( 0, 0 )
end

function tutorial_CityMapShowBank()
	AddDelayRemImage( 323, 389, 323 + 128, 389 + 64, "media/buttons/mouse_left.png", 5000 )	
	browser:Move( scrWidth / 2, 0 )
end

function tutorial_CityMapShowUniversitet()
	AddDelayRemImage(  0, 287, 0 + 128, 287 + 64, "media/buttons/mouse_left.png", 5000 )	
	browser:Move( scrWidth / 2, 0 )
end

function tutorial_ShowNewEngineProjectButton()
	browser:Move( scrWidth / 2, 0 )
	AddDelayRemImage( scrWidth / 4, scrHeight / 2, scrWidth / 4 + 128, scrHeight / 2 + 64, "media/buttons/mouse_left.png", 5000 )
end

function tutorial_ShowNewGameProjectButton()
	browser:Move( 0, 0 )
	AddDelayRemImage( scrWidth * 0.75, scrHeight / 2, scrWidth * 0.75 + 128, scrHeight / 2 + 64, "media/buttons/mouse_left.png", 5000 )
end