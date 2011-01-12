local base = _G

module( "monitor" )

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local LogScript = base.LogScript
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local mainWindow = nil
local width = 80

function Hide()
	mainWindow:Remove()
	mainWindow = nil
end

function Show()
	company = applic:GetPlayerCompany()
	if mainWindow == nil then
		mainWindow = guienv:AddWindow( "media/textures/monitor.png", 0, 0, base.scrWidth, base.scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetDraggable( false )
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 60, scrWidth - 20, scrHeight, 
		 			    "poweroff", mainWindow, -1, "",
						"./monitor.Hide()" )
	else
		mainWindow:SetVisible( true )
		return
	end
	
	local flick = guienv:AddFlick( 150, 90, scrWidth - 150, scrHeight - 90, 4, -1, mainWindow )
	
	button.Stretch( 0, 0, width, width, "m_inDevelop", flick, -1, base.STR_DEVPROJECTS, "./projectManager.Show()" )
	button.Stretch( 0, 0, width, width, "m_buhgalter", flick, -1, base.STR_BUHGALTERIA, "./buhgalter.Show()" )
	button.Stretch( 0, 0, width, width, "m_personal", flick, -1, base.STR_PERSONAL, "./userManager.Show()" )
end