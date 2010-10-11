local base = _G

module( "pda" )

local guienv = base.guienv
local button = base.button
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local lbMessage = nil
local mainWindow = nil
local animTextRunner = nil

local hw = 192 / 2
local hh = 252 / 2

function Show( textr )
	if mainWindow == nil then
		mainWindow = guienv:AddWindow( "media/maps/pda.png", scrWidth / 2 - hw, scrHeight / 2 - hh, 
							 		   scrWidth / 2 + hw, scrHeight / 2 + hh, -1, 
							 		   guienv:GetRootGUIElement() )
							 		   
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetDrawBody( false )
		--mainWindow:SetDraggable( false ) 
		
		button.Stretch( 60, 0, 100, 40, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./pda.Hide()" )
						
		lbMessage = guienv:AddLabel( " ", 30, 30, 162, 252 - 40, -1, mainWindow:Self() )
		lbMessage:SetOverrideFont( "font_7" )
								 
		animTextRunner = guienv:AddTextRunner( lbMessage:Self(), "" )
	else
		guienv:BringToFront( mainWindow:Self() )
	end

	base.CLuaElement( animTextRunner ):SetText( textr )
end

function Hide()
	mainWindow:Remove()
	mainWindow = nil
	animTextRunner = nil
	lbMessage = nil
end
