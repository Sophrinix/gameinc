local base = _G

module("button")

local guienv = base.guienv
local driver = base.driver 
local buttonsDirectory = "media/buttons/"

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

function EqualeTexture( x, y, texturePrefix, parentr, idr, textr, actionr )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()
	
	local btn = guienv:AddButton( x, y, x + txsw, y + txsh, parentr, idr, textr )
	SetImagePack( btn, 0, 0, txsw, txsh, normalPath, selectPath )
	btn:SetAction( actionr )
	
	return btn
end

function SetEqualeImage( button, texturePrefix )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()
	
	SetImagePack( button, 0, 0, txsw, txsh, normalPath, selectPath )
end

function Stretch( x1, y1, x2, y2, texturePrefix, parentr, idr, textr, actionr )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()

	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	SetImagePack( btn, 0, 0, txsw, txsh, normalPath, selectPath )
	btn:SetAction( actionr )
	
	return btn
end

function StretchOne( x1, y1, x2, y2, pathToTexture, parentr, idr, textr, actionr )
	local txs = driver:GetTexture( pathToTexture )
	local txsw, txsh = txs:GetSize()
	
	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	btn:SetImage( x, y, txsw, txsh, pathToTexture )
	btn:SetAction( actionr )
	
	return btn
end