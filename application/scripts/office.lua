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

function ShowHelp()
	tutorial.Update( "office/reception" )
end

local function _Hide()
	base.package.loaded[ "director" ] = false
	base.package.loaded[ "devRoom" ] = false
	base.package.loaded[ "inventionManager" ] = false
end

function Show()
	receptionWindow = window.fsWindow( "media/maps/reception.png", _Hide )
	
	base.rightPanel.AddYesNo( "Хотите больше узнать о вашем офисе?", ShowHelp, button.CloseParent )

	--directors room
	btnDirector = button.EqualeTexture( 448, 242, "director", receptionWindow, -1, "Dbg:Director", base.director.Show )
	btnDevRoom = button.EqualeTexture( 355, 249, "developers", receptionWindow, -1, "Dbg:DevRoom", base.devRoom.Show )
	btnResearch = button.EqualeTexture( 0, 157, "button_laboratory", receptionWindow, -1, "Dbg:Research", OpenLabRoom )
end