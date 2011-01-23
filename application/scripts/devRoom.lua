local base = _G

module( "devRoom" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local mainWindow = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "devRoom.FadeExitAction()" )	
end

function Show( ptr )
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/developers.png", 0, 0, 
									   scrWidth, scrHeight, -1, 
									   guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( "80e", "80e", "10e", "10e", 
		 			    "button_down", 
		 			    mainWindow, -1, "",
						"./devRoom.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DEVELOPERS_ROOM )
	
	button.EqualeTexture( 820, 384, "devComp", mainWindow, -1, "", "./testerComp.Show()")
	--employers manager
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "devRoom.FadeEnterAction()" )
end

function FadeEnterAction()
	mainWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

