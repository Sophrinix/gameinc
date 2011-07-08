local base = _G

IncludeScript( "testerComp" )
IncludeScript( "projectManager" )
IncludeScript( "projectChief" )

module( "devRoom" )

local guienv = base.guienv
local button = base.button
local tutorial = base.tutorial
local window = base.window

btnTester = nil

local mainWindow = nil

local function _Hide()
	base.package.loaded[ "testerComp" ] = false
	base.package.loaded[ "projectManager" ] = false
	base.package.loaded[ "projectChief" ] = false
end

function ShowHelp()
	tutorial.Update( "devRoom/main" )
end

function Show( ptr )
	mainWindow = window.fsWindow( "developers.png", _Hide )
	
	base.rightPanel.AddYesNo( "’отите больше узнать об управлении разработкой?", ShowHelp, button.CloseBlend )
	
	btnTester = button.EqualeTexture( 820, 384, "devComp", mainWindow, -1, "", base.testerComp.Show )
	btnProgress = button.Stretch( "9%", "22%", "28%", "62%", "btnProjectProgress", mainWindow, -1, "", base.projectManager.Show ) 
	btnManagers = button.Stretch( "67.7%", "44%", "82.5%", "69%", "btnProjectChiefs", mainWindow, -1, "", base.projectChief.Show )
	--employers manager
end

