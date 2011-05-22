local base = _G

IncludeScript("gameboxManager")
IncludeScript("diskManager")

module( "plantrd" )

local guienv = base.guienv
local button = base.button
local window = base.window

local company = nil
local plantWindow = nil

btnBoxCreate = nil
btnBoxProduce = nil

local function _Hide()
	base.package.loaded[ "gameboxManager" ] = false	
	base.package.loaded[ "diskManager" ] = false
end

local function _IsHaveGameToProduce()
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		
		if game.haveBox and game.inSale then 
			return true
		end
	end
	
	return false
end

local function _ShowDiskManager()
	
	if _IsHaveGameToProduce() then
		base.diskManager.Show()
	else
		guienv:MessageBox( "Нет игр для производства", false, false, button.CloseParent, button.NoFunction )
	end
end

function ShowHelp()
	base.tutorial.Update( "plant/main" )
end

function Show()
	company = base.applic.playerCompany
	plantWindow = window.fsWindow( "plant.png", _Hide )
	
	base.rightPanel.AddYesNo( "Хотите больше узнать о лаборатории?", ShowHelp, button.CloseParent )

	--box manager
	btnBoxCreate = button.EqualeTexture( 94, 29, "boxManager", plantWindow, -1, "", base.gameboxManager.Show )
	--produce
	btnBoxProduce = button.EqualeTexture( 407, 1, "produce", plantWindow, -1, "", _ShowDiskManager )
end