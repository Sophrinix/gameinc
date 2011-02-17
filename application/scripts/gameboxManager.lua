local base = _G

module( "gameboxManager" )

local guienv = base.guienv
local applic = base.applic
local button = base.button
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local wndGBM = nil
local company = nil
local currentGame = nil
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

local function CreateElementsForGameSelect()
	local row = 0
	local columnt = 0
	local gameWithoutBox = 0
	local offset = 40
	local delimiter = 4
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		
		if not game:HaveBox() then 
			row = gameWithoutBox/delimiter
			columnt = gameWithoutBox%delimiter
			
			button.StretchOne( offset + scrWidth/delimiter * columnt, 
					  		   offset + scrHeight/delimiter * row, 
					  		   offset + scrWidth/delimiter * (columnt+1), 
					  		   offset + scrHeight/delimiter * (row+1),
							   game:GetScreenshot( -1 ), 
							   wndGBM, 
							   -1, "", "./gameboxManager.SetGame()" )
		  										
			gameWithoutBox = gameWithoutBox + 1
		end
	end
	
	if gameWithoutBox == 0 then 
		local image = guienv:AddImage( scrWidth / 2 - 300, scrHeight / 2 - 300, 
									   scrWidth / 2 + 300, scrHeight / 2 + 300, 
									   wndGBM, -1, "" )
		image:SetImage( "media/textures/noGameForCreateBox.png" )
		image:SetScaleImage( true )
		image:SetUseAlphaChannel( true )
	end
end

function FadeEnterAction()
	CreateElementsForGameSelect()

	wndGBM:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
	guienv:AddTimer( base.AFADE_TIME, "guienv:FadeAction( 0, true, true )" )	
end

function FadeExitAction()
	wndGBM:Remove()
	wndGBM = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "gameboxManager.FadeExitAction()" )	
end

function HideWithoutBox()
	currentGame:RemoveBox()
	currentGame = nil
	Hide()
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
	wndAvaibleAddons:SetVisible( false )
	
	btnNextBoxImage:SetVisible( true )
	btnPrevBoxImage:SetVisible( true )
	base.CLuaElement( wndBoxPreview ):RemoveChilds()
	NextBoxImage()
	
	btnNext:SetAction( "./gameboxManager.HideWithBox()" )
end

function HideWithBox()
    currentGame:SetViewImage( textures[ currentBoxIndex ] )
    currentGame = nil
    Hide()
end

local function localCreateWindowForBoxAddedAddons()
	if wndBoxPreview == nil	then
		wndBoxPreview = guienv:AddWindow( "media/textures/gameBox.png", 220, 130, 520, 520, -1, wndGBM )	
		wndBoxPreview:SetDrawBody( false )									  
		wndBoxPreview:SetDraggable( false )
		wndBoxPreview:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.ViewerLeftMouseButtonUp()" )
		flickAdded = guienv:AddFlick( "5%", "5%", "95%", "95%", 3, -1, wndBoxPreview )
	else
		flickAdded:Clear()
	end
		
	local width = 60	
	for i=1, currentGame.boxAddonsNumber do
		local tech = currentGame:GetBoxAddon( i-1 )

		local lbu = guienv:AddLinkBox( tech:GetName(), 0, 0, width, width, -1, flickAdded )
		lbu:SetDraggable( true )
		lbu:SetData( tech )
		lbu:SetTexture( tech:GetTexture() )
	    lbu:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./gameboxManager.AddonRigthMouseButtonUp()" )
	end
end

local function localCreateWindowForBoxAvaibleAddons()

	if wndAvaibleAddons == nil then 
		wndAvaibleAddons = guienv:AddWindow( "media/textures/gameAddons.png", 820, 135, 990, 540, -1, wndGBM )
		wndAvaibleAddons:SetDrawBody( false )										 										 
		wndAvaibleAddons:SetDraggable( false )
		flickAvaible = guienv:AddFlick( "5%", "5%", "95%", "95%", 2, -1, wndAvaibleAddons )
	else
		flickAvaible:Clear()
	end
	
	local width = 60	
	for i=1, applic.boxAddonNumber do
		local boxAddon = applic:GetGameBoxAddon( i-1 )
	
		local linkt = guienv:AddLinkBox( boxAddon:GetName(), 0, 0, width, width, -1, flickAvaible )
										 					 
		linkt:SetDraggable( true )
		linkt:SetData( boxAddon )
		linkt:SetTexture( boxAddon:GetTexture() )
		linkt:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.LinkLeftMouseButtonUp()" )
	end
end

