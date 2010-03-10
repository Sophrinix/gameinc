pcall( require, "elmid" )
pcall( require, "logdefs")
pcall( require, "NerpaActionType")

--безопасная загрузка  скриптов

IncludeScript("swork_NodeWindowProperties")
IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_wndTraceIndicator") 
IncludeScript("swork_DrawRegionFunctions") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndReport")

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )

local ModeAddingObject=0
local SceneNodeBfPlace="_SceneNodeBeforePlace_"
local CustomObjectOnSceneCounter=0

function ResolveUserEvent( ptr )

	local sysEvent = CLuaEvent( ptr )
	
	local swEvent = sysEvent:GetUserData1()
	
	if swEvent == EVENT_ADD_TO_LIST then
		local tabler =  CLuaTable( guienv:GetElementByID( ID_EVENT_TABLE ) )
		
		local rowIndex = tabler:AddRow( tabler:GetRowCount() )
		
		tabler:SetCellText( rowIndex, 0, sysEvent:GetTime(), 0xff, 0xff, 0, 0 )
		tabler:SetCellText( rowIndex, 1, "warn", 0xff, 0xff, 0, 0 )
		tabler:SetCellText( rowIndex, 2, sysEvent:GetLogText(), 0xff, 0xff, 0, 0 )
	end
	
end

function ToggleModeAddingObjectInScene( ptr  )

	if ModeAddingObject == 0 then
		ModeAddingObject=1
		Log({src=SCRIPT, dev=ODS|CON}, "Start function object adding (ModeAddingObject="..ModeAddingObject..")")
		
		local node = CLuaSceneNode( sceneManager:AddCubeSceneNode( SceneNodeBfPlace ) )
		node:SetScale( 10, 10, 10 )
		node:SetMaterialTexture( 0, "media/t351sml.jpg" )
		node:SetMaterialFlag( EMF_LIGHTING, false )
				
		sceneManager:AddSceneFunction( SCENE_LMOUSE_LEFT_UP, "AddObjectInScene" )
		sceneManager:AddSceneFunction( SCENE_AFTER_END, "UpdateAddingObjectPosition" )
	else
		ModeAddingObject=0
		Log({src=SCRIPT, dev=ODS|CON}, "Stop function object adding (ModeAddingObject="..ModeAddingObject..")")

		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_LEFT_UP, "AddObjectInScene" )
		sceneManager:RemoveSceneFunction( SCENE_AFTER_END, "UpdateAddingObjectPosition" )
		
		local node = CLuaSceneNode( sceneManager:GetSceneNodeByName( SceneNodeBfPlace ) )
		if node:Self() ~= nil then 
			node:Remove()
		end 	
	end
	
end

function sworkSelectModeObjectAction( ptr )

	local object = sceneManager:GetSelectedNode()
	
	if object == nil then
		return
	end
	
	local node = CLuaSceneNode( object )
	local nodeTypeName = node:GetTypeName()
	
	Log({src=SCRIPT, dev=ODS|CON}, "sworkSelectModeObjectAction step 1")

	if nodeTypeName == "Terrain" then
		ToggleModeAddingObjectInScene( nil )
		return 
	end
	
	if nodeTypeName == "Sonar" or nodeTypeName == "Cube" then
		sworkCreateWindowPropertySonar( nil )
		return
	end	

end

function AddObjectInScene()

	CustomObjectOnSceneCounter = CustomObjectOnSceneCounter + 1
	local node = CLuaSceneNode( sceneManager:GetSceneNodeByName( SceneNodeBfPlace ) )
	node:SetName( "_CustomSceneObject"..CustomObjectOnSceneCounter )
	
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( node:Self() )
	node:SetTriangleSelector( selector )
	
	ToggleModeAddingObjectInScene( nil )

end

function UpdateAddingObjectPosition()

	local node = CLuaSceneNode( sceneManager:GetSceneNodeByName( SceneNodeBfPlace ) )
	local x=0
	local y=0
	local z=0
	
	x, y, z = sceneManager:GetCurrentWorldPosition()
	node:SetPosition( x, y, z )

end

function ToggleConsoleVisible( ptr )

	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()

end

function ZoomScrollBarChanged( ptr )

	if not ptr then
		return 0
	end
	
	local scrollBar = CLuaScrollBar( ptr )
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
		
	camera:SetZoom( scrollBar:GetPos() * 10 )
	
end

function RotateCameraFromMiniMap( ptr )

	if not ptr then
		return 0
	end
		
	local miniMap = CLuaMiniMap( ptr )
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
	x, y, z = camera:GetRotate()
	
	camera:SetRotate( x + miniMap:GetAngleOffset(), y, z )
end

function CameraScaleTrackBarVisibleToggle( ptr )
	
	local b = guienv:GetElementByID( ID_SCALETRACKBAR )
	
	if b then
		local scrb = CLuaScrollBar( b )
	
		scrb:SetVisible( not scrb:GetVisible() )
	end

end

function ToggleVisibleFormularAndEventTable( ptr )

	local targetsTable = CLuaElement( guienv:GetElementByID( ID_FORMULAR ) )
	local eventsTable = CLuaElement( guienv:GetElementByID( ID_EVENT_TABLE ) )
	
	local visible = targetsTable:GetVisible()
	
	targetsTable:SetVisible( not visible )
	eventsTable:SetVisible( visible )

end