local base = _G

module( "gameboxAddonManager" )

local gameboxManager = nil
local guienv = base.guienv
local applic = base.applic
local button = base.button
local window = base.window
local currentGame = nil

local _boxWindow = nil
local _addonsWindow = nil

--добавление нового элемента к коробке
local function _BoxWindowLmbClick()
	if  guienv.dragObject == nil then
		return 
	end

	local link = base.CLuaLinkBox( guienv.dragObject )
	
	currentAddon = base.CLuaTech( link.data )
		
	local applied = currentGame:AddBoxAddon( currentAddon )
	
	if not applied then
		guienv:MessageBox( "Слишком мало места, попробуй другой предмет", false, false, button.CloseParent, button.NoFunction )
	end
	
	guienv:SetDragObject( nil, "" )
	currentAddon = nil
	
	base.gameboxAddonManager.Update()	
end

--удаление дополнения к коробке
local function _RemoveAddon( mp )
	local link = base.CLuaLinkBox( mp )
	 
	local tech = base.CLuaTech( link.data )
	base.LogScript( "Want remove addon "..tech.name )
	currentGame:RemoveBoxAddon( link.data )
	link:Remove()	
	
	base.gameboxAddonManager.Update()
end

function CreateBoxWindow()
	if _boxWindow ~= nil then
		_boxWindow:Remove()
	end
	
	_boxWindow = guienv:AddWindow( "gameBox.png", 220, 130, 520, 520, -1, gameboxManager.mainWindow )	
	_boxWindow.drawBody = false
	_boxWindow.draggable = false
	_boxWindow.closeButton.visible = false
	_boxWindow.onLmbClick = _BoxWindowLmbClick
	
	flickAdded = guienv:AddFlick( "5%", "5%", "95%", "95%", 3, -1, _boxWindow )
		
	local width = 60	
	for i=1, currentGame.boxAddonsNumber do
		local tech = currentGame:GetBoxAddon( i-1 )

		local lbu = guienv:AddLinkBox( tech.name, 0, 0, width, width, -1, flickAdded )
		lbu.draggable = true
		lbu.data = tech.object
		lbu.texture = tech.texture
	    lbu.unsetFunction = _RemoveAddon
	end
end

function CreateAddonsWindow()
	if _addonsWindow ~= nil then 
	   _addonsWindow:Remove()
	end
	   		
	_addonsWindow = guienv:AddWindow( "gameAddons.png", 820, 135, 990, 540, -1, gameboxManager.mainWindow )
	_addonsWindow.drawBody = false								 										 
	_addonsWindow.draggable = false
	_addonsWindow.closeButton.visible = false
	flickAvaible = guienv:AddFlick( "5%", "5%", "95%", "95%", 2, -1, _addonsWindow )
	
	local width = 60	
	for i=1, applic.boxAddonNumber do
		local boxAddon = applic:GetGameBoxAddon( i-1 )
	
		local linkt = guienv:AddLinkBox( boxAddon.name, 0, 0, width, width, -1, flickAvaible )
				
		linkt.draggable = true
		linkt.data = boxAddon.object
		linkt.texture = boxAddon.texture
	end
end

local function _ApplyBox( sender )
	gameboxManager.saveWork = true
    currentGame.viewImage = currentGame:GetBoxImage( base.math.random() % currentGame.boxImageNumber )
    --textures[ currentBoxIndex ]
    currentGame = nil
    
	window.Hide( sender )    
end

function Update()
	CreateBoxWindow()
	CreateAddonsWindow()
end

function Show()
	gameboxManager = base.gameboxManager
	currentGame = gameboxManager.currentGame
	
	_boxWindow = nil
	_addonsWindow = nil
	
	Update()
	
	button.Stretch( _boxWindow.right, _boxWindow.bottom, "100+", "50+", "", 
					gameboxManager.mainWindow, -1, "Закончить", _ApplyBox )
end

function NextBoxImage()
	if currentBoxIndex == #textures then
		currentBoxIndex = 1
	else
		currentBoxIndex = currentBoxIndex + 1
	end
	
	base.LogScript( textures[ currentBoxIndex ] )
	
	wndBoxPreview.texture = textures[ currentBoxIndex ]
end

function PrevBoxImage()
	if currentBoxIndex == 1 then
		currentBoxIndex = #textures
	else
		currentBoxIndex = currentBoxIndex - 1
	end
	
	base.LogScript( textures[ currentBoxIndex ] )
	
	wndBoxPreview.texture = textures[ currentBoxIndex ]
end

function SelectBoxImage()
	wndAvaibleAddons.visible = false
	
	btnNextBoxImage.visible = true
	btnPrevBoxImage.visible = true
	base.CLuaElement( wndBoxPreview ):RemoveChilds()
	NextBoxImage()
	
	btnNext.action = _HideWithBox
end

local function localCreateBoxImage()
	textures = nil
	textures = {}
	for i=1, currentGame.boxImageNumber do
		base.table.insert( textures, currentGame:GetBoxImage( i-1 ) )
	end
	
	btnNextBoxImage = button.EqualeTexture( 540, 220, "button_next", wndGBM, -1, ">>", NextBoxImage )
	btnNextBoxImage.visible = false
	btnPrevBoxImage = button.EqualeTexture( 120, 220, "button_prev", wndGBM, -1, "<<", PrevBoxImage )
	btnPrevBoxImage.visible = false
	currentBoxIndex = 1
end
