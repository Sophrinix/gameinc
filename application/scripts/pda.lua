local base = _G

module( "pda" )

local guienv = base.guienv
local button = base.button
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local appPda = nil

local lbMessage = nil
local animTextRunner = nil

local hw = 192
local hh = 252
local offsethh = hh / 3
local visible = false

mainWindow = nil

function ToggleVisible()
	guienv:RemoveAnimators( mainWindow )
	
	if visible then	
		guienv:AddMoveAnimator( mainWindow, 0, scrHeight - offsethh, 1, true, true, false )
	else
		guienv:AddMoveAnimator( mainWindow, 0, scrHeight - hh, 1, true, true, false )
	end
	
	visible = not visible
end

function Next()
	appPda:Next()
	base.LogScript( "NextR" )
	local textr = appPda.time.."\n"..appPda.message
	base.CLuaElement( animTextRunner ).text = textr
end

function Prev()
	appPda:Prev()
	base.LogScript( "PrevR" )
	local textr = appPda.time .. "\n" .. appPda.message
	base.CLuaElement( animTextRunner ).text = textr
end

function Show( textr )
	if mainWindow == nil then
		mainWindow = guienv:AddWindow( "media/textures/pda.png", 0, scrHeight - offsethh, 
							 		   hw, scrHeight - offsethh + hh, -1, 
							 		   guienv.root )
							 		   
		mainWindow.closeButton.visible = false
		mainWindow.drawBody = false
		mainWindow.draggable = false
		
		button.Stretch( 70, 191, "53+", "50+", "pda_down", mainWindow, -1, "", Hide )
		button.Stretch( 10, 190, 40, 240, "suda", mainWindow, -1, "", Prev )
		button.Stretch( "30e", 190, "0e",  240, "tuda", mainWindow, -1, "", Next )
						
		lbMessage = guienv:AddLabel( " ", 30, 30, "30e", "40e", -1, mainWindow )
		lbMessage.font = "font_7"
								 
		animTextRunner = guienv:AddTextRunner( lbMessage, "" )
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, ToggleVisible )
		
		appPda = base.applic.pda
		
		guienv:AddTopElement( mainWindow )
	else
		guienv:BringToFront( mainWindow )
	end

	if textr ~= nil then
		appPda:AddMessage( textr, "./pda.ToggleVisible()" )
		local textr = appPda:GetTimeStr() .. "\n" .. appPda:GetMessage()
		base.CLuaElement( animTextRunner ).text = textr
	end
	
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, scrHeight - hh, 1, true, true, false )
	visible = true
end

function Hide()
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, scrHeight - offsethh, 1, true, true, false )
	visible = false
end