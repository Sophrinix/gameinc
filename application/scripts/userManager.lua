local base = _G

module( "userManager" ) 

local applic = base.applic
local guienv = base.guienv
local company = nil
local button = base.button
local window = base.window
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

local function ShowAvaibleEmployersToManage()
	local maxuser = company.userNumber
	
	listBoxCompanyEmployers:Clear()
	for i=1, maxuser do
		local user = company:GetUser( i-1 )
		
		if modeUserView == user.typeName then
			listBoxCompanyEmployers:AddItem( user.name, user.object )
			listBoxCompanyEmployers:SetItemTextColor( i-1, 0xff, 0x0, 0xff, 0x0 )
			if i % 2 == 0 then
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0x82, 0x82, 0x82 )
			else
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0xA4, 0xA4, 0xA4 )
			end
		end
	end
end

local function ShowUsersParameters( lbx, user )
	lbx:Clear()
	lbx:AddItem( "ќпыт=" .. user:GetParam( "knowledgeLevel" ), nil )
	lbx:AddItem( " ачество=" .. user:GetParam( "codeQuality" ), nil )
	lbx:AddItem( "—корость=" .. user:GetParam( "codeSpeed" ), nil )
	lbx:AddItem( "”стойчивость=" .. user:GetParam( "stability" ), nil )
	lbx:AddItem( "’арактер=" .. user:GetParam( "character" ), nil )	
	
	if user:GetSkill( base.SKILL_CODING ) > 0 then 
		lbx:AddItem( "ѕрограммирование=" .. user:GetSkill( base.SKILL_CODING ), nil )	
	end
end

function CloseUserInfoWindow( ptr )
	winInfo:Remove()
end

function ShowUserInfo()	
	if winInfo ~= nil then winInfo:Remove() end

	currentEmployer = base.CLuaUser( listBoxCompanyEmployers:GetSelectedObject() )
	
	winInfo = guienv:AddWindow( "", "25%", "33%", "50%+", "50%+", -1, windowUserManager )
	winInfo.text = currentEmployer.name
	winInfo.draggable = false 

	winInfo:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, CloseUserInfoWindow )
	
	local wd, ht = winInfo:GetSize()
	local listBox = guienv:AddListBox( 10, 20, "10e", "10e", -1, winInfo )
	
	ShowUsersParameters( listBox, currentEmployer )
end

local function _UpSalary()
	if currentEmployer ~= nil then
		local curSalary = currentEmployer:GetParam( "salary" )
		currentEmployer:SetParam( "salary", curSalary * 1.1 )
		LogScript( "!!!!!!!!!!!!!! Salary to "..currentEmployer.name.." is $"..curSalary )
		
		--currentEmployer:AddModificator( "effectively", 1.2, "7 day" ) 
	end
end

local function _GetMoneyForFireEmp()
	--работник остаетс€ на рынке труда
	company:RemoveUser( currentEmployer.name )
	
	--но тер€ет уважение к компании игрока, и может не пойти 
	--в неЄ снова
	-- получим текущее значение отношени€ рабочего к компании
	local relation = currentEmployer:GetRelation( company.name ) 
	relation:Set( "rel_value", relation:GetValue( "rel_value" ) * 0.8 ) 
	--также это вызовет некоторое снижение отношени€ к компании
	--у его друзей, чем выше показатель отношени€ между людьми
	--тем больше зависимость падени€ отношени€ к компании у друга

	-- больше к этому человеку нет доступа
	currentEmployer = nil
	ShowAvaibleEmployersToManage()
end

local function _NoGetMoneyForFireEmp()
	company:RemoveUser( currentEmployer.name )
	
	local relation = currentEmployer:GetRelation( company.name ) 
	relation:Set( "rel_value", realtion:GetValue( "rel_value" ) * 0.3 ) 
	--расчет возможности что уволенный человек забрал
	--какие-то данные или испортил разрабатываемый модуль,
	--передал разработку в другую компанию, чем увеличил прогресс 
	--какого нибудть модул€
	--если это произошло, то его надо убрать с рынка
	application:RemoveUser( currentEmployer )
	currentEmployer = nil
	ShowAvaibleEmployersToManage()
end

local function _RemoveUser()
	--увольн€€ человека надо выплатить ему зарплату за два мес€ца
	--иначе повышаетс€ шанс, что он чтонибудь стащит с конторы
	guienv:MessageBox( "–аботник хочет получить жалование за 2 мес€ца вперед ($" .. currentEmployer:GetParam( "salary" ) * 2 .. ")\n¬ыплатить???", 
					   true, true, _GetMoneyForFireEmp, _NoGetMoneyForFireEmp )
end

local function _ChangeUserType( name )
	modeUserView = mode[ name ] 
	currentEmployer = nil
	
	ShowAvaibleEmployersToManage()
end

local function _DownSalary()

end

local function _GetWeekend()

end

local function _GetPremia()

end

local function _SendToLearning()

end

local function _Communicate()

end

local function _Routine()

end

function Show()
	company = applic.playerCompany
	
	windowUserManager = window.fsWindow( "media/maps/director_cabinet_slider.png", Hide )
	
	--Coder's button
	local layout = guienv:AddLayout( "25%", "5%", "50%+", "15%", 10, -1, windowUserManager )
	button.LayoutButton( "buttonCoders", layout, -1, base.STR_CODERS, function () _ChangeUserType( STR_CODERS ) end )
	button.LayoutButton( "buttonDesigners", layout, -1, base.STR_DESIGNERS, function () _ChangeUserType( STR_DESIGNERS ) end )
	button.LayoutButton( "buttonComposers", layout, -1, base.STR_COMPOSERS, function () _ChangeUserType( STR_COMPOSERS ) end )
	button.LayoutButton( "buttonTesters", layout, -1, base.STR_TESTERS, function () _ChangeUserType( STR_TESTERS ) end )
	
	listBoxCompanyEmployers = guienv:AddComponentListBox( "5%", "18%", "95%", "80e", -1, windowUserManager )
	listBoxCompanyEmployers.itemHeigth = 128
	listBoxCompanyEmployers.onLmbDblClick = ShowUserInfo
	
	ShowAvaibleEmployersToManage()

	layout = guienv:AddLayout( "25%", "70e", "50%+", "5e", 10, -1, windowUserManager )
	button.LayoutButton( "fire", layout, -1, base.STR_FIRE_EMP, _RemoveUser )
	button.LayoutButton( "moneyup", layout, -1, base.STR_INC_SALARY, _UpSalary )
	button.LayoutButton( "moneydown", layout, -1, base.STR_DEC_SALARY, _DownSalary )
	button.LayoutButton( "weekend", layout, -1, base.STR_GET_WEEKEND, _GetWeekend )
	button.LayoutButton( "premia", layout, -1, base.STR_GET_PRESENT, _GetPremia )
	button.LayoutButton( "learning", layout, -1, base.STR_SEND_TO_SCHOOL, _SendToLearning )	
	button.LayoutButton( "talk", layout, -1, base.STR_COMMUNICATE, _Communicate )	
	button.LayoutButton( "task", layout, -1, base.STR_ROUTINE, _Routine )	
end