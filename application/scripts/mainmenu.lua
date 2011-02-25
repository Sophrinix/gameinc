local base = _G

module( "mainmenu" )

local guienv = base.guienv
local driver = base.driver
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local funcLayout = nil
local adminLayout = nil
local mainLayout = nil 
local timeLabel = nil
local balanceLabel = nil

mainWindow = nil

function ShowFuncsButtons( vis )
	mainLayout.visible = not vis
	adminLayout.visible = false
	funcLayout.visible = vis
end

function ShowAdminButtons( vis )
	mainLayout.visible = not vis
	adminLayout.visible = vis
	funcLayout.visible = false
end

local function AddButton( window, action, pathToTexture )
		local btn = guienv:AddButton( 0, 0, 1, 1, window, -1, "" )
		btn.action = action
		btn:SetImage( 0, 0, 70, 70, pathToTexture.."_normal.png" )
		btn:SetHoveredImage( 0, 0, 70, 70, pathToTexture.."_select.png" )
		btn:SetPressedImage( 0, 0, 70, 70, pathToTexture.."_pressed.png" )

		return btn
end

function ToggleConsole()
	local con = base.CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	con:ToggleVisible()
end

function PureFunction()

end

local function AddAdvancedFunctionButton()
	funcLayout = guienv:AddLayout( "15%", "5%", "80%", "95%", 10, -1, mainWindow ) 	

	AddButton( funcLayout, ToggleConsole, "media/top_menu/cmd" )
	AddButton( funcLayout, base.userManager.Show, "media/top_menu/ue" )
	AddButton( funcLayout, PureFunction, "media/top_menu/graf" )
	AddButton( funcLayout, PureFunction, "media/top_menu/move" )
	AddButton( funcLayout, PureFunction, "media/top_menu/move" )
	AddButton( funcLayout, function () ShowFuncsButtons( false ) end , "media/top_menu/back" )	
end

local function AddAdminingFunctionButton()
	adminLayout = guienv:AddLayout( "15%", "5%", "80%", "95%", 10, -1, mainWindow ) 	

	AddButton( adminLayout, PureFunction, "media/top_menu/report" )
	AddButton( adminLayout, PureFunction, "media/top_menu/targets" )
	AddButton( adminLayout, PureFunction, "media/top_menu/hard" )
	AddButton( adminLayout, PureFunction, "media/top_menu/replay" )
	AddButton( adminLayout, function () ShowAdminButtons( false ) end , "media/top_menu/back" )
end

local function _UpdateTimeLabel()
	base.applic:UpdateGameTime( timeLabel )	
	balanceLabel.text = "$"..base.applic.playerCompany.balance
end


function Show()
	local txs = driver:GetTexture( "media/top_menu/top_nerpa.png")
	local txsWidth = 0
	local txsHeight = 0
	txsWidth, txsHeight = txs:GetSize()
						  
	mainWindow = guienv:AddWindow(	"media/top_menu/top_nerpa.png",
			 						scrWidth/2 - txsWidth/2, 0, scrWidth/2 + txsWidth/2, 50,
									-1, guienv.root )
																						
	mainWindow.draggable = false
	mainWindow.drawBody = false
	mainWindow.closeButton.visible = false
	
	guienv:AddHoveredAnimator( mainWindow, 100, 255, 4, true, false, false )

	mainLayout = guienv:AddLayout( "15%", "5%", "70%", "60%", 10, -1, mainWindow ) 
	
	AddButton( mainLayout, PureFunction, "media/top_menu/trass" )
	AddButton( mainLayout, PureFunction, "media/top_menu/play" )
	AddButton( mainLayout, function () ShowAdminButtons( true ) end, "media/top_menu/settings" )
	AddButton( mainLayout, function () ShowFuncsButtons( true ) end, "media/top_menu/one" )	
	AddButton( mainLayout, base.sworkApplicationClose, "media/top_menu/off" )
	
	timeLabel = guienv:AddLabel( "Время", "180e", "5%", "130+", "15+", -1, mainLayout )
	timeLabel.color = base.toColor( 0xFF, 0xC0, 0xC0, 0xC0 )
	balanceLabel = guienv:AddLabel( "UserName", "180e", "50%", "130", "15+", -1, mainLayout )
	balanceLabel.color = base.toColor( 0xFF, 0xC0, 0xC0, 0xC0 )
	mainWindow:AddLuaFunction( base.GUIELEMENT_AFTER_DRAW, _UpdateTimeLabel )
	
	AddAdminingFunctionButton()
	AddAdvancedFunctionButton()
										   
	--покажем кнопки главного меню	
	mainLayout.visible = true
	adminLayout.visible = false
	funcLayout.visible = false
	
	guienv:AddTopElement( mainWindow )
end