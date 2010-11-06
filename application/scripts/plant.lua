local base = _G

module( "plant" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local company = nil
local plantWindow = nil

function FadeEnterAction()
	plantWindow:SetVisible( true )
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
		plantWindow:SetVisible( true )
	else
		plantWindow = guienv:AddWindow( "media/maps/plant_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		plantWindow:GetCloseButton():SetVisible( false )
		plantWindow:SetDraggable( false )
		plantWindow:SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", plantWindow:Self(), -1, "",
						"./plant.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_PLANT )
	
	--box manager
	button.EqualeTexture( 94, 29, "boxManager", plantWindow:Self(), -1, "", "./gameboxManager.Show()" )
	--produce
	button.EqualeTexture( 407, 1, "produce", plantWindow:Self(), -1, "", "./diskManager.Show()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "plant.FadeEnterAction()" )
end