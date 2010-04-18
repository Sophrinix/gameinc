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
local WINDOW_EMPMANAGE_USER_INFO = WINDOW_EMPLOYERS_MANAGE_ID + 1

local currentEmployer = CLuaUser( nil )
local listBoxCompanyEmployers = CLuaListBox( nil )

local function ShowAvaibleEmployersToManage()
	local company = CLuaCompany( applic:GetPlayerCompany() )
	local maxuser = company:GetUserNumber()
	
	listBoxCompanyEmployers:Clear()
	for i=1, maxuser do
		local user = CLuaUser( company:GetUser( i-1 ) )
		if modeUserView == user:GetTypeName() then
			listBoxCompanyEmployers:AddItem( user:GetName(), user:Self() )
		end
	end
end

local function ShowUsersParameters( lbx, user )
	lbx:Clear()
	lbx:AddItem( "Опыт=" .. user:GetParam( "knowledgeLevel" ), nil )
	lbx:AddItem( "Качество=" .. user:GetParam( "codeQuality" ), nil )
	lbx:AddItem( "Скорость=" .. user:GetParam( "codeSpeed" ), nil )
	lbx:AddItem( "Устойчивость=" .. user:GetParam( "stability" ), nil )
	lbx:AddItem( "Характер=" .. user:GetParam( "character" ), nil )	
	
	if user:GetSkill( SKL_CODING ) > 0 then 
		lbx:AddItem( "Программирование=" .. user:GetSkill( SKL_CODING ), nil )	
	end
end

function sworkCreateAdvancedUserInfoWindow( ptr )
	local winInfo = CLuaWindow( guienv:GetElementByName( WINDOW_EMPMANAGE_USER_INFO ) )
	local listBox = CLuaListBox( ptr )
	currentEmployer:SetObject( listBox:GetSelectedObject() )

	winInfo:Remove()
	
	local parent = guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID )
	winInfo:SetObject( guienv:AddWindow( "", 210, 110, width - 10, height - 10, WINDOW_EMPMANAGE_USER_INFO, parent ) )
	winInfo:SetText( currentEmployer:GetName() )
	winInfo:SetDraggable( false )
	local btn = CLuaButton( winInfo:GetCloseButton() )
	btn:SetVisible( false )
	
	local btnRemoveUser = CLuaButton( guienv:AddButton( 10, 20, 60, 70, winInfo:Self(), -1, "Увлт" ) )
	btnRemoveUser:SetAction( "sworkRemoveUserFromCompany" )
	
	local btnUpSalary = CLuaButton( guienv:AddButton( 60, 20, 110, 70, winInfo:Self(), -1, "Пдн З/П" ) )
	btnUpSalary:SetAction( "sworkUserUpSalary" )
	
	local btnDownSalary = CLuaButton( guienv:AddButton( 110, 20, 160, 70, winInfo:Self(), -1, "Умн З/П" ) )
	btnDownSalary:SetAction( "sworkUserDownSalary" )
	
	local btnGetWeekEnd = CLuaButton( guienv:AddButton( 160, 20, 210, 70, winInfo:Self(), -1, "Вхдн" ) )
	btnGetWeekEnd:SetAction( "sworkUserGetWeekend" )
	
	local btnGetPremia = CLuaButton( guienv:AddButton( 210, 20, 260, 70, winInfo:Self(), -1, "Премия" ) )
	btnGetPremia:SetAction( "sworkUserGetPremia" )
	
	local btnSaveData = CLuaButton( guienv:AddButton( 210, 20, 260, 70, winInfo:Self(), -1, "Save" ) )
	btnSaveData:SetAction( "sworkManageEmployersUserSaveData" )
	
	local wd, ht = winInfo:GetSize()
	local listBox = CLuaListBox( guienv:AddListBox( 160, 80, wd - 10, ht - 10, -1, winInfo:Self() ) )
	
	ShowUsersParameters( listBox, currentEmployer )
end

function sworkManageEmployersUserSaveData( ptr )
	--currentEmployer:Save( "save/dalerank/companies/daleteam/users/" )
	currentEmployer:Save( "save/dalerank/companies/daleteam/users/Villi Salo" )
end

function sworkCreateWindowEmployersManage( ptr )
	local windowg = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID ) )
	
	if windowg:Empty() == 1 then
		windowg:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_EMPLOYERS_MANAGE_ID, guienv:GetRootGUIElement() ) )
	else
		local elm = CLuaElement( windowg:Self() )
		elm:RemoveChilds()
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

function sworkWindowManageEmployersChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	currentEmployer:SetObject( nil )
	sworkCreateWindowEmployersManage()
end