local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )
local wndGBM = CLuaWindow( nil )
local company = CLuaCompany( nil )
local gameBox = CLuaGameBox( nil )

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
													  wndGBM:Self(), WINDOW_GAMEBOXWIZ_ID + gameWithoutBox + 1, game:GetName() ) )												
			btn:SetAction( "sworkGameBoxManagerSetGame" )			 
			gameWithoutBox = gameWithoutBox + 1
		end
		
	end
	
	if gameWithoutBox == 0 then 
		guienv:AddLabel( "Нет игр", width / 2, height / 2, width / 2 + 100, height / 2 + 50, -1, wndGBM:Self() )
	end
end

local function CreateElementsForBoxSizeSelect()
	wndGBM:RemoveChilds()
	
	local btn = CLuaButton( guienv:AddButton( 20, 20, 20 + 200, 20 + 200, wndGBM:Self(), -1, "маленькая" ) )							  
	btn:SetObject( guienv:AddButton( 220, 20, 220 + 200, 20 + 200, wndGBM:Self(), -1, "средняя" ) )	
	btn:SetObject( guienv:AddButton( 20, 220, 20 + 200, 220 + 200, wndGBM:Self(), -1, "большая" ) )
	btn:SetObject( guienv:AddButton( 220, 220, 220 + 200, 220 + 200, wndGBM:Self(), -1, "мега" ) )		
end

function sworkGameBoxManagerSetGame( ptr )
	local btn = CLuaButton( ptr )
	local id = btn:GetID() - WINDOW_GAMEBOXWIZ_ID - 1
end

function sworkCreateGameBoxManagerWindow( ptr )

	company:SetObject( applic:GetPlayerCompany() )
	wndGBM:SetObject( guienv:GetElementByName( WINDOW_GAMEBOXWIZ_NAME ) )
	wndGBM:Remove()
	
	wndGBM:SetObject( guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_GAMEBOXWIZ_ID, guienv:GetRootGUIElement() ) )
	wndGBM:SetName( WINDOW_GAMEBOXWIZ_NAME )
	
	CreateElementsForGameSelect()
end