local function localCreateBoxImage()
	textures = nil
	textures = {}
	for i=1, currentGame:GetBoxImageNumber() do
		base.table.insert( textures, currentGame:GetBoxImage( i-1 ) )
	end
	
	btnNextBoxImage = button.EqualeTexture( 540, 220, "button_next", wndGBM, -1, ">>", "./gameboxManager.NextBoxImage()" )
	btnNextBoxImage:SetVisible( false )
	btnPrevBoxImage = button.EqualeTexture( 120, 220, "button_prev", wndGBM, -1, "<<", "./gameboxManager.PrevBoxImage()" )
	btnPrevBoxImage:SetVisible( false )
	currentBoxIndex = 1
end

local function localCreateBoxViewerAndAddons()
	localCreateWindowForBoxAddedAddons()
	localCreateWindowForBoxAvaibleAddons()
	localCreateBoxImage()
		
	btnExit = guienv:AddButton( scrWidth - 100, scrHeight - 50, scrWidth - 10, scrHeight - 10, wndGBM, -1, "Выход" )
	btnExit:SetAction( "./gameboxManager.HideWithoutBox()" )	
	
	btnNext = guienv:AddButton( scrWidth - 200, scrHeight - 50, scrWidth - 110, scrHeight - 10, wndGBM, -1, "Создать" )
	btnNext:SetAction( "./gameboxManager.SelectBoxImage()" )	
end

--удаление дополнения к коробке
function AddonRigthMouseButtonUp( mp )
	local link = base.CLuaLinkBox( mp )
	 
	local tech = base.CLuaTech( link:GetData() )
	base.LogScript( tech:GetName() )
	currentGame:RemoveBoxAddon( tech:GetName() )
	link:Remove()	
	
	localCreateWindowForBoxAddedAddons()
end

--добавление нового элемента к коробке
function ViewerLeftMouseButtonUp()
	if currentAddon == nil then
		return 
	end
	
	local applied = currentGame:AddBoxAddon( currentAddon )
	
	if not applied then
		guienv:MessageBox( "Слишком мало места, попробуй другой предмет", false, false, "", "" )
	end
	
	guienv:SetDragObject( nil )
	currentAddon = nil
	dragLink:SetVisible( false )
	
	localCreateWindowForBoxAddedAddons()	
end

local function localResetWindow()
	local elm = base.CLuaElement( wndGBM )
	elm:RemoveChilds()
	
	dragLink = guienv:AddLinkBox( "", 0, 0, 50, 50, -1, wndGBM )
	dragLink:SetVisible( false )		
end

function MiniBox()
	localResetWindow()
	currentGame:CreateBox( 10 )
	localCreateBoxViewerAndAddons()
end	

function SetGame( mp )
	local btn = base.CLuaButton( mp )
	currentGame = company:GetGame( btn.text )
	
	localResetWindow()
	
	local flick = guienv:AddFlick( "5%", "5%", "95%", "95%", 3, -1, wndGBM )
	
	local btn = guienv:AddButton( 0, 0, 50, 50, flick, -1, "маленькая" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.MiniBox()" )
	
	btn = guienv:AddButton(  0, 0, 75, 75, flick, -1, "средняя" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectMiddleBox()" )
	
	btn = guienv:AddButton( 0, 0, 120, 120, flick, -1, "большая" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectBigBox()" )
	
	btn = guienv:AddButton( 0, 0, 150, 150, flick, -1, "мега" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectMegaBox()" )
	
	btn = guienv:AddButton( scrWidth - 200, 10, scrWidth -10, 50, wndGBM, -1, "Выход" )
	btn:SetAction( "./gameboxManager.Hide()" )	
end

function Show()
	company = applic.playerCompany

	if wndGBM == nil then	
		wndGBM = guienv:AddWindow( "media/maps/plant_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		wndGBM:SetDraggable( false )
		wndGBM:GetCloseButton():SetVisible( false )
		wndGBM:SetVisible( false )
		
		wndGBM:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.WindowLeftMouseButtonUp()" )
		
		--adding closeButton
		button.Stretch( "80e", "60e", "60+", "60+", 
		 			    "button_down", wndGBM, -1, "",
						"./gameboxManager.Hide()" )
	end
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "gameboxManager.FadeEnterAction()" )
end

function WindowLeftMouseButtonUp( ptr )
	guienv:SetDragObject( nil )
	currentAddon = nil
	dragLink:SetVisible( false )
end

function LinkLeftMouseButtonUp( mp )
	local elm = base.CLuaElement( mp )
	
	if not elm.empty and elm:GetTypeName() == base.ELEMENT_GUILINKBOX then
		local linkt = base.CLuaLinkBox( elm ) 
		currentAddon = base.CLuaTech( linkt:GetData() )
	        
		if linkt:IsDraggable() and linkt:IsEnabled() 
		then
			dragLink:SetData( linkt:GetData() )
			dragLink:SetTexture( linkt:GetTexture() )
			dragLink:SetModuleType( linkt:GetModuleType() )
			guienv:SetDragObject( dragLink )
			dragLink:SetVisible( true )
		end
	end
end
