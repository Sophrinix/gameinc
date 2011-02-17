local base = _G

module( "tutorialDirector" )

function ShowEmployers()
	base.button.ArrowToElement( base.director.btnEmployer, "media/buttons/mouse_left.png", 5000 )
	AddDelayRemImage( 850, 300, 850 + 128, 300 + 64, "media/buttons/mouse_left.png", 5000 )
end

function ShowComp()
	base.button.ArrowToElement( base.director.btnComp, "media/buttons/mouse_left.png", 5000 )
end

function ShowProject()
	base.button.ArrowToElement( base.director.btnProject, "media/buttons/mouse_left.png", 5000 )
end

function ShowCase()
	base.button.ArrowToElement( base.director.btnCase, "media/buttons/mouse_left.png", 5000 )
end

