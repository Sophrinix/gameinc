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
		 			    "poweroff", mainWindow:Self(), -1, "",
						"./monitor.Hide()" )
	else
		mainWindow:SetVisible( true )
		return
	end
	
	local offsetX = 150
	local width = 90
	button.Stretch( offsetX, width, offsetX + width, width+width, "m_inDevelop", mainWindow:Self(), -1, 
					base.STR_DEVPROJECTS, "./projectManager.Show()" )
					
	offsetX = offsetX + 20 + width					
	button.Stretch( offsetX, width, offsetX + width, width+width, "m_buhgalter", mainWindow:Self(), -1, 
					base.STR_BUHGALTERIA, "./buhgalter.Show()" )

	offsetX = offsetX + 20 + width					
	button.Stretch( offsetX, width, offsetX + width, width+width, "m_personal", mainWindow:Self(), -1, 
					base.STR_PERSONAL, "./userManager.Show()" )
end