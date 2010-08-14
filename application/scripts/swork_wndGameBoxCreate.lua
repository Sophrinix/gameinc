local wndGBM = nil
local company = nil
local currentGame = CLuaGame( nil )
local currentAddon = CLuaTech( nil )
local wndBoxPreview = nil
local wndAvaibleAddons = nil
local boxImagePictureFlow = CLuaPictureFlow( nil )

local windowImageViewer = CLuaWindow( nil )

local width = 800
local height = 600

local function CreateElementsForGameSelect()
	local row = 0
	local columnt = 0
	local gameWithoutBox = 0
	for i=1, company:GetGameNumber() do
		local game = CLuaGame( company:GetGame( i-1 ) )
		if not game:HaveBox() then 
			row = gameWithoutBox/3
			columnt = gameWithoutBox%3
			
			local btn = CLuaButton( guienv:AddButton( width/3 * columnt, height/3 * row, 
												 	  width/3 * ( columnt + 1 ), height/3 * (row+1),
													  wndGBM:Self(), -1, game:GetName() ) )		
			btn:SetImage( 0, 0, 0, 0, game:GetScreenshot( -1 ) )				  										
			btn:SetAction( "sworkGameBoxManagerSetGame" )			 
			gameWithoutBox = gameWithoutBox + 1
		end
		
	end
	
	if gameWithoutBox == 0 then 
		guienv:AddLabel( "Нет игр", width / 2, height / 2, width / 2 + 100, height / 2 + 50, -1, wndGBM:Self() )
	end
end

function sworkGameBoxManagerClose( ptr )
	local btn = CLuaButton( ptr )
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
					   				  5, height * 0.7, width - 200, height - 5, 
									  -1, wndGBM:Self() )
									  
	wndBoxPreview:SetDraggable( false )
	wndBoxPreview:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameBoxManagerWindowViewerLeftMouseButtonUp" )
	
	local w, h = wndBoxPreview:GetSize()
		
	for i=1, currentGame:GetBoxAddonsNumber() do
		local tech = CLuaTech( currentGame:GetBoxAddon( i-1 ) )
		local lbu = CLuaLinkBox( guienv:AddLinkBox( tech:GetName(), 
													5 + (h-10) * ( i - 1 ), 5,
													5 + (h-10) * i, h - 5,
													-1, wndBoxPreview:Self() ) )
		lbu:SetData( tech:Self() )
		lbu:SetTexture( tech:GetTexture() )
	    lbu:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkGameBoxManagerWindowAddonRigthMouseButtonUp" )
	end
end

local function localCreateWindowForBoxAvaibleAddons()
	wndAvaibleAddons:Remove()
	wndAvaibleAddons = guienv:AddWindow( "", width - 200, 5, width - 5, height - 50, 
										 -1, wndGBM:Self() )
	wndAvaibleAddons:SetDraggable( false )
	wndAvaibleAddons:SetText( "Avaible box addon" )
	
	local itemCount = 0
	local linet = 0
	local columnt = 0
	guienv:MessageBox( "Всего дополнений "..applic:GetGameBoxAddonNumber(), false, false, "", "" )
	for i=1, applic:GetGameBoxAddonNumber() do
		local boxAddon = CLuaTech( applic:GetGameBoxAddon( i-1 ) )
		if boxAddon:GetLevel() < currentGame:GetBoxLevel() and not currentGame:IsMyBoxAddon( boxAddon:GetName() ) then
			linet = itemCount / 3
			columnt = itemCount % 3
			itemCount = itemCount + 1
			local linkt = CLuaLinkBox( guienv:AddLinkBox( boxAddon:GetName(), 
														  (columnt) * 60 - 10, 20 + linet * 60, 
						  								  (columnt+1) * 60 - 10, 20 + (linet+1) * 60, 
														  -1, wndAvaibleAddons:Self() ) )
			linkt:SetDraggable( true )
			linkt:SetData( boxAddon:Self() )
			linkt:SetTexture( boxAddon:GetTexture() )
			linkt:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameBoxManagerLinkLeftMouseButtonUp" )
		end
	end
end

local function localCreateBoxViewerAndAddons()
	localCreateWindowForBoxAddedAddons()
	localCreateWindowForBoxAvaibleAddons()
		
	local btn = CLuaButton( guienv:AddButton( width - 100, height - 50, width - 10, height - 10, wndGBM:Self(), -1, "Выход" ) )	
	btn:SetAction( "sworkGameBoxManagerClose" )	
	
	btn:SetObject( guienv:AddButton( width - 200, height - 50, width - 110, height - 10, wndGBM:Self(), -1, "Создать" ) )	
	btn:SetAction( "sworkGameBoxManagerClose" )	
