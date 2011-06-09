local base = _G

IncludeScript( "gameboxSelectGame" )
IncludeScript( "gameboxSelectBoxSize" )
IncludeScript( "gameboxAddonManager" )

module( "gameboxManager" )

local guienv = base.guienv
local applic = base.applic
local button = base.button
local window = base.window

local currentAddon = nil
local wndBoxPreview = nil
local wndAvaibleAddons = nil

local windowImageViewer = nil
local dragLink = nil
local flickAvaible = nil
local flickAdded = nil
local textures = {}
local btnNextBoxImage = nil
local btnPrevBoxImage = nil
local currentBoxIndex = 1

mainWindow = nil
currentGame = nil
company = nil
saveWork = false

local function _HideWithoutBox()
	guienv:SetDragObject( nil, "" )
	
	if not saveWork and currentGame then
		currentGame:RemoveBox()
		currentGame = nil
	end
	
	mainWindow = nil
	
	base.package.loaded[ "gameboxSelectGame" ] = false
	base.package.loaded[ "gameboxSelectBoxSize" ] = false
	base.package.loaded[ "gameboxAddonManager" ] = false
end

local function _CancelAddon( ptr )
	guienv:SetDragObject( nil, "" )
	currentAddon = nil
end

function Show()
	saveWork = false
	company = applic.playerCompany
	currentGame = nil

	local txsBlur = base.driver:CreateBlur( "plant.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, _HideWithoutBox )	
	mainWindow.visible = false
	mainWindow.onLmbClick = _CancelAddon
	
	base.gameboxSelectGame.Show()
end
