local base = _G

module( "magazine" )

local guienv = base.guienv
local header 
local text 
local mainWindow = nil

local lbHeader = nil
local edText = nil
local image = nil

local function _Hide()
	mainWindow.visible = false
end

function Show( header, text, font, texture )
	base.LogScript( "show for "..header  )
	if mainWindow == nil then
		mainWindow = guienv:AddWindow( "magazine_news.png", "25%", "25%", "50%+", "50%+", -1, guienv.root )
		mainWindow.drawBody = false
		
		lbHeader = guienv:AddLabel( header, "8%", "33%", "92%", "40%", -1, mainWindow )
		lbHeader:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
		edText = guienv:AddEdit( text, "50%", "43%", "92%", "92%", -1, mainWindow ) 
		edText.multiline = true
		edText.enabled = true
		
		image = guienv:AddImage( "8%", "43%", "48%", "92%", mainWindow, -1, "" )
		image.scale = true
		image.alphaChannel = true
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, _Hide )
	end
	
	lbHeader.text = header
	edText.text = text	
	
	if font then
		edText.font = font
	end
	
	if texture then
		image.texture = texture
	end

	
	mainWindow.visible = true
	
	guienv:BringToFront( mainWindow )
end