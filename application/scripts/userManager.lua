local base = _G

module( "userManager" ) 

local applic = base.applic
local guienv = base.guienv
local company = nil
local button = base.button

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

local scrWidth = 0
local scrHeight = 0

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
	
	if user:GetSkill( base.SKL_CODING ) > 0 then 
		lbx:AddItem( "Программирование=" .. user:GetSkill( base.SKL_CODING ), nil )	
	end
end

function CloseUserInfoWindow( ptr )
	Log({src=base.SCRIPT, dev=base.ODS}, "Window now will remove" )
	winInfo:Remove()
end

function ShowUserInfo()	
	if winInfo ~= nil then winInfo:Remove() end

	currentEmployer = base.CLuaUser( listBoxCompanyEmployers:GetSelectedObject() )
	
	local parent = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYERS_MANAGE_ID ) )
	winInfo = guienv:AddWindow( "", width / 2 - 300, height / 2 - 200, width / 2 + 300, height / 2 + 200, -1, parent:Self() )
	winInfo:SetText( currentEmployer:GetName() )
	winInfo:SetDraggable( false )
	
	--сделаем невидимой кнопку закрыть, чтобы закрывать окно по правой кнопке мыши
	local btn = winInfo:GetCloseButton()
	btn:SetVisible( false )
	
	winInfo:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, "./userManager.CloseUserInfoWindow()" )
	
	local wd, ht = winInfo:GetSize()
	local listBox = guienv:AddListBox( 10, 20, wd - 10, ht - 10, -1, winInfo:Self() )
	
	ShowUsersParameters( listBox, currentEmployer )
end

function Hide()
	windowUserManager:Remove()
	windowUserManager = nil
end

function Show()
	scrWidth, scrHeight = base.scrWidth, base.scrHeight
	company = applic:GetPlayerCompany()
	
	if windowUserManager == nil or windowUserManager:Empty() == 1 then
		windowUserManager = guienv:AddWindow( "media/director_cabinet_slider.tga", 0, 0, 
											  scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		windowUserManager:GetCloseButton():SetVisible( false )
		windowUserManager:SetDraggable( false ) 
		
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", windowUserManager:Self(), -1, "",
						"./userManager.Hide()" )
	else
		base.CLuaElement( windowUserManager:Self() ):RemoveChilds()
	end
	
	--Coder's button
	button.EqualeTexture( 20, 40, "buttonCoders", windowUserManager:Self(), -1, base.STR_CODERS, "./userManager.ChangeUserType( STR_CODERS )" )
	button.EqualeTexture( scrWidth * 0.25, 40, "buttonDesigners", windowUserManager:Self(), -1, base.STR_DESIGNERS, "./userManager.ChangeUserType( STR_DESIGNERS )" )
	button.EqualeTexture( scrWidth * 0.5, 40, "buttonComposers", windowUserManager:Self(), -1, base.STR_COMPOSERS, "./userManager.ChangeUserType( STR_COMPOSERS )" )
	button.EqualeTexture( scrWidth * 0.75, 40, "buttonTesters", windowUserManager:Self(), -1, base.STR_TESTERS, "./userManager.ChangeUserType( STR_TESTERS )" )
	
	listBoxCompanyEmployers = guienv:AddComponentListBox( 20, 160, scrWidth - 20, scrHeight - 80, -1, windowUserManager:Self() )
	listBoxCompanyEmployers:SetItemHeigth( 128 );
	windowUserManager:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./userManager.ShowUserInfo()" )
	
	ShowAvaibleEmployersToManage()
	
	local i = 0
	btnRemoveUser = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight  - 10, windowUserManager:Self(), -1, "Уволить" )
	btnRemoveUser:SetAction( "./userManager.RemoveUser()" )
	
	i = i + 1
	btnUpSalary = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, "З/П + 5%" )
	btnUpSalary:SetAction( "./userManager.UpSalary()" )
	
	i = i +	1
	btnDownSalary = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, "З/П - 5%" )
	btnDownSalary:SetAction( "./userManager.DownSalary()" )
	
	i = i + 1
	btnGiveWeekEnd = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, "Отпуск" )
	btnGiveWeekEnd:SetAction( "./userManager.GetWeekend()" )
	
	i = i +  1
	btnGivePremia = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight - 10, windowUserManager
	:Self(), -1, "Премия" )
	btnGivePremia:SetAction( "./userManager.GetPremia()" )
end

function ChangeUserType( name )
	modeUserView = mode[ name ] 
	currentEmployer = nil
	
	ShowAvaibleEmployersToManage()
end