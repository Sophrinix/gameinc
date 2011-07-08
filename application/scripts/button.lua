local base = _G

module("button")

local guienv = base.guienv
local driver = base.driver 
local buttonsDirectory = "media/buttons/"

local function GetAbsPath( name )
	return buttonsDirectory..name..".png"
end

local function GetNormalPath( name )
	return buttonsDirectory..name.."_normal.png"
end

local function GetSelectPath( name )
	return buttonsDirectory..name.."_select.png"
end

local function SetImagePack( button, x, y, w, h, norm, sel )
	button:SetImage( x, y, w, h, norm )
	button:SetHoveredImage( x, y, w, h, sel )	
	button:SetPressedImage( x, y, w, h, sel )	
end

function CloseParent( mp )
	mp = base.CLuaButton( mp ).parent
	base.CLuaElement( mp ):Remove()
end

function CloseBlend( mp )
	mp = base.CLuaButton( mp ).parent
	guienv:AddBlenderAnimator( mp, 255, 10, 700, false, false, true ) 
end

function NoFunction()

end

function EqualeTexture( x, y, texturePrefix, parentr, idr, textr, actionr )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()
	
	if txsw == 0 then txsw = 60 end
	if txsh == 0 then txsh = 60 end
	
	local btn = guienv:AddButton( x, y, x + txsw, y + txsh, parentr, idr, textr )
	SetImagePack( btn, 0, 0, txsw, txsh, normalPath, selectPath )
	btn.action = actionr
	
	return btn
end

function SetEqualeImage( button, texturePrefix )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	
	SetImagePack( button, 0, 0, txs.width, txs.height, normalPath, selectPath )
end

local function _StandartSound()
	base.soundenv:Play( "mouseover.wav" )
end

function Stretch( x1, y1, x2, y2, texturePrefix, parentr, idr, textr, actionr, tooltip, sndOn, sndLv )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()

	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	SetImagePack( btn, 0, 0, txsw, txsh, normalPath, selectPath )
	btn.action = actionr
	
	if tooltip ~= nil then btn.tooltip = tooltip end
	if sndOn == nil then btn.onHovered = _StandartSound end
	if sndLv == nil then btn.onHoveredLeft = _StandartSound end
	
	return btn
end

function StretchGrayed( x1, y1, x2, y2, texturePrefix, parentr, idr, textr, actionr, tooltip, sndOn, sndLv )
	local selectPath = GetAbsPath( texturePrefix )
	local grTxs = driver:CreateGrayscale( selectPath )
	local txs = driver:GetTexture( selectPath )
	local txsw, txsh = txs:GetSize()

	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	SetImagePack( btn, 0, 0, txsw, txsh, grTxs.path, selectPath )
	btn.action = actionr
	
	if tooltip ~= nil then btn.tooltip = tooltip end
	if sndOn == nil then btn.onHovered = _StandartSound end
	if sndLv == nil then btn.onHoveredLeft = _StandartSound end
	
	return btn
end

function LayoutButton( texturePrefix, parentr, idr, textr, actionr )
	return Stretch( 0, 0, 1, 1, texturePrefix, parentr, idr, textr, actionr )
end

function LayoutPushButton( texturePrefix, parentr, idr, textr, actionr )
	local btn = Stretch( 0, 0, 1, 1, texturePrefix, parentr, idr, textr, actionr )
	btn.pushButton = true
	
	return btn
end

function StretchOne( x1, y1, x2, y2, pathToTexture, parentr, idr, textr, actionr )
	local txs = driver:GetTexture( pathToTexture )
	local txsw, txsh = txs:GetSize()
	
	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	btn:SetImage( x, y, txsw, txsh, pathToTexture )
	btn.action = actionr
	
	return btn
end

function ArrowToElement( elm, textureName, w, h, time )
	local x, y = elm:GetCenter()
	local image = guienv:AddImage( x - w / 2, y - h, w.."+", h.."+", guienv.root, -1, "" );
	image.texture = textureName
	image.scale = true
	image.alphaChannel = true
	guienv:AddDestructor( image, time )
	
	return image
end