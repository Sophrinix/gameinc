local base = _G

module( "tutorialPlant" )

function ShowBoxCreating()
	base.button.ArrowToElement( base.plant.btnBoxCreate, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowBoxProduce()
	base.button.ArrowToElement( base.plant.btnBoxProduce, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end