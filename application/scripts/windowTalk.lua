local base = _G

module( "windowTalk" )

local guienv = base.guienv
local driver = base.driver
local _mainWindow = nil
local _portretWindow = nil
local _dialogWindow = nil

function Show( txsPath, text, x, y )
	_dialogWindow = nil
	 
	_mainWindow = guienv:AddWindow( "", 0, 0, "0e", "0e", -1, guienv.root )
	_mainWindow.closeButton.visible = false
	_mainWindow.draggable = false
	_mainWindow.drawBody = false
	_mainWindow.visible = true
	
	local texture = driver:GetTexture( txsPath )
	_portretWindow = guienv:AddWindow( txsPath, x, y, texture.width.."+", texture.height.."+", -1, _mainWindow )
	_portretWindow.closeButton.visible = false
	_portretWindow.draggable = false
	_portretWindow.drawBody = false
	_portretWindow.visible = true
	
	return _mainWindow
end

function Hide()
	_mainWindow:Remove()
end

function Dialog( text, tblAction )
	
	if _dialogWindow then
	   _dialogWindow:Remove()	
	end
	
	local fw, fh = guienv:GetTextSize( "font_12", text )

	local width = _mainWindow.width * 0.3
	fh = fh * 1.5
	local height = base.math.ceil( fw / width ) * fh;
	
	local answerNumber = #tblAction / 2
	local answerHeight = answerNumber * fh
	height = height + answerHeight
	
	base.LogScript( "width="..width.." height="..height )
	
	_dialogWindow = guienv:AddWindow( "dialogSmall.png", _portretWindow.right - 50, _portretWindow.top + 20, 
									  width .. "+", height .. "+", -1, _mainWindow )
									  
	_dialogWindow.closeButton.visible = false
	_dialogWindow.draggable = false
	_dialogWindow.drawBody = false
	_dialogWindow.visible = true									  

	local lb = guienv:AddLabel( text, 10, 10, "10e", answerHeight .. "e", -1, _dialogWindow )
	lb.font = "font_12"
	
	for i=1, answerNumber do
		local varAnswerText = tblAction[ i * 2 - 1 ]
		local varAnswerFunction = tblAction[ i * 2 ]
		
		local link = guienv:AddLink( 30, _dialogWindow.height - i * fh, "30e", "30+", 
									 _dialogWindow, -1, varAnswerText, varAnswerFunction )
		link.font = "font_12"
		link.color = base.NrpARGB( 0xff, 0, 0, 0 )
	end
end