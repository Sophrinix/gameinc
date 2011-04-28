local base = _G

module( "tutorialReklame" )

function ShowTypes()
	base.button.ArrowToElement( base.reklameManager.picflowReklames, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowGames()
	base.button.ArrowToElement( base.reklameManager.lbxGames, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowInfoArea()
	base.button.ArrowToElement( base.reklameManager.btnDecDayNumber, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( "50%", 0 )
end

function ShowApplyWork()
	base.button.ArrowToElement( base.reklameManager.btnApplyWork, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( "50%", 0 )
end
