local base = _G

module( "plant" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local company = nil
local plantWindow = nil

btnBoxCreate = nil
btnBoxProduce = nil

function FadeEnterAction()
	plantWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	plantWindow:Remove()
	plantWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "plant.FadeExitAction()" )	
end

function Show()
	if plantWindow then
		plantWindow.visible = true
	else
		plantWindow = window.fsWindow( "media/maps/plant_normal.png", Hide )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_PLANT )
	
	--box manager
	btnBoxCreate = button.EqualeTexture( 94, 29, "boxManager", plantWindow, -1, "", base.gameboxManager.Show )
	--produce
	btnBoxProduce = button.EqualeTexture( 407, 1, "produce", plantWindow, -1, "", base.diskManager.Show )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end