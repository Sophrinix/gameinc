local base = _G

IncludeScript("topGame" )
IncludeScript("journals")

module( "shop" )

local guienv = base.guienv

local window = base.window
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

function FadeEnterAction()
	mainWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
	
	base.package.loaded[ "topGame" ] = nil
	base.package.loaded[ "journals" ] = nil
	
end

function Show()  
    if mainWindow then
		mainWindow.visible = true
		return
	else
		mainWindow = window.fsWindow( "media/maps/windowShop_normal.png", Hide )
	end

	--игры в продаже
	button.EqualeTexture( 147, 333, "gameInSale", mainWindow, -1, "", base.saleManager.Show )
	
	--топ-лист месяца
	button.EqualeTexture( 703, 67, "toplistmonth", mainWindow, -1, "", function () base.topGame.Show( topGame.TIME_MONTH ) end )

	--топ-лист за все время
	button.EqualeTexture( 119, 94, "toplisttime", mainWindow, -1, "",function () base.topGame.Show( topGame.TIME_LIFE ) end )

	--игровые журналы
	button.EqualeTexture( 861, 268, "showMagazines", mainWindow, -1, "", base.journals.Show )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end