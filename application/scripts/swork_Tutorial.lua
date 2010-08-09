startGameWithTutorial = false

function StartDescriptionGame()
	local officeNodeName = "officeNode" 
	browser:Show()
	browser:Navigate( GetHelpLinkForObject( officeNodeName ) )
	
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
	--camera:MoveTo( sceneManager:GetSceneNodeByName( officeNodeName ), 300 )
end

function StartTutorialStuffEmploye()
	browser:Show()
end