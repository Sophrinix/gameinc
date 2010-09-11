startGameWithTutorial = false
STEP_OVERVIEW_OFFICE = 1
STEP_OVERVIEW_RECEPTION = 2
STEP_CREATE_NEW_PROJECT = 3

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
		browser:Navigate( "media/html/TutorialStartNewProjectEng.htm" )
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

end

function tutorial_ShowWhereManageEmployersButton()

end

function tutorial_OnReceptionShowDirectorsRoom()
	AddDelayRemImage( 450, 240, 450 + 128, 240 + 64, "media/buttons/mouse_left.png", 5000 )
end