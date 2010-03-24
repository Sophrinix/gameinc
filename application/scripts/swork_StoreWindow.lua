require("elmid")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local driver = CLuaDriver( NrpGetVideoDriver() )

local tableHeight = 180
local scrWidth = 0
local scrHeight = 0 
	
scrWidth, scrHeight = driver:GetScreenSize()

function AddStoreWindow()

	local width = 0
	local height = 0
	
	local windowg = CLuaWindow( guienv:AddWindow( "storeWindow", 180, scrHeight - tableHeight, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( NAME_STORE_WINDOW )

end