local base = _G

module( "tutorialLab" )

function ShowVideo()
	base.button.ArrowToElement( base.laboratory.btnVideo, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( 0, 0 )
end

function ShowSound()
	base.button.ArrowToElement( base.laboratory.btnSound, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, base.scrHeight / 2 )
end

function ShowGenre()
	base.button.ArrowToElement( base.laboratory.btnGenre, "media/buttons/mouse_left.png", 5000 )
	browser:Move( 0, 0 )
end

function ShowTech()
	base.button.ArrowToElement( base.laboratory.btnTech, "media/buttons/mouse_left.png", 5000 )
	browser:Move( base.scrWidth / 2, 0 )
end


