local base = _G

module( "rightPanel" )

local guienv = base.guienv
local button = base.button

local _defaultWidth = 250
local _defaultHeight = 100
local _closeAreaOffset = 30
local _mainWindow = nil

local windows = {}

local function _Resort( sender )
	local wndCount = 0
	for i=1, _mainWindow.childCount do
		local elm = base.CLuaElement( _mainWindow:GetChild( i-1 ) )
		
		if elm.object ~= sender and elm.typeName == "CNrpWindow" then		
			elm:SetPosition( 0, wndCount * ( _defaultHeight + 5 ) )
			wndCount = wndCount + 1
		end
	end

	base.LogScript( "window number = "..wndCount )	
	_mainWindow.visible = wndCount > 0
end

function AddYesNo( text, funcYes, funcNo )
	local wnd = guienv:AddWindow( "media/textures/rightPanelWindow.png", 0, 0, "0e", _defaultHeight.."+", -1, _mainWindow )
	local lb = guienv:AddLabel( text, "5%", "5%", "80%", "60%", -1, wnd )
	lb.font = "font_8"
	
	wnd.drawBody = false
	wnd.closeButton:SetPosition( 284, 18 )
	wnd.onRemove = _Resort
	
	local btn = button.Stretch( "5%", "35e", "43%", "0e", "button_rp", wnd, -1, "Да", function() funcYes(); wnd:Remove() end )
	btn = button.Stretch( "43%", "35e", "81%", "0e", "button_rp", wnd, -1, "Нет", funcNo )
	
	guienv:AddTimer( 5000, function() guienv:AddToDeletionQueue( wnd ) end, wnd.object )

	_Resort()
end

function AddOk( text, funcOk )
	local wnd = guienv:AddWindow( "media/textures/rightPanelWindow.png", 0, 0, "311+", "154+", -1, _mainWindow )
	local lb = guienv:AddLabel( text, "5%", "5%", "80%", "60%", -1, wnd )
	wnd.drawBody = false
	wnd.closeButton:SetPosition( "24e", 18 )
	wnd.onRemove = _Resort
	
	_Resort()
end

function Show()
	if _mainWindow == nil then
		_mainWindow = guienv:AddWindow( "", _defaultWidth .. "e", "10%", "0e", "90%", -1, guienv.root )
		_mainWindow.drawBody = false
		_mainWindow.closeButton.visible = false
		guienv:AddTopElement( _mainWindow )
	
		--AddYesNo( "тестовое сообщение", nil, nil )
	end
end