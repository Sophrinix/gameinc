local base = _G

module( "userManager" ) 

local applic = base.applic
local guienv = base.guienv
local company = nil
local button = base.button
local Log = base.Log

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
			listBoxCompanyEmployers:SetItemTextColor( i-1, 0xff, 0x0, 0xff, 0x0 )
			if i % 2 == 0 then
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0x42, 0x42, 0x42 )
			else
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0x24, 0x24, 0x24 )
			end
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
	
	winInfo = guienv:AddWindow( "", scrWidth / 2 - 300, scrHeight / 2 - 200, scrWidth / 2 + 300, scrHeight / 2 + 200, -1, windowUserManager:Self() )
	winInfo:SetText( currentEmployer:GetName() )
	winInfo:SetDraggable( false )

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
		windowUserManager = guienv:AddWindow( "media/maps/director_cabinet_slider.png", 0, 0, 
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
	btnRemoveUser = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight  - 10, windowUserManager:Self(), -1, base.STR_FIRE_EMP )
	btnRemoveUser:SetAction( "./userManager.RemoveUser()" )
	
	i = i + 1
	btnUpSalary = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_INC_SALARY )
	btnUpSalary:SetAction( "./userManager.UpSalary()" )
	
	i = i +	1
	btnDownSalary = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_DEC_SALARY )
	btnDownSalary:SetAction( "./userManager.DownSalary()" )
	
	i = i + 1
	btnGiveWeekEnd = guienv:AddButton( 100 * i, scrHeight  - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_GET_WEEKEND )
	btnGiveWeekEnd:SetAction( "./userManager.GetWeekend()" )
	
	i = i +  1
	btnGivePremia = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_GET_PRESENT )
	btnGivePremia:SetAction( "./userManager.GetPremia()" )
	
	i = i +  1
	btnGivePremia = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_SEND_TO_SCHOOL )
	btnGivePremia:SetAction( "./userManager.SendToLearning()" )	
	
	i = i +  1
	btnGivePremia = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_COMMUNICATE )
	btnGivePremia:SetAction( "./userManager.Communicate()" )	
	
	i = i +  1
	btnGivePremia = guienv:AddButton( 100 * i, scrHeight - 70, 100 * (i + 1), scrHeight - 10, windowUserManager:Self(), -1, base.STR_ROUTINE )
	btnGivePremia:SetAction( "./userManager.Rooutine()" )	
end

function GetMoneyForFireEmp()
	--работник остается на рынке труда
	company:RemoveUser( currentEmployer:GetName() )
	
	--но теряет уважение к компании игрока, и может не пойти 
	--в неё снова
	local relation = currentEmployer:GetRelation( company:GetName() ) 
	
	currentEmployer:SetRelation( company:GetName(), relation * 0.5 ) 
	--также это вызовет некоторое снижение отношения к компании
	--у его друзей, чем выше показатель отношения между людьми
	--тем больше зависимость падения отношения к компании
	currentEmployer = nil
end

function NoGetMoneyForFireEmp()

	company:RemoveUser( currentEmployer:GetName() )
	--расчет возможности что уволенный человек забрал
	--какие-то данные или испортил разрабатываемый модуль,
	--передал разработку в другую компанию, чем увеличил прогресс 
	--какого нибудть модуля
	--если это произошло, то его надо убрать с рынка
	application:RemoveUser( currentEmployer:Self() )
	currentEmployer = nil
end

function RemoveUser()
	--увольняя человека надо выплатить ему зарплату за два месяца
	--иначе повышается шанс, что он чтонибудь стащит с конторы
	guienv:MessageBox( "Работник хочет получить жалование за 2 месяца вперед ($" .. currentEmployer:GetParam( "salary" ) * 2 .. ")\nВыплатить???", 
					   true, true, "./userManager.GetMoneyForFireEmp()", "./userManager.NoGetMoneyForFireEmp()" )
end

function ChangeUserType( name )
	modeUserView = mode[ name ] 
	currentEmployer = nil
	
	ShowAvaibleEmployersToManage()
end