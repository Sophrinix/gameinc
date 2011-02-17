local base = _G

IncludeScript("director")

module( "office" )

local guienv = base.guienv
local receptionWindow = nil

local button = base.button
local tutorial = base.tutorial
local window = base.window

btnDirector = nil
btnDevRoom = nil
btnResearch = nil

function FadeEnterAction()
	receptionWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	receptionWindow:Remove()
	receptionWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false)			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
end

function OpenLabRoom()
	base.inventionManager.Show( "", base.applic.playerCompany.name )
end

function Show()
	if receptionWindow then
		receptionWindow.visible = true
	else
		receptionWindow = window.fsWindow( "media/maps/reception.png", Hide )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_RECEPTION )
	--directors room
	btnDirector = button.EqualeTexture( 448, 242, "director", receptionWindow, -1, "Dbg:Director", base.director.Show )
	btnDevRoom = button.EqualeTexture( 355, 249, "developers", receptionWindow, -1, "Dbg:DevRoom", base.devRoom.Show )
	btnResearch = button.EqualeTexture( 0, 157, "button_laboratory", receptionWindow, -1, "Dbg:Research", OpenLabRoom )
	--
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end