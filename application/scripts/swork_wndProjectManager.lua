local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )

local mode = { }
mode[ "������������" ] = "coder"
mode[ "��������" ] = "designer"
mode[ "�����������" ] = "composer"
mode[ "������������" ] = "tester"
--"coder" "designer" "composer" "tester"

local currentEmployer = CLuaUser( nil )
local currentProject = CLuaProject( nil )
local currentComponent = CLuaTech( nil )

local modeUserView = "coder"

local width = 800
local height = 600
local WNDPRJMNG_CMBX_SELECT_PRJ = WINDOW_PRJMANAGE_ID + 1
local WNDPRJMNG_PNL_USER_INFO = WINDOW_PRJMANAGE_ID + 2
local WNDPRJMNG_CMBX_USERS = WINDOW_PRJMANAGE_ID + 3
local WINDOW_PRJMANAGE_USERINFO = WINDOW_PRJMANAGE_ID + 4
local WINDOW_PRJMANAGE_COMPONENTS = WINDOW_PRJMANAGE_ID + 5

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
	LogScript( "Company Project number="..cmpProjectNumber )
	for i=1, cmpProjectNumber do
		local ptrProject = CLuaProject( company:GetProject( i-1 ) )
		
		if ptrProject:GetTechType() == PT_GAME then
			cmbxPrj:AddItem( "����:" .. ptrProject:GetName(), ptrProject:Self() )	
		end
		
		if ptrProject:GetTechType() == PT_GAMEENGINE then
				cmbxPrj:AddItem( "������:" .. ptrProject:GetName(), ptrProject:Self() )	
		end
	end

	local lbxProjectComponents = CLuaListBox( guienv:AddComponentListBox( 10, 50, 300, 590, WINDOW_PRJMANAGE_COMPONENTS, windowg:Self() ) )
	
	local btnCmpAction = CLuaButton( guienv:AddButton( width / 2 + 10, 10, width - 10, 40, windowg:Self(), -1, "���������" ) )
	btnCmpAction:SetAction( "sworkWindowManagerProjectToggleComponentLider" )
	
	guienv:AddComboBox( "", 310, 110, 790, 130, WNDPRJMNG_CMBX_USERS, windowg:Self() )
	local button = CLuaButton( guienv:AddButton( 310, 50, 420, 100, windowg:Self(), -1, "������������" ) )
	button:SetAction( "sworkWindowManageProjectsChangerUserType" )
	sworkWindowManageProjectsChangerUserType( button:Self() )	
	button:SetObject( guienv:AddButton( 430, 50, 540, 100, windowg:Self(), -1, "���������" ) )
	button:SetAction( "sworkWindowManageProjectsChangerUserType" )

	button:SetObject( guienv:AddButton( 550, 50, 660, 100, windowg:Self(), -1, "�����������" ) )
	button:SetAction( "sworkWindowManageProjectsChangerUserType" )
	
	button:SetObject( guienv:AddButton( 670, 50, 790, 100, windowg:Self(), -1, "������������" ) )
	button:SetAction( "sworkWindowManageProjectsChangerUserType" )
	
	windowg:AddLuaFunction( GUIELEMENT_CMBXITEM_SELECTED, "sworkWindowProjectManagerComboBoxItemSelected" )
end

function sworkWindowManagerProjectToggleComponentLider( ptr )
	if currentComponent:HaveLider() then
		Log({src=SCRIPT, dev=ODS|CON}, "PROJECT-MANAGER:Remove component from "..currentEmployer:GetName() )
		currentComponent:SetLider( nil )
		if currentComponent:GetEmployerPosibility() < 40 then
			guienv:ShowMessage( currentEmployer:GetName() .. " ����� ������������� ������, ����� ������ ��������� ��� ������" )
		end
	else
		Log({src=SCRIPT, dev=ODS|CON}, "PROJECT-MANAGER:Add component to "..currentEmployer:GetName() )
		currentComponent:SetLider( currentEmployer:Self() )	
		
		if currentComponent:GetWorkPercentDone() > 70 then
			guienv:ShowMessage( "��������� ����� ��������. �������� ��� ������� �������� �������� � ������ ����� ����������" )
		end
	end
end

