local base = _G 

module( "tutorialShop" )

function ShowChartAllTime()
	local elm = base.button.ArrowToElement( base.shop.btnTopAllTime, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowChartMonth()
	local elm = base.button.ArrowToElement( base.shop.btnTopMonth, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowGameInSale()
	local elm = base.button.ArrowToElement( base.shop.btnInSale, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowJournals()
	local elm = base.button.ArrowToElement( base.shop.btnJournals, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end
