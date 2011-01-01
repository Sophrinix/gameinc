local base = _G

module( "director" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

local mainWindow = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "director.FadeExitAction()" )	
end

function Show( ptr )
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/director_cabinet.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( "95%", "95%", "100%", "100%", 
		 			    "button_down", 
		 			    mainWindow:Self(), -1, "",
						"./director.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DIRECTORS_ROOM )
	
	button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", "./projectSelect.Show()")
	--employers manager
	button.EqualeTexture( 805, 238, "employersManager", mainWindow, -1, "", "./userManager.Show()" )
	--project manager
	button.EqualeTexture( 612, 251, "projectManager", mainWindow, -1, "", "./monitor.Show()" )
	
	button.EqualeTexture( 750, 233, "button_safe", mainWindow, -1, "", "./safe.Show()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "director.FadeEnterAction()" )
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