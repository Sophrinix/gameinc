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
		guienv:AddMoveAnimator( mainWindow, 0, offsethhEnd, 2, true, true, false )
	else
		guienv:AddMoveAnimator( mainWindow, 0, hhEnd, 2, true, true, false )
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

local function _CheckTime()
	local year, _, _ = base.applic:GetGameTime()
	
	if year < 1988 then
		mainWindow.texture = "pdaPaper.png"
	elseif year >= 1988 and year < 1998 then
		mainWindow.texture = "pda.png"
	elseif year >= 1998 and year < 2011 then
		mainWindow.texture = "futurePda.png"
	else
		mainWindow.texture = "noPdaTexture.png"
	end
end

function Show( textr )
	if mainWindow == nil then
		base.LogScript( "pda show ofsethh=".. offsethh.. "  hh="..hh ) 
		mainWindow = guienv:AddWindow( "", 0, hh.."e", hw, "0e", -1, guienv.root )
							 		   
		mainWindow.closeButton.visible = false
		mainWindow.drawBody = false
		mainWindow.draggable = false
		
		button.Stretch( 70, 191, "53+", "50+", "pda_down", mainWindow, -1, "", Hide )
		button.Stretch( 10, 190, 40, 240, "suda", mainWindow, -1, "", Prev )
		button.Stretch( "30e", 190, "0e",  240, "tuda", mainWindow, -1, "", Next )
						
		lbMessage = guienv:AddLabel( " ", 30, 30, "30e", "40e", -1, mainWindow )
		lbMessage.font = "font_7"
								 
		animTextRunner = guienv:AddTextRunner( lbMessage, "" )
		
		mainWindow.onLmbClick = _ToggleVisible
		
		appPda = base.applic.pda
		
		guienv:AddTopElement( mainWindow )
	end
	
	_CheckTime()

	if textr ~= nil then
		appPda:AddMessage( textr, _ToggleVisible )
		local textr = appPda.time .. "\n" .. appPda.message
		base.CLuaElement( animTextRunner ).text = textr
	end
	
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, hhEnd, 2, true, true, false )
	visible = true
	
	base.soundenv:Play( "Reminder.wav" )
end

function Hide()
	guienv:RemoveAnimators( mainWindow )
	guienv:AddMoveAnimator( mainWindow, 0, offsethhEnd, 2, true, true, false )
	visible = false
end