local base = _G

module( "windowTalk" )

local guienv = base.guienv
local driver = base.driver
local button = base.button
local _mainWindow = nil
local _portretWindow = nil
local _dialogWindow = nil

local answerFont = "font_12"
local marginLeft = 40

function Show( txsPath, text, x, y )
	_dialogWindow = nil
	 
	local txs = driver:CreateGrayscale( "city_map.png" )
	_mainWindow = guienv:AddWindow( txs.path, 0, 0, "0e", "0e", -1, guienv.root )
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
	
	local frame =  guienv:AddWindow( "", 0, 0, "0e", "0e", -1, _portretWindow )
	frame.closeButton.visible = false
	frame.draggable = false
	frame.style = "BoxFrame"
	
	return _mainWindow
end

function Hide()
    guienv:AddBlenderAnimator( _mainWindow, 255, 10, 700, false, false, true ) 
end

local function _CheckDialogWidth( tblAction )
	local ret = _mainWindow.width * 0.3
	local answerNumber = #tblAction / 2
	for i=1, answerNumber do
		local varAnswerText = tblAction[ i * 2 - 1 ]
		local fw, fh = guienv:GetTextSize( answerFont, varAnswerText )
		
		ret = base.math.max( ret, fw )
	end
	
	return ret + 60
end

function Dialog( text, tblAction )
	
	if _dialogWindow then
	   _dialogWindow:Remove()	
	end
	
	local fw, fh = guienv:GetTextSize( answerFont, text )
	
	local width = _CheckDialogWidth( tblAction )
	local height = base.math.ceil( fw / width + 3 ) * fh;
	
	local answerNumber = #tblAction / 2
	local answerHeight = ( answerNumber + 2 ) * fh
	height = height + answerHeight
	
	base.LogScript( "width="..width.." height="..height )
	
	_dialogWindow = guienv:AddWindow( "", _portretWindow.right - 50, _portretWindow.top + 20, 
									  width .. "+", height .. "+", -1, _mainWindow )
									  
	_dialogWindow.closeButton.visible = false
	_dialogWindow.draggable = false
	_dialogWindow.style = "DialogWindow"
	_dialogWindow.drawBody = true
	_dialogWindow.visible = true	
									  
	local lb = guienv:AddLabel( text, marginLeft, 10, "20e", answerHeight .. "e", -1, _dialogWindow )
	lb.font = answerFont
	
	for i=1, answerNumber do
		local varAnswerText = tblAction[ i * 2 - 1 ]
		local varAnswerFunction = tblAction[ i * 2 ]
		
		local link = guienv:AddLink( marginLeft, height - 10 - i * fh, "20e", fh.."+", 
									 _dialogWindow, -1, varAnswerText, varAnswerFunction )
		link.font = answerFont
		link.color = base.NrpARGB( 0xff, 0, 0, 0xff )
	end
end