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

function EqualeTexture( x, y, texturePrefix, parentr, idr, textr, actionr )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()
	
	local btn = guienv:AddButton( x, y, x + txsw, y + txsh, parentr, idr, textr )
	btn:SetImage( 0, 0, txsw, txsh, normalPath )
	btn:SetHoveredImage( 0, 0, txsw, txsh, selectPath )	
	btn:SetPressedImage( 0, 0, txsw, txsh, selectPath )	
	btn:SetAction( actionr )
	
	return btn
end

function Stretch( x1, y1, x2, y2, texturePrefix, parentr, idr, textr, actionr )
	local normalPath = GetNormalPath( texturePrefix )
	local selectPath = GetSelectPath( texturePrefix )
	local txs = driver:GetTexture( normalPath )
	local txsw, txsh = txs:GetSize()

	local btn = guienv:AddButton( x1, y1, x2, y2, parentr, idr, textr )
	btn:SetImage( 0, 0, txsw, txsh, normalPath )
	btn:SetHoveredImage( 0, 0, txsw, txsh, selectPath )	
	btn:SetPressedImage( 0, 0, txsw, txsh, selectPath )	
	btn:SetAction( actionr )
	
	return btn
end