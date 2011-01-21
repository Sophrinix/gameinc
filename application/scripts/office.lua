local base = _G

IncludeScript("director")

module( "office" )

local guienv = base.guienv
local receptionWindow = nil

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

function FadeEnterAction()
	receptionWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	receptionWindow:Remove()
	receptionWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "office.FadeExitAction()" )	
end

function OpenLabRoom()
	base.inventionManager.Show( "", base.applic.playerCompany:GetName() )
end

function Show()
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/maps/reception.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		receptionWindow:GetCloseButton():SetVisible( false )
		receptionWindow:SetDraggable( false )
		receptionWindow:SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", receptionWindow:Self(), -1, "",
						"./office.Hide()" )
		
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_RECEPTION )
	--directors room
	button.EqualeTexture( 448, 242, "director", receptionWindow, -1, "director", "./director.Show()" )
	button.EqualeTexture( 355, 249, "developers", receptionWindow, -1, "devRoom", "./devRoom.Show()" )
	button.EqualeTexture( 0, 157, "button_laboratory", receptionWindow, -1, "devRoom", "./office.OpenLabRoom()" )
	--
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "office.FadeEnterAction()" )
end