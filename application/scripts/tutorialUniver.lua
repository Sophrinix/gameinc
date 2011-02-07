local base = _G

module( "tutorialUniver" )

function ShowDesk()
	base.button.ArrowToElement( base.univer.btnDesk, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end