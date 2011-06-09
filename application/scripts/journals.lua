local base = _G

IncludeScript( "gameRecense" )

module( "journals" )

local button = base.button
local window = base.window
local guienv = base.guienv
local browser = base.browser
local applic = base.applic

local mainWindow = nil
lbxGames = nil
prgGraphik = nil
prgSound = nil
prgBugs = nil
prgGenres = nil
imgRating = nil
btnShowRecense = nil
currentGame = nil

local function _ShowHistoryReview()
	currentGame = base.CLuaGame( lbxGames.selectedObject )
	browser:Show()
	browser:Navigate( currentGame.description )
	browser:Move( "50%", 0 )
	
	prgGraphik.position = currentGame.graphikRating
	prgSound.position = currentGame.soundRating
	prgGenres.position = currentGame.gameplayRating
	prgBugs.position = currentGame.bugsRating
end

local function _ShowAvaibleGames()
	local game = nil
	for i=1, applic.gamesNumber do
		game = applic:GetGame( i-1 )
		if game.inSale then
			lbxGames:AddItem( game.name, game.object )
		end
	end	
end

local function _Hide()
	base.package.loaded[ "gameRecense" ] = false
	
	mainWindow = nil
end

local function _AddProgressBar( l, t, r, b )
	local ret = guienv:AddProgressBar( mainWindow, l, t, r, b, -1 )
	ret.image = "media/progressbar/prgNoStars.png"
	ret.fillImage = "media/progressbar/prgStars.png"
	ret.alphaChannel = true
	return ret 
end

function ShowHelp()
	base.tutorial.Update( "shop/journals" )
end

function Show()
	local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, _Hide )
	
	local img = guienv:AddImage( 0, 0, "0e", "0e", mainWindow, -1, "" );
	img.texture = "magazine_busines.png"
	img.alphaChannel = true
	guienv:SendToBack( img )
			
	lbxGames = guienv:AddComponentListBox( 320, 50, 730, 520, -1, mainWindow )	
	lbxGames.onChangeSelect = _ShowHistoryReview					
	
	prgGraphik = _AddProgressBar( "57%", "84.2%", "67%", "85.7%" )
	prgSound = _AddProgressBar( "57%", "86.9%", "67%", "88.4%" )
	prgGenres = _AddProgressBar( "57%", "89.3%", "67%", "90.8%" )
	prgBugs = _AddProgressBar( "57%", "91.8%", "67%", "93.4%" )
	btnShowRecense = button.StretchOne( "67.3%", "81.5%", "73%", "90%", "", mainWindow, -1, "", base.gameRecense.Show )
	
	_ShowAvaibleGames()
	
	base.rightPanel.AddYesNo( "Хотите больше узнать о журналах?", ShowHelp, button.CloseParent )
end