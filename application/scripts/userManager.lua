local base = _G

module( "userManager" ) 

local applic = base.applic
local guienv = base.guienv
local company = nil

local mode = { }
mode[ base.STR_CODERS ] = "coder"
mode[ base.STR_DESIGNERS ] = "designer"
mode[ base.STR_COMPOSERS ] = "composer"
mode[ base.STR_TESTERS ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"

local currentEmployer = nil
local listBoxCompanyEmployers = nil
local winInfo = nil
local windowUserManager = nil
local btnRemoveUser = nil
local btnUpSalary = nil
local btnDownSalary = nil
local btnGiveWeekEnd = nil
local btnGivePremia = nil

local function ShowAvaibleEmployersToManage()
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

	currentEmployer = CLuaUser( listBoxCompanyEmployers:GetSelectedObject() )
	
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

function Hide()
	windowUserManager:Remove()
	windowUserManager = nil
end

function Show()
	company = applic:GetPlayerCompany()
	
	if windowUserManager == nil or windowUserManager:Empty() == 1 then
		windowUserManager = guienv:AddWindow( "media/director_cabinet_slider.tga", 0, 0, 
											  base.scrWidth, base.scrHeight, -1, guienv:GetRootGUIElement() )
		windowUserManager:GetCloseButton():SetVisible( false )
		windowUserManager:SetDraggable( false ) 
		
		button.Stretch( base.scrWidth - 80, base.scrHeight - 80, base.scrWidth, base.scrHeight, 
		 			    "button_down", windowUserManager:Self(), -1, "",
						"./userManager:Hide()" )
	else
		CLuaElement( windowUserManager:Self() ):RemoveChilds()
	end
	
	--Coder's button
	button.EqualeTexture( 20, 40, "buttonCoders", windowg:Self(), -1, base.STR_CODERS, "./userManager.ChangerUserType( STR_CODERS )" )
	button.EqualeTexture( base.scrWidth * 0.25, 40, "buttonDesigners", windowg:Self(), -1, base.STR_DESIGNERS, "./userManager.ChangerUserType( STR_DESIGNERS )" )
	button.EqualeTexture( base.scrWidth * 0.5, 40, "buttonComposers", windowg:Self(), -1, base.STR_COMPOSERS, "./userManager.ChangerUserType( STR_COMPOSERS )" )
	button.EqualeTexture( base.scrWidth * 0.75, 40, "buttonTesters", windowg:Self(), -1, base.STR_TESTERS, "./userManager.ChangerUserType( STR_TESTERS )" )
	
	listBoxCompanyEmployers = guienv:AddComponentListBox( 20, 120, base.scrWidth - 20, base.scrHeight - 80, -1, windowUserManager:Self() )
	listBoxCompanyEmployers:SetItemHeigth( 128 );
	windowUserManager:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "./userManager.ShowUserInfo()" )
	
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