function sworkWindowManagerProjectComponentSelect( ptr )
	local cmbx = CLuaListBox( ptr )
	currentComponent:SetObject( cmbx:GetSelectedObject() )
end

local function ShowWindowUserInfo( userPtr )
	currentEmployer:SetObject( userPtr )
	local windowg = CLuaWindow( guienv:GetElementByID( WINDOW_PRJMANAGE_USERINFO ) )
	local widdddd, hhhhhhh = windowg:GetSize()
	
	if windowg:Empty() == 1 then
		windowg:SetObject( guienv:AddWindow( "", 310, 140, 790, 590, WINDOW_PRJMANAGE_USERINFO, guienv:GetElementByID(WINDOW_PRJMANAGE_ID) ) )
		windowg:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "sworkWindowManagerProjectComponentSelect" )
	else
		local elm = CLuaElement( windowg:Self() )
		elm:RemoveChilds()
	end
	
	local button = CLuaButton( windowg:GetCloseButton() )
	button:SetVisible( false )
	windowg:SetDraggable( false )

	guienv:AddLabel( "����", 5, 30, widdddd, 30 + 20, -1, windowg:Self() )
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 50, 30, widdddd - 5, 30 + 20, -1 ) )
	prg:SetPosition( currentEmployer:GetParam( "knowledgeLevel" ) )						   
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
	
	guienv:AddLabel( "��������", 5, 50, widdddd, 50 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 55, widdddd - 5, 55 + 20, -1 ) )
	prg:SetPosition( currentEmployer:GetParam("codeQuality") ) 	
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "��������", 5, 70, widdddd, 70 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 80, widdddd - 5, 80 + 20, -1 ) )
	prg:SetPosition( currentEmployer:GetParam("codeSpeed") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "������������", 5, 90, widdddd, 90 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 105, widdddd - 5, 105 + 20, -1 ) )
	prg:SetPosition( currentEmployer:GetParam("stability") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
					   
	local lbx = CLuaListBox( guienv:AddComponentListBox( 10, 135, widdddd - 10, hhhhhhh - 10, -1, windowg:Self() ) )
end

local function ShowUnworkedGameProjectComponent( ptrProject )
	local gp = CLuaGameProject( ptrProject )
	local lbx = CLuaComponentListBox( guienv:GetElementByID( WINDOW_PRJMANAGE_COMPONENTS ) )
	local tech = CLuaTech( nil )
	
	for i=1, gp:GetGenreModuleNumber() do
	    tech:SetObject( gp:GetGenre( i-1 ) )
		if tech:Empty() == 1 or not tech:HaveLider() then 
		  lbx:AddItem( tech:GetName(), tech:Self() )	
		end
	end
end

function sworkWindowProjectManagerComboBoxItemSelected( ptr )
	local cmbx = CLuaComboBox( ptr )
	if cmbx:GetID() == WNDPRJMNG_CMBX_USERS then
		ShowWindowUserInfo( cmbx:GetSelectedObject() ) 	
		return 0
	end
	
	if cmbx:GetID() == WNDPRJMNG_CMBX_SELECT_PRJ then
		currentProject:SetObject( cmbx:GetSelectedObject() )
		if currentProject:GetTechType() == PT_GAME then
			ShowUnworkedGameProjectComponent( currentProject:Self() )
			return 0
		end
		
		if currentProject:GetTechType() == PT_GAMEENGINE then
			ShowUnworkedEngineProjectComponent( currentProject:Self() )
			return 0		
		end
	end
end

local function ShowAvaibleCompanyUsers()
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxuser = company:GetUserNumber()
	
	local cmbx = CLuaComboBox( guienv:GetElementByID( WNDPRJMNG_CMBX_USERS ) )
	
	cmbx:Clear()
	for i=1, maxuser do
		local user = CLuaUser( company:GetUser( i-1 ) )
		if modeUserView == user:GetTypeName() then
			cmbx:AddItem( user:GetName(), user:Self() )
		end
	end
	sworkWindowProjectManagerComboBoxItemSelected( cmbx:Self() )
end

function sworkWindowManageProjectsChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	currentEmployer:SetObject( nil )
	currentComponent:SetObject( nil )
	ShowAvaibleCompanyUsers()
end