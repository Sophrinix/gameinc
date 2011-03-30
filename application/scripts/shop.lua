local base = _G

IncludeScript("topGame" )
IncludeScript("journals")
IncludeScript("saleManager")

module( "shop" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

function Hide()
    base.LogScript( "shop.Hide" )
	base.package.loaded[ "topGame" ] = false
	base.package.loaded[ "journals" ] = false
	base.package.loaded[ "saleManager" ] = false
end

function Show()  
	mainWindow = window.fsWindow( "windowShop.png", Hide )
	
	--игры в продаже
	button.EqualeTexture( 147, 333, "gameInSale", mainWindow, -1, "", base.saleManager.Show )
	--топ-лист месяца
	button.EqualeTexture( 703, 67, "toplistmonth", mainWindow, -1, "", function() base.topGame.Show( base.topGame.TIME_MONTH ) end )
	--топ-лист за все время
	button.EqualeTexture( 119, 94, "toplisttime", mainWindow, -1, "", function() base.topGame.Show( base.topGame.TIME_LIFE ) end )
	--игровые журналы
	button.EqualeTexture( 861, 268, "showMagazines", mainWindow, -1, "", base.journals.Show )
end