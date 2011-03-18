local base = _G

IncludeScript("gameboxManager")
IncludeScript("diskManager")

module( "plantrd" )

local guienv = base.guienv
local button = base.button
local window = base.window

local company = nil
local plantWindow = nil

btnBoxCreate = nil
btnBoxProduce = nil

local function _Hide()
	base.package.loaded[ "gameboxManager" ] = false	
	base.package.loaded[ "diskManager" ] = false
end

function Show()
	plantWindow = window.fsWindow( "media/maps/plant_normal.png", _Hide )
	
	base.tutorial.Update( base.tutorial.STEP_OVERVIEW_PLANT )
	
	--box manager
	btnBoxCreate = button.EqualeTexture( 94, 29, "boxManager", plantWindow, -1, "", base.gameboxManager.Show )
	--produce
	btnBoxProduce = button.EqualeTexture( 407, 1, "produce", plantWindow, -1, "", base.diskManager.Show )
end