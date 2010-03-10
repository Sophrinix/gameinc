local sceneManager = CLuaSceneManager( NrpGetSceneManager() )

function AddCamera()

	local camera = CLuaCamera( sceneManager:AddCameraSceneNode( sceneManager:GetRootSceneNode() ) )
	
	--local anim = sceneManager:AddNerpaCameraAnimator( -315, -1000, 100, 1000 )
	
	camera:AddAnimator( anim )
	camera:SetName( MAIN_CAMERA_NAME )
	camera:SetFarValue( 200000 )
	camera:SetPosition( 0, 5000, 4500 )
	camera:SetTarget( 4500,100,0 )
		
	sceneManager:SetActiveCamera( camera:Self() )

end