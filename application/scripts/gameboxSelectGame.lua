local base = _G 

module( "gameboxSelectGame" )

local gameboxManager = nil
local guienv = base.guienv
local applic = base.applic
local button = base.button
local window = base.window
local company = nil

local mainWindow = nil

local function _SetGame( mp )
	local btn = base.CLuaButton( mp )
	gameboxManager.currentGame = company:GetGame( btn.text )
	base.LogScript( "current game is " .. gameboxManager.currentGame.name )
	mainWindow:Remove()
	
	base.gameboxSelectBoxSize.Show()
end

function Show()
	gameboxManager = base.gameboxManager
	company = gameboxManager.company
	
	mainWindow = guienv:AddWindow( "", "10%", "10%", "90%", "90%", -1, gameboxManager.mainWindow )
	mainWindow.closeButton.visible = false
	mainWindow.draggable = false
	mainWindow.drawBody = false
	
	local flick = guienv:AddFlick( "5%", "5%", "95%", "95%", 3, -1, mainWindow )
	local games = {}
	for i=1, company.gameNumber do
		local game = company:GetGame( i-1 )
		
		if not game.haveBox then 				
			button.StretchOne( 0, 0, 100, 100, game:GetScreenshot( -1 ), flick, -1, game.name, _SetGame )
			base.table.insert( games, game )
		end
	end
	
	base.LogScript( "found "..#games.." games without box" )
	if #games == 0 then 
		local lb = guienv:AddLabel( "Нет готовых игр", "33%", "33%", "33%+", "33%+", -1, mainWindow )
		lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
		lb.font = "font_28"
		lb.color = base.WHITE_COLOR
	end
end

