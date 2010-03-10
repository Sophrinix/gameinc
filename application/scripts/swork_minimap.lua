local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local driver = CLuaDriver( NrpGetVideoDriver() )

function AddButtonCamManager( mp )

	local width = 0
	local height = 0
	width, height = mp:GetSize()
	
	local scrb = CLuaScrollBar( guienv:AddCircleScrollBar( width - 70, height - 70, width, height, mp:Self(), ID_SCALETRACKBAR )  )
	
	scrb:SetMin( 10 )
	scrb:SetMax( 500 )
	scrb:SetAction( "ZoomScrollBarChanged" )

	scrb:SetTexture( "media/s_fon.png" )
	scrb:SetSliderTexture( "media/s_fon_x.png" )

	--[[guienv:AddRectAnimator( scrb:Self(),
	 					    width - 50, height - 50, width, height,
							width / 2, height / 2, width, height,
							3 )
--]]		
	local btn = CLuaButton( guienv:AddButton( 0, 0, 50, 20, mp:Self(), -1, "Block") )
	btn:SetAction( "CameraBlockToggle" )
	
	btn:SetObject( guienv:AddButton( width - 50, 0, width , 20, mp:Self(), -1, "Rejim") ) 
	btn:SetAction( "CameraRejimToggle" )
end

function AddMiniMap()

	local mapWidth = 180
	local mapHeight = 180
	
	local scrWidth = 0
	local scrHeight = 0 
	
	scrWidth, scrHeight = driver:GetScreenSize()
		
	local miniMap = CLuaMiniMap( guienv:AddMiniMap( 0, scrHeight - mapHeight, mapWidth, scrHeight, 
								  	 				-1, 
													sceneManager:GetActiveCamera() ) )
	
	miniMap:SetTexture( "media/earth.png" )
	miniMap:SetName( "WindowMiniMap" )
	miniMap:SetOnRotateAction( "RotateCameraFromMiniMap" )
	
	AddButtonCamManager( miniMap )
end