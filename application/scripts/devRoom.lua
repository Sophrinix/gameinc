local base = _G

IncludeScript( "testerComp" )

module( "devRoom" )

local guienv = base.guienv
local button = base.button
local tutorial = base.tutorial
local window = base.window

local mainWindow = nil

local function _Hide()
	base.package.loaded[ "testerComp" ] = nil
end

function Show( ptr )
	mainWindow = window.fsWindow( "media/maps/developers.png", _Hide )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DEVELOPERS_ROOM )
	
	button.EqualeTexture( 820, 384, "devComp", mainWindow, -1, "", base.testerComp.Show )
	--employers manager
end

