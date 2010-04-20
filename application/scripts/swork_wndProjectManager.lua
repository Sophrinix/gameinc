local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )

local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеы" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"

local width = 800
local height = 600
local WNDPRJMNG_CMBX_SELECT_PRJ = WINDOW_PRJMANAGE_ID + 1

function sworkCreateWindowProjectManager( ptr )
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local windowg = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID ) )
	
	if windowg:Empty() == 1 then
		windowg:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_PRJMANAGE_ID, guienv:GetRootGUIElement() ) )
	else
		local elm = CLuaElement( windowg:Self() )
		elm:RemoveChilds()
	end
	
	local cmbxPrj = CLuaComboBox( guienv:AddComboBox( "", 10, 20, width / 2 - 10, 40, WNDPRJMNG_CMBX_SELECT_PRJ, windowg:Self() ) )
	local cmpProjectNumber = company:GetProjectNumber()
	
	for i=1, cmpProjectNumber do
		local ptrProject = company:GetProject( i-1 )
		cmbxPrj:AddItem( "", ptrProject:GetName() )
	end
	
	local button = CLuaButton( guienv:AddButton( 10, 10, 200, 100, windowg:Self(), -1, "Программисты" ) )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	button:SetObject( guienv:AddButton( 210, 10, 400, 100, windowg:Self(), -1, "Дизайнеры" ) )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )

	button:SetObject( guienv:AddButton( 410, 10, 600, 100, windowg:Self(), -1, "Композиторы" ) )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	button:SetObject( guienv:AddButton( 610, 10, 800, 100, windowg:Self(), -1, "Тестировщики" ) )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	listBoxCompanyEmployers:SetObject( guienv:AddListBox( 10, 110, 200, 590, -1, windowg:Self() ) )
	windowg:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "sworkCreateAdvancedUserInfoWindow" )
	
	ShowAvaibleEmployersToManage()
end