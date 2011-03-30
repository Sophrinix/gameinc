local base = _G

module( "journals" )

local button = base.button
local window = base.window
local guienv = base.guienv
local browser = base.browser
local currentGame = nil
local applic = base.applic

local MainWindow = nil
local lbxGames = nil
local picflowGames = nil

local function _ShowHistoryReview()
	currentGame = base.CLuaGame( lbxGames.selectedObject )
	browser:Show()
	browser:Navigate( currentGame.description )
end

local function ShowAvaibleGames()
	local game = nil
	for i=1, applic.gamesNumber do
		game = applic:GetGame( i-1 )
		if game.inSale then
			lbxGames:AddItem( game.name, game.object )
		end
	end	
end

local function _Hide()
end

function Show()
	if mainWindow then
		mainWindow.visible = true
	else
		local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
		mainWindow = window.fsWindow( txsBlur.path, _Hide )
		
		local img = guienv:AddImage( 0, 0, "0e", "0e", mainWindow, -1, "" );
		img.texture = "media/textures/magazine.png"
		img.alphaChannel = true
		guienv:SendToBack( img )
				
		lbxGames = guienv:AddComponentListBox( 320, 50, 730, 520, -1, mainWindow )						
		--picflowGames = guienv:AddPictureFlow( 60, 10, scrWidth - 10, scrHeight / 3, -1, campaniesWindow:Self() )
		--picflowGames:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
		--picflowGames:SetDrawBorder( false )		
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, _ShowHistoryReview )				
	end
	
	ShowAvaibleGames()
end