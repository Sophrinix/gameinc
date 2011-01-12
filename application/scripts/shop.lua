local base = _G

module( "shop" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local browser = base.browser
local tutorial = base.tutorial
local applic = base.applic

local mainWindow = nil

function FadeEnterAction()
	mainWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "shop.FadeExitAction()" )	
end

function Show()  
    if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/windowShop_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./shop.Hide()" )
	end

	--игры в продаже
	button.EqualeTexture( 147, 333, "gameInSale", mainWindow:Self(), -1, "", "./saleManager.Show()")
	
	--топ-лист месяца
	button.EqualeTexture( 703, 67, "toplistmonth", mainWindow:Self(), -1, "", "./topGame.Show( topGame.TIME_MONTH )" )

	--топ-лист за все время
	button.EqualeTexture( 119, 94, "toplisttime", mainWindow:Self(), -1, "", "./topGame.Show( topGame.TIME_LIFE )")

	--игровые журналы
	button.EqualeTexture( 861, 268, "showMagazines", mainWindow:Self(), -1, "", "./journals.Show()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "shop.FadeEnterAction()" )
end

function ShowMonthTopList()

end

function ShowAllTimeTopList()

end