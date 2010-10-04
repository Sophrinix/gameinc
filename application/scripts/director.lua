local base = _G

module( "director" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

function Show( ptr )
	if directorCabinetWindow then
		directorCabinetWindow:SetVisible( true )
	else
		directorCabinetWindow = guienv:AddWindow( "media/director_cabinet.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		directorCabinetWindow:SetDraggable( false )
		directorCabinetWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", directorCabinetWindow:Self(), -1, "",
						"./directorCabinetWindow:Remove(); directorCabinetWindow = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DIRECTORS_ROOM )
	
	button.EqualeTexture( 0, 227, "newProject", directorCabinetWindow:Self(), -1, "", "sworkCreateWindowWizardProject")
	--employers manager
	button.EqualeTexture( 805, 238, "employersManager", directorCabinetWindow:Self(), -1, "", "./userManager.Show()" )
	--project manager
	button.EqualeTexture( 612, 251, "projectManager", directorCabinetWindow:Self(), -1, "", "./projectManager.Show()" )
end