local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеры" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"

local width, height = driver:GetScreenSize()

local currentEmployer = CLuaUser( nil )
local listBoxCompanyEmployers = nil
local winInfo = nil
local btnRemoveUser = CLuaButton( nil )
local btnUpSalary = CLuaButton( nil )
local btnDownSalary = CLuaButton( nil )
local btnGiveWeekEnd = CLuaButton( nil )
local btnGivePremia = CLuaButton( nil )

local function ShowAvaibleEmployersToManage()
	local company = applic:GetPlayerCompany()
	local maxuser = company:GetUserNumber()
	
	listBoxCompanyEmployers:Clear()
	for i=1, maxuser do
		local user = company:GetUser( i-1 )
		
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

function sworkCloseAdvancedUserInfoWindow( ptr )
	Log({src=SCRIPT, dev=ODS}, "Window now will remove" )
	winInfo:Remove()
end

function sworkCreateAdvancedUserInfoWindow( ptr )
	if winInfo ~= nil then winInfo:Remove() end

	currentEmployer:SetObject( listBoxCompanyEmployers:GetSelectedObject() )
	
	local parent = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID ) )
	winInfo = guienv:AddWindow( "", width / 2 - 300, height / 2 - 200, width / 2 + 300, height / 2 + 200, -1, parent:Self() )
	winInfo:SetText( currentEmployer:GetName() )
	winInfo:SetDraggable( false )
	
	--сделаем невидимой кнопку закрыть, чтобы закрывать окно по правой кнопке мыши
	local btn = winInfo:GetCloseButton()
	btn:SetVisible( false )
	
	winInfo:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkCloseAdvancedUserInfoWindow" )
	
	local wd, ht = winInfo:GetSize()
	local listBox = CLuaListBox( guienv:AddListBox( 10, 20, wd - 10, ht - 10, -1, winInfo:Self() ) )
	
	ShowUsersParameters( listBox, currentEmployer )
end

function sworkCreateWindowEmployersManage( ptr )
	local windowg = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID ) )
	
	if windowg:Empty() == 1 then
		windowg = guienv:AddWindow( "", 0, 0, width, height, WINDOW_EMPLOYERS_MANAGE_ID, guienv:GetRootGUIElement() )
	else
		local elm = CLuaElement( windowg:Self() )
		elm:RemoveChilds()
	end
	
	local button = guienv:AddButton( 10, 10, 200, 100, windowg:Self(), -1, "Программисты" )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	button = guienv:AddButton( 210, 10, 400, 100, windowg:Self(), -1, "Дизайнеры" )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )

	button = guienv:AddButton( 410, 10, 600, 100, windowg:Self(), -1, "Композиторы" )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	button = guienv:AddButton( 610, 10, 800, 100, windowg:Self(), -1, "Тестировщики" )
	button:SetAction( "sworkWindowManageEmployersChangerUserType" )
	
	listBoxCompanyEmployers = guienv:AddComponentListBox( 10, 110, width - 10, height - 80, -1, windowg:Self() )
	listBoxCompanyEmployers:SetItemHeigth( 128 );
	windowg:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "sworkCreateAdvancedUserInfoWindow" )
	
	ShowAvaibleEmployersToManage()
	
	local i = 0
	btnRemoveUser = guienv:AddButton( 100 * i, height - 70, 100 * (i + 1), height - 10, windowg:Self(), -1, "Уволить" )
	btnRemoveUser:SetAction( "sworkRemoveUserFromCompany" )
	
	i = i + 1
	btnUpSalary = guienv:AddButton(  100 * i, height - 70,  100 * (i + 1), height - 10, windowg:Self(), -1, "З/П + 5%" )
	btnUpSalary:SetAction( "sworkUserUpSalary" )
	
	i = i +	1
	btnDownSalary = guienv:AddButton( 100 * i, height - 70, 100 * (i + 1), height - 10, windowg:Self(), -1, "З/П - 5%" )
	btnDownSalary:SetAction( "sworkUserDownSalary" )
	
	i = i + 1
	btnGiveWeekEnd = guienv:AddButton(  100 * i, height - 70,  100 * (i + 1), height - 10, windowg:Self(), -1, "Отпуск" )
	btnGiveWeekEnd:SetAction( "sworkUserGetWeekend" )
	
	i = i +  1
	btnGivePremia = guienv:AddButton(  100 * i, height - 70,  100 * (i + 1), height - 10, windowg:Self(), -1, "Премия" )
	btnGivePremia:SetAction( "sworkUserGetPremia" )
end

function sworkWindowManageEmployersChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	currentEmployer = nil
	
	ShowAvaibleEmployersToManage()
end