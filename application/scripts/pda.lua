local base = _G

module( "pda" )

local guienv = base.guienv
local button = base.button
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local appPda = nil

local lbMessage = nil
local mainWindow = nil
local animTextRunner = nil

local hw = 192
local hh = 252
local offsethh = hh / 3
local visible = false

function GetWindow()
	return mainWindow 
end

function ToggleVisible()
	guienv:RemoveAnimators( mainWindow:Self() )
	
	if visible then	
		guienv:AddMoveAnimator( mainWindow:Self(), scrWidth - hw, scrHeight - offsethh, 
							1, true, true, false )
	else
		guienv:AddMoveAnimator( mainWindow:Self(), scrWidth - hw, scrHeight - hh, 
							1, true, true, false )
	end
	
	visible = not visible
end

function Next()
	appPda:Next()
	base.Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "NextR" )
	local textr = appPda:GetTimeStr().."\n"..appPda:GetMessage()
	base.CLuaElement( animTextRunner ):SetText( textr )
end

function Prev()
	appPda:Prev()
	base.Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "PrevR" )
	local textr = appPda:GetTimeStr().."\n"..appPda:GetMessage()
	base.CLuaElement( animTextRunner ):SetText( textr )
end

function Show( textr )
	if mainWindow == nil then
		mainWindow = guienv:AddWindow( "media/textures/pda.png", 0, scrHeight - offsethh, 
							 		   hw, scrHeight - offsethh + hh, -1, 
							 		   guienv:GetRootGUIElement() )
							 		   
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetDrawBody( false )
		mainWindow:SetDraggable( false ) 
		
		button.Stretch( 70, 191, 70+53, 191+50, 
		 			    "pda_down", mainWindow:Self(), -1, "",
						"./pda.Hide()" )
						
		button.Stretch( 10, 190, 40,  240, "suda", mainWindow:Self(), -1, "", "./pda.Prev()" )
		button.Stretch( hw - 30, 190, hw,  240, "tuda", mainWindow:Self(), -1, "", "./pda.Next()" )
						
		lbMessage = guienv:AddLabel( " ", 30, 30, 162, 252 - 40, -1, mainWindow:Self() )
		lbMessage:SetOverrideFont( "font_7" )
								 
		animTextRunner = guienv:AddTextRunner( lbMessage:Self(), "" )
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./pda.ToggleVisible()" )
		
		appPda = base.applic:GetPda()
	else
		guienv:BringToFront( mainWindow:Self() )
	end

	if textr ~= nil then
		appPda:AddMessage( textr, "./pda.ToggleVisible()" )
		local textr = appPda:GetTimeStr() .. "\n" .. appPda:GetMessage()
		base.CLuaElement( animTextRunner ):SetText( textr )
	end
	
	guienv:RemoveAnimators( mainWindow:Self() )
	guienv:AddMoveAnimator( mainWindow:Self(), scrWidth - hw, scrHeight - hh, 
							1, true, true, false )
	visible = true
end

function Hide()
	guienv:RemoveAnimators( mainWindow:Self() )
	guienv:AddMoveAnimator( mainWindow:Self(), 0, scrHeight - offsethh, 
							1, true, true, false )
	visible = false
end