function ApplicationAddCityCamera()

	local camera = CLuaCamera( sceneManager:AddCameraSceneNode( sceneManager:GetRootSceneNode() ) )
	
	local anim = sceneManager:AddNerpaCameraAnimator( -315, -1000, 100, 1000 )
	
	camera:AddAnimator( anim )
	camera:SetName( MAIN_CAMERA_NAME )
	camera:SetFarValue( 200000 )
	camera:SetPosition( -400, 800, -800 )
	camera:SetTarget( 0,0,0 )
		
	sceneManager:SetActiveCamera( camera:Self() )

end