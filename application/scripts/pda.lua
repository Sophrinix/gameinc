local base = _G

module( "pda" )

local guienv = base.guienv
local button = base.button
local appPda = nil

local lbMessage = nil
local animTextRunner = nil

local hw = 192
local hh = 252
local hhEnd = "252e"
local offsethh = hh / 3
local offsethhEnd = offsethh.."e"
local visible = false

mainWindow = nil

local function _ToggleVisible()
	guienv:RemoveAnimators( mainWindow )

	base.LogScript( "pda _ToggleVisible offsethhEnd=".. offsethhEnd.. "  hhEnd="..hhEnd ) 
	
	if visible then	
		guienv:AddMoveAnimator( mainWindow, 0, offsethhEnd, 1, true, true, false )
	else
		guienv:AddMoveAnimator( mainWindow, 0, hhEnd, 1, true, true, false )
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
		base.LogScript( "pda show ofsethh=".. offsethh.. "  hh="..hh ) 
		mainWindow = guienv:AddWindow( "media/textures/pda.png", 0, offsethhEnd, 
							 		   hw, (offsethh + hh).."e", -1, 
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
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, _ToggleVisible )
		
		appPda = base.applic.pda
		
		guienv:AddTopElement( mainWindow )
	else
		guienv:BringToFront( mainWindow )
	end

	if textr ~= nil then
		appPda:AddMessage( textr, _ToggleVisible )
		local textr = appPda.time .. "\n" .. appPda.message
		base.CLuaElement( animTextRunner ).text = textr
	end
	
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, hhEnd, 1, true, true, false )
	visible = true
end

function Hide()
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, offsethhEnd, 1, true, true, false )
	visible = false
end