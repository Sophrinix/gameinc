require("logdefs")
require("elmid")
require("NerpaActionType")

local WND_WIDTH = 400
local WND_HEIGHT = 400
local STEP = 5
local TEXTURE_NAME = "_RendeTargetTextureCurrentTarget_"
local OBJECT_ANCHORED_CAMERA = "_AnchoredToObjectCamera_"
local IMAGE_ANCHORED_CAMERA = "_ImageAnchoredCamera_"

function sworkTargetCameraToggleVisible( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_ADVANCED_CAMERA_NAME ) )
	
	if windowg:Empty() == 1 then	
		sworkWindowAdvancedCameraCreate( nil )
	else
		sworkWindowAdvancedCameraClose( nil )
	end

end

function sworkWindowAdvancedCameraCreate( ptr )

	local windowg = guienv:AddWindow( "", -WND_WIDTH, 100, 
										0, 100 + WND_HEIGHT, 
										-1, guienv:GetRootGUIElement() )
	
	Log({src=SCRIPT, dev=ODS}, "Окна доп.камеры нет, создаем заново" )
	
	windowg:SetDraggable( true )
	windowg:SetName( WINDOW_ADVANCED_CAMERA_NAME )

	local button = windowg:GetCloseButton()
	button:SetVisible( false )
	
	button:SetObject( guienv:AddButton( WND_WIDTH - 15, 0, WND_WIDTH, 15, windowg:Self(), -1, "X") )
	button:SetAction( "sworkWindowAdvancedCameraClose" )

	local img = CLuaImage( guienv:AddImage( 5, 20, WND_WIDTH - 5, WND_HEIGHT - 5, windowg:Self(), -1, "" ) )
	img:SetScaleImage( true )
	img:SetUseAlphaChannel( true )
	local texture = CLuaTexture( driver:AddRenderTargetTexture( 512, 512, TEXTURE_NAME )  )
	img:SetImage( texture:Self() )
	texture:Drop()
	
	local fixedCam =  CLuaCamera( sceneManager:AddCameraSceneNode( sceneManager:GetRootSceneNode() ) )
	
	fixedCam:SetPosition( 500,800,500 )
	fixedCam:SetTarget( -10,10,-100 )
	fixedCam:SetName( OBJECT_ANCHORED_CAMERA )
	
	sceneManager:SetActiveCamera( sceneManager:GetSceneNodeByName( MAIN_CAMERA_NAME ) )
		
	--создадим аниматор, который переместит окно в новые координаты, оставит его видимым, а потом удалится сам
	guienv:AddMoveAnimator( windowg:Self(), 0, 150, STEP, true, true, false )
	sceneManager:AddSceneFunction( SCENE_BEFORE_RENDER, "sworkWindowAdvancedCameraUpdate" )

end

function sworkWindowAdvancedCameraClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_ADVANCED_CAMERA_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Нет окна доп.камеры чтобы его закрывать" )
	else	
		guienv:RemoveAnimators( windowg:Self() ) 
		guienv:AddMoveAnimator( windowg:Self(), -WND_WIDTH, 150, STEP, false, true, true )
		sceneManager:AddToDeletionQueue( sceneManager:GetSceneNodeByName( OBJECT_ANCHORED_CAMERA ) )
	end	
	
	sceneManager:RemoveSceneFunction( SCENE_BEFORE_RENDER, "sworkWindowAdvancedCameraUpdate" )
end

function sworkWindowAdvancedCameraUpdate( ptr )
	
	local fixedCam = CLuaCamera( sceneManager:GetSceneNodeByName( OBJECT_ANCHORED_CAMERA ) )
	if fixedCam:Empty() == 0 then

		driver:SetRenderTarget( driver:GetTexture( TEXTURE_NAME ), true, true, 0, 0, 0, 0xff );

		sceneManager:SetActiveCamera( fixedCam:Self() )
		sceneManager:RenderScene()

		driver:SetRenderTarget( nil, true, true, 0, 0, 0, 0 );

		sceneManager:SetActiveCamera( sceneManager:GetSceneNodeByName( MAIN_CAMERA_NAME ) )
	end
	
end