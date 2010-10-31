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

local function CreateElementsForGameSelect()
	local row = 0
	local columnt = 0
	local gameWithoutBox = 0
	local offset = 40
	local delimiter = 4
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		if not game:HaveBox() then 
			row = gameWithoutBox/3
			columnt = gameWithoutBox%3
			
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

function Hide()
	local btn = base.CLuaButton( base.NrpGetSender() )
	
	if btn:GetText() == "Выход" then
		currentGame:RemoveBox()
	else
	    currentGame:SetViewImage( boxImagePictureFlow:GetSelectedItem() )
	end
	wndGBM:Remove()
end

local function localCreateWindowForBoxAddedAddons()
	wndBoxPreview:Remove()
	wndBoxPreview = guienv:AddWindow( "Added addons box", 
					   				  5, scrHeight * 0.7, scrWidth - 200, scrHeight - 5, 
									  -1, wndGBM:Self() )
									  
	wndBoxPreview:SetDraggable( false )
	wndBoxPreview:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.ViewerLeftMouseButtonUp()" )
	
	local w, h = wndBoxPreview:GetSize()
		
	for i=1, currentGame:GetBoxAddonsNumber() do
		local tech = currentGame:GetBoxAddon( i-1 )
		local lbu = guienv:AddLinkBox( tech:GetName(), 
													5 + (h-10) * ( i - 1 ), 5,
													5 + (h-10) * i, h - 5,
													-1, wndBoxPreview:Self() )
		lbu:SetData( tech:Self() )
		lbu:SetTexture( tech:GetTexture() )
	    lbu:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./gameboxManager.AddonRigthMouseButtonUp()" )
	end
end

local function localCreateWindowForBoxAvaibleAddons()
	wndAvaibleAddons:Remove()
	wndAvaibleAddons = guienv:AddWindow( "", scrWidth - 200, 5, scrWidth - 5, scrHeight - 50, 
										 -1, wndGBM:Self() )
	wndAvaibleAddons:SetDraggable( false )
	wndAvaibleAddons:SetText( "Avaible box addon" )
	
	local itemCount = 0
	local linet = 0
	local columnt = 0
	guienv:MessageBox( "Всего дополнений "..applic:GetGameBoxAddonNumber(), false, false, "", "" )
	for i=1, applic:GetGameBoxAddonNumber() do
		local boxAddon = applic:GetGameBoxAddon( i-1 )
		if boxAddon:GetLevel() < currentGame:GetBoxLevel() and not currentGame:IsMyBoxAddon( boxAddon:GetName() ) then
			linet = itemCount / 3
			columnt = itemCount % 3
			itemCount = itemCount + 1
			local linkt = guienv:AddLinkBox( boxAddon:GetName(), 
														  (columnt) * 60 - 10, 20 + linet * 60, 
						  								  (columnt+1) * 60 - 10, 20 + (linet+1) * 60, 
														  -1, wndAvaibleAddons:Self() )
			linkt:SetDraggable( true )
			linkt:SetData( boxAddon:Self() )
			linkt:SetTexture( boxAddon:GetTexture() )
			linkt:AddLuaFunction( base.GUIELEMENT_LMOUSE_LEFTUP, "./gameboxManager.LinkLeftMouseButtonUp()" )
		end
	end
end

local function localCreateBoxViewerAndAddons()
	localCreateWindowForBoxAddedAddons()
	localCreateWindowForBoxAvaibleAddons()
		
	local btn = guienv:AddButton( scrWidth - 100, scrHeight - 50, scrWidth - 10, scrHheight - 10, wndGBM:Self(), -1, "Выход" )
	btn:SetAction( "./gameboxManager.Hide()" )	
	
	btn = guienv:AddButton( scrWidth - 200, scrHeight - 50, scrWidth - 110, scrHeight - 10, wndGBM:Self(), -1, "Создать" )
	btn:SetAction( "./gameboxManager.Hide()" )	
end

--удаление дополнения к коробке
function AddonRigthMouseButtonUp()
	local link = base.CLuaLinkBox( base.NrpGetSender() )
	 
	local tech = CLuaTech( link:GetData() )
	currentGame:RemoveBoxAddon( tech:GetName() )
	link:Remove()	
	
	localCreateBoxViewerAndAddons()
end

--добавление нового элемента к коробке
function WindowLeftMouseButtonUp()
	currentGame:AddBoxAddon( currentAddon:Self() )
	
	guienv:SetDragObject( nil )
	
	localCreateBoxViewerAndAddons()	
end

local function localCreateBoxImagePictureFlow()
	boxImagePictureFlow:Remove()
	
	boxImagePictureFlow = guienv:AddPictureFlow(  20, 20, 
	   									          width - 200, height * 0.7, 
	   									          -1, wndGBM:Self() )
	boxImagePictureFlow:SetPictureRect( 0, 0, (width - 200) / 3, (width - 200) / 3 )

	for i=1, currentGame:GetBoxImageNumber() do
		boxImagePictureFlow:AddItem( currentGame:GetBoxImage( i-1 ), 
									 currentGame:GetBoxImage( i-1 ), 
									 nil )
	end
end

function SelectMiniBox()
	local elm = base.CLuaElement( wndGBM:Self() )
	elm:RemoveChilds()
	currentGame:CreateBox( 1 )
	localCreateBoxImagePictureFlow()
	localCreateBoxViewerAndAddons()
end	

function SetGame()
	local btn = base.CLuaButton( base.NrpGetSender() )
	currentGame = company:GetGame( btn:GetText() )
	
	local elm = base.CLuaElement( wndGBM:Self() )
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
	
	btn = guienv:AddButton( width - 200, 10, width -10, 50, wndGBM:Self(), -1, "Выход" )
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
	else
		wndGBM:SetVisible( true )
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
		local linkt = base.CLuaLinkBox( elm:Self() ) 
		local boxAddon = CLuaTech( linkt:GetData() )
	
		if linkt:IsDraggable() and 
		   linkt:IsEnabled() and 
		   not currentGame:IsMyBoxAddon( boxAddon:GetName() ) 
		then
			guienv:SetDragObject( linkt:Self() )
			currentAddon:SetObject( linkt:GetData() )
		end
	end
end
