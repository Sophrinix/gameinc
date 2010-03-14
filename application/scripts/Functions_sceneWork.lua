pcall( require, "elmid" )
pcall( require, "logdefs")
pcall( require, "NerpaActionType")

--безопасная загрузка  скриптов

IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndReport")
IncludeScript("swork_wndLoanAction")

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local app = CLuaApplication( NrpGetApplication() )

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
app:AddFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
app:AddFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
app:AddFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )

function sworkAppDayChange()

end

function sworkAppMonthChange()

end

function sworkAppYearChange()

end

function sworkMainLoop()
	app:UpdateGameTime( ID_DATE_LABEL, ID_TIME_LABEL )	
end

function ToggleConsoleVisible( ptr )

	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()

end

function sworkSelectObjectOnCityScene( ptr )

	local node = CLuaSceneNode( ptr )
	
	if node:GetName() == "bankNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( bankSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
		return 0
	end
	
end 

function sworkSelectObjectOnBankScene( ptr )

	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "loanNode" then
		sworkCreateWindowLoanAction()
		return 0
	end

	if nodeName == "exitBankNode" then
		SetVisibleObjects( bankSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-BANK:Не могу найти узел для работы"..nodeName )
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