end

--удаление дополнения к коробке
function sworkGameBoxManagerWindowAddonRigthMouseButtonUp( ptr )
	local link = CLuaLinkBox( ptr ) 
	local tech = CLuaTech( link:GetData() )
	currentGame:RemoveBoxAddon( tech:GetName() )
	link:Remove()	
	
	localCreateBoxViewerAndAddons()
end

--добавление нового элемента к коробке
function sworkGameBoxManagerWindowViewerLeftMouseButtonUp( ptr )
	currentGame:AddBoxAddon( currentAddon:Self() )
	
	guienv:SetDragObject( nil )
	
	localCreateBoxViewerAndAddons()	
end

local function localCreateBoxImagePictureFlow()
	boxImagePictureFlow:Remove()
	
	boxImagePictureFlow = CLuaPictureFlow( guienv:AddPictureFlow(  20, 20, 
	   									                           width - 200, height * 0.7, 
	   									                           -1, wndGBM:Self() ) )
	boxImagePictureFlow:SetPictureRect( 0, 0, (width - 200) / 3, (width - 200) / 3 )

	for i=1, currentGame:GetBoxImageNumber() do
		boxImagePictureFlow:AddItem( currentGame:GetBoxImage( i-1 ), 
									 currentGame:GetBoxImage( i-1 ), 
									 nil )
	end
end

function sworkGameBoxManagerSelectMiniBox( ptr )
	local elm = CLuaElement( wndGBM:Self() )
	elm:RemoveChilds()
	currentGame:CreateBox( 1 )
	localCreateBoxImagePictureFlow()
	localCreateBoxViewerAndAddons()
end	

function sworkGameBoxManagerSetGame( ptr )
	local btn = CLuaButton( ptr )
	currentGame:SetObject( company:GetGame( btn:GetText() ) )
	
	local elm = CLuaElement( wndGBM:Self() )
	elm:RemoveChilds()
	
	local btn = CLuaButton( guienv:AddButton( width / 4 - 50, height / 4 - 50, 
											  width / 4 + 50, height / 4 + 50,
											  wndGBM:Self(), -1, "маленькая" ) )	
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "sworkGameBoxManagerSelectMiniBox" )
	
	btn:SetObject( guienv:AddButton(  3 * width / 4 - 75, height / 4 - 75, 
									  3 * width / 4 + 75, height / 4 + 75, wndGBM:Self(), -1, "средняя" ) )	
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "sworkGameBoxManagerSelectMiddleBox" )
	
	btn:SetObject( guienv:AddButton( width / 4 - 120, 3 * height / 4 - 120, 
									 width / 4 + 120, 3 * height / 4 + 120, wndGBM:Self(), -1, "большая" ) )
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "sworkGameBoxManagerSelectBigBox" )
	
	btn:SetObject( guienv:AddButton( 3 * width / 4 - 150, 3 * height / 4 - 150, 
									 3 * width / 4 + 150, 3 * height / 4 + 150, wndGBM:Self(), -1, "мега" ) )		
	btn:SetImage( 0, 0, 0, 0, "media/textures/boxWithoutImage.png" )						  
	btn:SetAction( "sworkGameBoxManagerSelectMegaBox" )
	
	btn:SetObject( guienv:AddButton( width - 200, 10, width -10, 50, wndGBM:Self(), -1, "Выход" ) )	
	btn:SetAction( "sworkGameBoxManagerClose" )	
end

function sworkCreateGameBoxManagerWindow( ptr )
	company = applic:GetPlayerCompany()
	wndGBM:SetObject( guienv:GetElementByName( WINDOW_GAMEBOXWIZ_NAME ) )
	wndGBM:Remove()
	
	wndGBM = guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_GAMEBOXWIZ_ID, guienv:GetRootGUIElement() )
	wndGBM:SetName( WINDOW_GAMEBOXWIZ_NAME )
	wndGBM:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameBoxManagerWindowLeftMouseButtonUp" )
	
	CreateElementsForGameSelect()
end

function sworkWindowLeftMouseButtonUp( ptr )
	guienv:SetDragObject( nil )
	currentAddon:SetObject( nil )
end

function sworkGameBoxManagerLinkLeftMouseButtonUp( ptr )
	local elm = CLuaElement( ptr )
	
	if elm:Empty() == 0 and elm:GetTypeName() == "CNrpGuiLinkBox" then
		local linkt = CLuaLinkBox( elm:Self() ) 
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
