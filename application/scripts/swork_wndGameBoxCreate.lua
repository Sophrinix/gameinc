local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )
local wndGBM = CLuaWindow( nil )
local company = CLuaCompany( nil )
local currentGame = CLuaGame( nil )
local currentAddon = CLuaTech( nil )

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
			btn:SetAction( "sworkGameBoxManagerSetGame" )			 
			gameWithoutBox = gameWithoutBox + 1
		end
		
	end
	
	if gameWithoutBox == 0 then 
		guienv:AddLabel( "Нет игр", width / 2, height / 2, width / 2 + 100, height / 2 + 50, -1, wndGBM:Self() )
	end
end

function sworkGameBoxManagerClose( ptr )
	wndGBM:Remove()
end

function sworkGameBoxManagerSelectMiniBox( ptr )
	local itemCount = 0
	local linet = 0
	local columnt = 0
	local addonNumber = applic:GetGameBoxAddonNumber() 
	guienv:MessageBox( "Всего дополнений "..addonNumber, false, false, "", "" )
	for i=1, addonNumber do
		local boxAddon = CLuaTech( applic:GetGameBoxAddon( i-1 ) )
		if boxAddon:GetIntParam("size") == 0 then
			linet = itemCount / 3
			columnt = itemCount % 3
			itemCount = itemCount + 1
			local linkt = CLuaLinkBox( guienv:AddLinkBox( boxAddon:GetName(), 
														 width - (columnt+1) * 60 - 10, 20 + linet * 60, 
														 width - columnt * 60 - 10, 20 + (linet+1) * 60, 
														 -1, wndGBM:Self() ) )
			linkt:SetDraggable( true )
			linkt:SetData( boxAddon:Self() )
			linkt:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkGameBoxManagerLinkLeftMouseButtonUp" )
		end
	end
end

function sworkGameBoxManagerSetGame( ptr )
	local btn = CLuaButton( ptr )
	currentGame:SetObject( company:GetGame( btn:GetText() ) )
	
	local elm = CLuaElement( wndGBM:Self() )
	elm:RemoveChilds()
	
	local btn = CLuaButton( guienv:AddButton( 20, 20, 20 + 200, 20 + 200, wndGBM:Self(), -1, "маленькая" ) )							  
	btn:SetAction( "sworkGameBoxManagerSelectMiniBox" )
	
	btn:SetObject( guienv:AddButton( 220, 20, 220 + 200, 20 + 200, wndGBM:Self(), -1, "средняя" ) )	
	btn:SetAction( "sworkGameBoxManagerSelectMiddleBox" )
	
	btn:SetObject( guienv:AddButton( 20, 220, 20 + 200, 220 + 200, wndGBM:Self(), -1, "большая" ) )
	btn:SetAction( "sworkGameBoxManagerSelectBigBox" )
	
	btn:SetObject( guienv:AddButton( 220, 220, 220 + 200, 220 + 200, wndGBM:Self(), -1, "мега" ) )		
	btn:SetAction( "sworkGameBoxManagerSelectMegaBox" )
	
	btn:SetObject( guienv:AddButton( width - 200, height - 50, width -10, height - 10, wndGBM:Self(), -1, "Выход" ) )	
	btn:SetAction( "sworkGameBoxManagerClose" )	
end

function sworkCreateGameBoxManagerWindow( ptr )

	company:SetObject( applic:GetPlayerCompany() )
	wndGBM:SetObject( guienv:GetElementByName( WINDOW_GAMEBOXWIZ_NAME ) )
	wndGBM:Remove()
	
	wndGBM:SetObject( guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_GAMEBOXWIZ_ID, guienv:GetRootGUIElement() ) )
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
	
		if linkt:IsDraggable() and linkt:IsEnabled() then
			guienv:SetDragObject( linkt:Self() )
			currentAddon:SetObject( linkt:GetData() )
		end
	end
end
