local base = _G

module( "tutorialMedia" )

function ShowReklame()
	base.button.ArrowToElement( base.reklameManager.btnReklame, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end