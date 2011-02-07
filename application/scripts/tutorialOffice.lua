local base = _G

module( "tutorialOffice" )

function ShowDirector()
	base.button.ArrowToElement( base.office.btnDirector, "media/buttons/mouse_left.png", 5000 )
end

function ShowDevRoom()
	base.button.ArrowToElement( base.office.btnDevRoom, "media/buttons/mouse_left.png", 5000 )
end

function ShowResearch()
	base.button.ArrowToElement( base.office.btnResearch, "media/buttons/mouse_left.png", 5000 )
end