local base = _G

module( "tutorialCity" )

function ShowPlant()
	base.button.ArrowToElement( base.city.btnPlant, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( base.scrWidth / 2, 0 )
end

function ShowOffice() 
	base.button.ArrowToElement( base.city.btnOffice, "media/buttons/mouse_left.png", 5000 )
end

function ShowLab()
	base.button.ArrowToElement( base.city.btnLab, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowReklame()
	base.button.ArrowToElement( base.city.btnReklame, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowBank()
	base.button.ArrowToElement( base.city.btnBank, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( base.scrWidth / 2, 0 )
end

function ShowUniver()
	base.button.ArrowToElement( base.city.btnUniver, "media/buttons/mouse_left.png", 5000 )
	browser:Move( scrWidth / 2, 0 )
end

function ShowShop()
	base.button.ArrowToElement( base.city.btnShop, "media/buttons/mouse_left.png", 5000 )
	browser:Move( scrWidth / 2, 0 )
end



