local base = _G

module( "tutorialDevRoom" )

function ShowTester()
	local elm = base.button.ArrowToElement( base.devRoom.btnTester, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end