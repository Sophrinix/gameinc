local base = _G

module( "tutorialBank" )

function ShowLoan()
	base.button.ArrowToElement( base.bank.btnLoan, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( "50%", 0 )
end

function ShowDebt()
	base.button.ArrowToElement( base.bank.btnDebt, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( "50%", 0 )
end