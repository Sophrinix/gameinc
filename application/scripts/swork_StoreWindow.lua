require("elmid")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local tableHeight = 180
local minimapWidth = 180

local scrWidth = 0
local scrHeight = 0 
	
local driver = CLuaDriver( NrpGetVideoDriver() )
scrWidth, scrHeight = driver:GetScreenSize()

function AddStorePanel()

	local width = 0
	local height = 0
	
	local tabler = CLuaTable( guienv:AddTable( minimapWidth, scrHeight - tableHeight, 
											   scrWidth, scrHeight, ID_FORMULAR, guienv:GetRootGUIElement() ) )
	
	tabler:SetName( NAME_FORMULAR )
	
	width, height =	tabler:GetSize() 
	tabler:SetAlpha( 120 );
	
	for i=0, 10 do
		tabler:AddColumn( "c="..i, -1 );
		tabler:SetColumnWidth( 0, width / 10 );	
	end
	
	tabler:AddRow( 0 );
	tabler:AddRow( 1 );	
	tabler:SetRowHeight( height / 2 )
end