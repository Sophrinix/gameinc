require("elmid")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local tableHeight = 180
local minimapWidth = 180

local scrWidth = 0
local scrHeight = 0 
	
local driver = CLuaDriver( NrpGetVideoDriver() )
scrWidth, scrHeight = driver:GetScreenSize()

function AddFormular()

	local width = 0
	local height = 0
	
	local tabler = CLuaTable( guienv:AddTable( minimapWidth, scrHeight - tableHeight, 
											   scrWidth, scrHeight, ID_FORMULAR, guienv:GetRootGUIElement() ) )
	
	tabler:SetName( NAME_FORMULAR )
	
	width, height =	tabler:GetSize() 
	tabler:SetAlpha( 120 );
	tabler:AddColumn( "Номер", -1 );
	tabler:SetColumnWidth( 0, width / 2 - 1);
	
	tabler:AddColumn( "Время", -1 );
	tabler:SetColumnWidth( 1, width / 2 - 1);

	tabler:AddRow( 0 );
	tabler:AddRow( 1 );
	tabler:AddRow( 2 );
	tabler:AddRow( 3 );
	tabler:AddRow( 4 );
	tabler:AddRow( 5 );

	local button = CLuaButton( guienv:AddButton( width - 30, 0, width, 30, tabler:Self(), -1, "События") )
	button:SetAction( "ToggleVisibleFormularAndEventTable" )
	
end

function AddEventTable()

	local width = 0
	local height = 0
	
	local tabler = CLuaTable( guienv:AddTable( minimapWidth, scrHeight - tableHeight, 
											   scrWidth, scrHeight, ID_EVENT_TABLE, guienv:GetRootGUIElement() ) );
	tabler:SetName( NAME_EVENT_TABLE )
	
	width, height =	tabler:GetSize() 
	
	tabler:SetAlpha( 120 );
	tabler:AddColumn( "Время", -1 );
	tabler:SetColumnWidth( 0, 40 );
	
	tabler:AddColumn( "Важн.", -1 );
	tabler:SetColumnWidth( 1, 40 );
	
	tabler:AddColumn( "Событие", -1 );
	tabler:SetColumnWidth( 2, 40 );

	local button = CLuaButton( guienv:AddButton( width - 30, 0, width, 30, tabler:Self(), -1, "Цели") )
	button:SetAction( "ToggleVisibleFormularAndEventTable" )
	
	tabler:SetVisible( false )
	sceneManager:AddSceneFunction( SCENE_USER_EVENT, "ResolveUserEvent" )
	
end