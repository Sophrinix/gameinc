local base = _G

module( "gameboxSelectBoxSize" )

local guienv = base.guienv
local button = base.button

local mainWindow = nil

local function _SetSize( size )
	mainWindow:Remove()
	base.gameboxManager.currentGame:CreateBox( size )
	base.gameboxAddonManager.Show()
end

function Show()
	mainWindow = guienv:AddWindow( "", "10%", "10%", "90%", "90%", -1, base.gameboxManager.mainWindow )
	mainWindow.closeButton.visible = false
	mainWindow.draggable = false
	mainWindow.drawBody = false

	local flick = guienv:AddFlick( "5%", "5%", "95%", "95%", 3, -1, mainWindow )

	button.Stretch( 0, 0, 50, 50, "smallBox", flick, -1, "", function() _SetSize(10) end )  
	button.Stretch( 0, 0, 75, 75, "middleBox", flick, -1, "", function() _SetSize(20) end )  
	button.Stretch( 0, 0, 120, 120, "bigBox", flick, -1, "", function() _SetSize(30) end )  
	button.Stretch( 0, 0, 120, 120, "megaBox", flick, -1, "", function() _SetSize(50) end )  
end