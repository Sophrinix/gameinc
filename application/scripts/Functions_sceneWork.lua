pcall( require, "elmid" )
pcall( require, "logdefs")
pcall( require, "NerpaActionType")

--безопасная загрузка  скриптов

IncludeScript("swork_NodeWindowProperties")
IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndReport")

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )

function ToggleConsoleVisible( ptr )

	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()

end

function sworkSelectObjectOnScene( ptr )

	local node = CLuaSceneNode( ptr )
	
	if node:GetName() == "bankNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( bankSceneObjects, true )
		return 0
	end
	
	if node:GetName() == "exitBankNode" then
		SetVisibleObjects( bankSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		return 0
	end

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