local base = _G

module( "tutorialDevRoom" )

function ShowTesters()
	base.button.ArrowToElement( base.devRoom.btnTester, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end