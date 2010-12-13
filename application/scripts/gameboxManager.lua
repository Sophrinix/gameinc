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
local boxImagePictureFlow = nil

local windowImageViewer = nil
local textureForBox = nil

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
							   wndGBM:Self(), 
							   -1, "", "./gameboxManager.SetGame()" )
		  										
			gameWithoutBox = gameWithoutBox + 1
		end
	end
	
	if gameWithoutBox == 0 then 
		local image = guienv:AddImage( scrWidth / 2 - 300, scrHeight / 2 - 300, 
									   scrWidth / 2 + 300, scrHeight / 2 + 300, 
									   wndGBM:Self(), -1, "" )
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

function HideWithBox()
	local pathToImage = currentGame:GetBoxImage(  boxImagePictureFlow:GetSelected() )
    currentGame:SetViewImage( pathToImage )
    currentGame = nil
    Hide()
end

local function localCreateWindowForBoxAddedAddons()
	if wndBoxPreview ~= nil then
		wndBoxPreview:Remove()
	end
	
	guienv:AddLabel( "Выбор обложки для коробки:", 
					 scrWidth / 2 + 10, 20, scrWidth - 10, 40, 
					 -1, wndGBM:Self() )
	wndBoxPreview = guienv:AddWindow( textureForBox, 
					   				  20, 80 + scrHeight * 0.2, scrWidth - 300, scrHeight - 20, 
									  -1, wndGBM:Self() )
				
	wndBoxPreview:SetDrawBody( false )									  
	wndBoxPreview:SetDraggable( false )
	wndBoxPreview:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.ViewerLeftMouseButtonUp()" )
	
	local w, h = wndBoxPreview:GetSize()
		
	for i=1, currentGame:GetBoxAddonsNumber() do
		local tech = currentGame:GetBoxAddon( i-1 )
		local txsw, txsh = base.driver:GetTexture( tech:GetTexture() ):GetSize()

		local x = base.math.random( w - txsw )
		local y = base.math.random( h - txsh )

		local lbu = guienv:AddLinkBox( tech:GetName(), 
													5 + x, 5 + y,
													5 + x + txsw, 5 + y + txsh,
													-1, wndBoxPreview:Self() )
		lbu:SetData( tech:Self() )
		lbu:SetTexture( tech:GetTexture() )
	    lbu:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./gameboxManager.AddonRigthMouseButtonUp()" )
	end
end

local function localCreateWindowForBoxAvaibleAddons()
	if wndAvaibleAddons ~= nil then wndAvaibleAddons:Remove() end
	
	wndAvaibleAddons = guienv:AddWindow( "", scrWidth - 200, 5, scrWidth - 5, scrHeight - 50, 
										 -1, wndGBM:Self() )
    wndAvaibleAddons:SetDrawBody( false )										 										 
	wndAvaibleAddons:SetDraggable( false )
	wndAvaibleAddons:SetText( "Avaible box addon" )

	local xoff = 0
	local yoff = 0
	local width = 60	
	for i=1, applic:GetGameBoxAddonNumber() do
		local boxAddon = applic:GetGameBoxAddon( i-1 )
	
		local linkt = guienv:AddLinkBox( boxAddon:GetName(), 
	  								     xoff, yoff, 
						  				 xoff + width, yoff + width, 
										 -1, wndAvaibleAddons:Self() )
										 
		yoff = yoff + width
		if yoff > scrHeight then
			xoff = xoff + width
			yoff = 0
		end										 
		linkt:SetDraggable( true )
		linkt:SetData( boxAddon:Self() )
		linkt:SetTexture( boxAddon:GetTexture() )
		linkt:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.LinkLeftMouseButtonUp()" )
	end
end

local function localCreateBoxViewerAndAddons()
	localCreateWindowForBoxAddedAddons()
	localCreateWindowForBoxAvaibleAddons()
		
	local btn = guienv:AddButton( scrWidth - 100, scrHeight - 50, scrWidth - 10, scrHeight - 10, wndGBM:Self(), -1, "Выход" )
	btn:SetAction( "./gameboxManager.HideWithoutBox()" )	
	
	btn = guienv:AddButton( scrWidth - 200, scrHeight - 50, scrWidth - 110, scrHeight - 10, wndGBM:Self(), -1, "Создать" )
	btn:SetAction( "./gameboxManager.HideWithBox()" )	
end

