local advFuncButtons = { }
local adminFuncButtons = { }
local mainMenuButtons = { } 

local imgTop = 0
local imgHeight = 32
local imgWidth = 32

mainMenuWindow = nil

local function AddButton( window, id, x, y, action, pathToTexture )

		local btn = guienv:AddButton( x, y, x + imgWidth, y + imgHeight, window, id, "" )
		btn:SetAction( action )
		btn:SetImage( 0, 0, 70, 70, pathToTexture..".png" )
		btn:SetHoveredImage( 0, 0, 70, 70, pathToTexture.."_x.png" )
		btn:SetPressedImage( 0, 0, 70, 70, pathToTexture.."_y.png" )
		btn:SetVisible( false )

		return btn:Self()
end

local function SetVisibleToArray( ptr, visible )
	
	local elm = CLuaElement( ptr[ 0 ] )
	
	for i=0, #ptr do
		elm:SetObject( ptr[ i ] )
		elm:SetVisible( visible )	
	end
	
end

local function AddAdvancedFunctionButton( menuw )

	local x = 65
	
	local btnCnt = 0
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_CONSOLE_TOGGLE_VISIBLE, x, imgTop, 
										  "ToggleConsoleVisible", "media/top_menu/cmd" )
	
	x = x + imgWidth;	btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_SONAR_TOGGLE_ENABLED, x, imgTop, 
										  "ToggleSonarEnabled", "media/top_menu/ok" )
	
	x = x + imgWidth;	btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_GRAPHIC_TOGGLE_VISIBLE, x, imgTop, 
										  "sworkToggleGraphicWindowVisible", "media/top_menu/graf" )
	
	x = x + imgWidth;	btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_SELECT_OBJECT_IN_SCENE, x, imgTop, 
										  "sworkSelectModeObjectAction", "media/top_menu/move" )
	
	x = x + imgWidth;	btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_DRAWREGION_IN_SCENE, x, imgTop, 
										  "sworkDrawRegionInSceneAction", "media/top_menu/move" )
	
	x = x + imgWidth;	btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_TARGETCAMERA_TOGGLE, x, imgTop, 
										  "sworkTargetCameraToggleVisible", "media/top_menu/replay" )

	x = 360; btnCnt = btnCnt + 1
	advFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_BACK_FROM_ADVFUNC_FRAME, x, imgTop, 
										  "AdvancedFunctionFrameToggleVisible", "media/top_menu/back" )
	
end

local function AddAdminingFunctionButton( menuw )

	local x = 65
	
	local btnCnt = 0
	adminFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_REPORT_TOGGLE_VISIBLE, x, imgTop, 
										    "sworkWindowReportToggleVisible", "media/top_menu/report" )

	x = x + imgWidth; btnCnt = btnCnt + 1
	adminFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_ARCHIVE_TOGGLE_VISIBLE, x, imgTop, 
										    "sworkToggleArchiveWindowVisible", "media/top_menu/targets" )
	
	x = x + imgWidth; btnCnt = btnCnt + 1
	adminFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_HARDWARE_TOGGLE_VISIBLE, x, imgTop, 
										    "sworkToggleHardwareWindowVisible", "media/top_menu/hard" )

	x = x + imgWidth; btnCnt = btnCnt + 1
	adminFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_PLAY_REC, x, imgTop, 
										    "sworkTogglePlayerWindowVisible", "media/top_menu/replay" )

	x = 360; btnCnt = btnCnt + 1
	adminFuncButtons[ btnCnt ] = AddButton( menuw:Self(), ID_BACK_FROM_ADMINING_FRAME, x, imgTop, 
										    "AdminingFrameToggleVisible", "media/top_menu/back" )

end

function AddMenuWindow()

	local txs = driver:GetTexture( "./media/top_menu/top_nerpa.png")
	local txsWidth = 0
	local txsHeight = 0
	txsWidth, txsHeight = txs:GetSize()
						  
	mainMenuWindow = guienv:AddWindow(	"./media/top_menu/top_nerpa.png",
			 							scrWidth/2 - txsWidth/2, 0, scrWidth/2 + txsWidth/2, 50,
										-1,
										guienv:GetRootGUIElement() )
																						
	mainMenuWindow:SetDraggable( false )
	mainMenuWindow:SetDrawBody( false )
	guienv:AddHoveredAnimator( mainMenuWindow:Self(), 100, 255, 4, true, false, false )

	local btn = mainMenuWindow:GetCloseButton()
	btn:SetVisible( false )
	
	local x = 65
	local btnCnt = 0
	mainMenuButtons[ btnCnt ] = AddButton( mainMenuWindow:Self(), ID_TRAS_IND, x, imgTop, 
										    "sworkToggleIndicatorWindowVisible", "media/top_menu/trass" )
	
	x = x + imgWidth;  btnCnt = btnCnt + 1
	mainMenuButtons[ btnCnt ] = AddButton( mainMenuWindow:Self(), ID_START_STOP, x, imgTop, 
										    "sworkToggleDeviceListenerRejim", "media/top_menu/play" )

	x = x + imgWidth;  btnCnt = btnCnt + 1
	mainMenuButtons[ btnCnt ] = AddButton( mainMenuWindow:Self(), ID_ADMINING, x, imgTop, 
										    "AdminingFrameToggleVisible", "media/top_menu/settings" )

	x = x + imgWidth;  btnCnt = btnCnt + 1
	mainMenuButtons[ btnCnt ] = AddButton( mainMenuWindow:Self(), ID_FUNC, x, imgTop, 
										    "AdvancedFunctionFrameToggleVisible", "media/top_menu/star" )
	x = 335;  btnCnt = btnCnt + 1
	--создание метки с часами
	local timeLabel = guienv:AddLabel( "Время", x - 140, imgTop, x + 40, imgTop + 15, ID_DATETIME_LABEL, mainMenuWindow:Self() )
	timeLabel:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 );
	mainMenuButtons[ btnCnt ] = timeLabel:Self()
	
	btnCnt = btnCnt + 1
	local userLabel = guienv:AddLabel( "UserName", x - 140, imgTop + 17, x + 40, imgTop + 32, ID_USERNAME_LABEL, mainMenuWindow:Self() )
	userLabel:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 );
	mainMenuButtons[ btnCnt ] = userLabel:Self()
	
	x = x + imgWidth; btnCnt = btnCnt + 1
	mainMenuButtons[ btnCnt ] = AddButton( mainMenuWindow:Self(), ID_QUIT, x, imgTop, 
										   "sworkApplicationClose", "media/top_menu/off" )
										   
	--покажем кнопки главного меню	
	AddAdminingFunctionButton( mainMenuWindow )
	AddAdvancedFunctionButton( mainMenuWindow )
	SetVisibleToArray( mainMenuButtons, true )

end

function AdvancedFunctionFrameToggleVisible( ptr )

	local elm = CLuaElement( advFuncButtons[ 0 ] )
	local visible = not elm:GetVisible()
	SetVisibleToArray( mainMenuButtons, not visible )
	SetVisibleToArray( advFuncButtons, visible )
		
end

function AdminingFrameToggleVisible( ptr )
	local elm = CLuaElement( adminFuncButtons[ 0 ] )
	local visible = not elm:GetVisible()
	SetVisibleToArray( mainMenuButtons, not visible )
	SetVisibleToArray( adminFuncButtons, visible )
end