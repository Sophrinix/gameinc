local base = _G

module( "window" )

function fsWindow( txs, action )
	local wnd = base.guienv:AddWindow( txs, 0, 0, "0e", "0e", -1, base.guienv.root )
	wnd.closeButton.visible = false
	wnd.draggable = false
	wnd.drawBody = false
	--adding closeButton
	base.button.Stretch( "50e", "50e", "0e", "0e", "button_down", wnd, -1, "",	action )
	
	return wnd
end