--удаление дополнения к коробке
function AddonRigthMouseButtonUp()
	local link = base.CLuaLinkBox( base.NrpGetSender() )
	 
	local tech = base.CLuaTech( link:GetData() )
	currentGame:RemoveBoxAddon( tech:GetName() )
	link:Remove()	
	
	localCreateWindowForBoxAddedAddons()
end

--добавление нового элемента к коробке
function ViewerLeftMouseButtonUp()
	local applied = currentGame:AddBoxAddon( currentAddon:Self() )
	
	if not applied then
		guienv:MessageBox( "Слишком мало места, попробуй другой предмет", false, false, "", "" )
	end
	
	guienv:SetDragObject( nil )
	currentAddon = nil
	
	localCreateWindowForBoxAddedAddons()	
end

local function localCreateBoxImagePictureFlow()
	if boxImagePictureFlow ~= nil then
		boxImagePictureFlow:Remove()
	end
	
	boxImagePictureFlow = guienv:AddPictureFlow(  20, 60, scrWidth - 200, 60 + scrHeight * 0.2, 
	   									          -1, wndGBM:Self() )
	boxImagePictureFlow:SetPictureRect( 0, 0, scrHeight * 0.2 - 40,  scrHeight * 0.2 - 40 )

	for i=1, currentGame:GetBoxImageNumber() do
		boxImagePictureFlow:AddItem( currentGame:GetBoxImage( i-1 ), 
									 currentGame:GetBoxImage( i-1 ), 
									 nil )
	end
end

function MiniBox()
	local elm = base.CLuaElement( wndGBM )
	elm:RemoveChilds()
	currentGame:CreateBox( 10 )
	textureForBox = "media/textures/miniBox.png"
	localCreateBoxImagePictureFlow()
	localCreateBoxViewerAndAddons()
end	

function SetGame()
	local btn = base.CLuaButton( base.NrpGetSender() )
	currentGame = company:GetGame( btn:GetText() )
	
	local elm = base.CLuaElement( wndGBM )
	elm:RemoveChilds()
	
	local btn = guienv:AddButton( scrWidth / 4 - 50, scrHeight / 4 - 50, 
								  scrWidth / 4 + 50, scrHeight / 4 + 50,
								  wndGBM:Self(), -1, "маленькая" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.MiniBox()" )
	
	btn = guienv:AddButton(  3 * scrWidth / 4 - 75, scrHeight / 4 - 75, 
							  3 * scrWidth / 4 + 75, scrHeight / 4 + 75, wndGBM:Self(), -1, "средняя" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectMiddleBox()" )
	
	btn = guienv:AddButton( scrWidth / 4 - 120, 3 * scrHeight / 4 - 120, 
							 scrWidth / 4 + 120, 3 * scrHeight / 4 + 120, wndGBM:Self(), -1, "большая" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectBigBox()" )
	
	btn = guienv:AddButton( 3 * scrWidth / 4 - 150, 3 * scrHeight / 4 - 150, 
							 3 * scrWidth / 4 + 150, 3 * scrHeight / 4 + 150, wndGBM:Self(), -1, "мега" )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "./gameboxManager.SelectMegaBox()" )
	
	btn = guienv:AddButton( scrWidth - 200, 10, scrWidth -10, 50, wndGBM:Self(), -1, "Выход" )
	btn:SetAction( "./gameboxManager.Hide()" )	
end

function Show()
	company = applic:GetPlayerCompany()

	if wndGBM == nil then	
		wndGBM = guienv:AddWindow( "media/maps/plant_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		wndGBM:SetDraggable( false )
		wndGBM:GetCloseButton():SetVisible( false )
		wndGBM:SetVisible( false )
		
		wndGBM:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.WindowLeftMouseButtonUp()" )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", wndGBM:Self(), -1, "",
						"./gameboxManager.Hide()" )
	end
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "gameboxManager.FadeEnterAction()" )
end

function WindowLeftMouseButtonUp( ptr )
	guienv:SetDragObject( nil )
	currentAddon = nil
end

function LinkLeftMouseButtonUp()
	local elm = base.CLuaElement( base.NrpGetSender() )
	
	if elm:Empty() == 0 and elm:GetTypeName() == "CNrpGuiLinkBox" then
		local linkt = base.CLuaLinkBox( elm ) 
		local boxAddon = base.CLuaTech( linkt:GetData() )
	
		if linkt:IsDraggable() and linkt:IsEnabled() 
		then
			guienv:SetDragObject( linkt )
			currentAddon = base.CLuaTech( linkt:GetData() )
		end
	end
end
