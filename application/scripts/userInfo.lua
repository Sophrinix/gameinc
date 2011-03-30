local base = _G

module( "userInfo" )

local guienv = base.guienv

local function _AddProgressBar( windowe, pos, texture )
	local prg = guienv:AddProgressBar( windowe, 0, 0, 1, 1, -1 )
	prg.position = pos
	prg.image = texture
	prg.alphaChannel = true
	prg.fillImage = "media/progressbar/prg_star_fill.png"
	--local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, windowe )
	--label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
end

function Update( user, windowg )
	windowg.closeButton.visible = false
	windowg.draggable = false
	windowg:SetMaxSize( "45%", "32%" )
	
	local money = user:GetParam( "wantMoney" )
	local label = guienv:AddLabel( base.string.format( "%s ($%d)", user.name, money ), 
								   "5%", 5, "95%", "20+", -1, windowg )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
	
	local ww, hh = windowg:GetSize()
	
	local hhImage = base.math.min( ww, hh - 20 )
	local wwImage = hhImage / 1.4
	
	local image = guienv:AddImage( "10", "10", wwImage.."+", hhImage.."+", windowg, -1, "" )
	image.texture = user.texture
	image.scale = true
	image.useAlphaChannel = true
	
	hhImage = hhImage * 0.9
	local layout = guienv:AddLayout( wwImage + 10, 25, (hhImage * 1.5).."+", hhImage.."+", 2, -1, windowg )
	_AddProgressBar( layout, user.knowledges, "media/progressbar/prg_experience.png" )
	_AddProgressBar( layout, user.quality, "media/progressbar/prg_quality.png" )
	_AddProgressBar( layout, user:GetParam( "codeSpeed" ), "media/progressbar/prg_speed.png" )
	_AddProgressBar( layout, user:GetParam( "stability" ), "media/progressbar/prg_stamina.png" )
					
	return windowg			   
end