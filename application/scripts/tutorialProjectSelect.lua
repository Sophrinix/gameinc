local base = _G

module( "tutorialProjectSelect" )

function ShowEngine()
	browser:Move( scrWidth / 2, 0 )
	base.button.ArrowToElement( base.projectSelect.btnEngine, "media/buttons/mouse_left.png", 5000 )
end

function ShowGame()
	browser:Move( 0, 0 )
	base.button.ArrowToElement( base.projectSelect.btnGame, "media/buttons/mouse_left.png", 5000 )
end