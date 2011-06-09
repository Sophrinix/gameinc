local base = _G

IncludeScript( "mail" )

module( "mainmenu" )

local guienv = base.guienv
local driver = base.driver
local button = base.button

local _height = 50

local funcLayout = nil
local adminLayout = nil
local mainLayout = nil 
local timeLabel = nil
local balanceLabel = nil

mainWindow = nil

function ShowAdminButtons( vis )
	mainLayout.visible = not vis
	adminLayout.visible = vis
	funcLayout.visible = false
end

function ToggleConsole()
	local con = base.CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	con:ToggleVisible()
end

local function _AddButton( prefix, lt, action )
	if lt == nil then
		base.LogScript( "layout == nil for button "..prefix )
		return
	end
	
	local ret = button.LayoutButton( prefix, lt, -1, "", action )
	ret:SetMaxSize( _height, _height )
end

local function AddAdminingFunctionButton()
	adminLayout = guienv:AddLayout( "300", 0, "300e", "0e", 10, -1, mainWindow ) 	

	_AddButton( "tm_back", adminLayout, function () ShowAdminButtons( false ) end )
end

function UpdateTime()
	local y, mh, d, hr, min = base.applic:GetGameTime()
	timeLabel.text = base.string.format( "%04d/%02d/%02d  %02d:%02d", y, mh, d, hr, min )
	balanceLabel.text = "$"..base.applic.playerCompany.balance
end

local function _ShowHelp()

end

function Show()					  
	mainWindow = guienv:AddWindow(	"", 0, 0, "0e", _height, -1, guienv.root )																					
	mainWindow.draggable = false
	mainWindow.drawBody = false
	mainWindow.closeButton.visible = false
	
	mainLayout = guienv:AddLayout( 300, 0, "300e", "0e", 10, -1, mainWindow ) 
	
	_AddButton( "vopros", mainLayout, _ShowHelp )
	_AddButton( "tm_settings", mainLayout, function () ShowAdminButtons( true ) end )
	_AddButton( "email", mainLayout, base.mail.Show )
	_AddButton( "tm_off", mainLayout, base.sworkApplicationClose )

	timeWindow = guienv:AddWindow( "timeBar.png", 0, 0, "296+", "0e", -1, mainWindow )			
	timeWindow.draggable = false
	timeWindow.drawBody = false
	timeWindow.closeButton.visible = false
	timeLabel = guienv:AddLabel( "Время", 70, 0, "200+", "0e", -1, timeWindow )
	timeLabel:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	timeLabel.color = base.NrpARGB( 0xFF, 0xFF, 0xFF, 0xFF )
	
	moneyWindow = guienv:AddWindow(	"moneyBar.png", "300e", 0, "296+", "0e", -1, mainWindow )	
	moneyWindow.draggable = false
	moneyWindow.drawBody = false
	moneyWindow.closeButton.visible = false		
	balanceLabel = guienv:AddLabel( "UserName", 20, 0, "70e", "0e", -1, moneyWindow )
	balanceLabel:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	balanceLabel.color = base.NrpARGB( 0xFF, 0xFF, 0xFF, 0xFF )
	
	AddAdminingFunctionButton()
										   
	--покажем кнопки главного меню	
	mainLayout.visible = true
	adminLayout.visible = false
	
	guienv:AddTopElement( mainWindow )
end