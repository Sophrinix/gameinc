local base = _G

IncludeScript("director")
IncludeScript("devRoom")
IncludeScript("inventionManager")

module( "office" )

local guienv = base.guienv
local receptionWindow = nil

local button = base.button
local tutorial = base.tutorial
local window = base.window

btnDirector = nil
btnDevRoom = nil
btnResearch = nil

function OpenLabRoom()
	base.inventionManager.Show( "", base.applic.playerCompany.name )
end

function Show()
	receptionWindow = window.fsWindow( "media/maps/reception.png", nil )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_RECEPTION )
	--directors room
	btnDirector = button.EqualeTexture( 448, 242, "director", receptionWindow, -1, "Dbg:Director", base.director.Show )
	btnDevRoom = button.EqualeTexture( 355, 249, "developers", receptionWindow, -1, "Dbg:DevRoom", base.devRoom.Show )
	btnResearch = button.EqualeTexture( 0, 157, "button_laboratory", receptionWindow, -1, "Dbg:Research", OpenLabRoom )
end