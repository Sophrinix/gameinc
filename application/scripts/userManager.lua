local base = _G

IncludeScript( "moneySelect" )
IncludeScript( "userAward" )
IncludeScript( "userLearning" )
IncludeScript( "userRecognition" )

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

currentEmployer = nil

local listBoxCompanyEmployers = nil
local windowUserManager = nil
local btnRemoveUser = nil
local btnUpSalary = nil
local btnDownSalary = nil
local btnGiveWeekEnd = nil
local btnGivePremia = nil
local messageBox = nil

local function _ShowAvaibleEmployersToManage()
	listBoxCompanyEmployers:Clear()
	listBoxCompanyEmployers.itemHeight = 60

	for i=1, company.userNumber do
		local user = company:GetUser( i-1 )
		
		if modeUserView == user.typeName then
			listBoxCompanyEmployers:AddItem( user.name, user.object )

			if i % 2 == 0 then
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0x82, 0x82, 0x82 )
			else
				listBoxCompanyEmployers:SetItemBgColor( i-1, 0xff, 0xA4, 0xA4, 0xA4 )
			end
		end
	end
end

function CloseUserInfoWindow( ptr )
	winInfo:Remove()
end

local function _SetNewSalary( start, stop )
	if currentEmployer ~= nil then
		currentEmployer:SetParam( "salary", stop )
		
		base.LogScript( "!!!!!!!!!!!!!! Salary to "..currentEmployer.name.." is $"..stop )
		--currentEmployer:AddModificator( "effectively", 1.2, "7 day" )
	end
end

local function _ChangeSalary()
	if currentEmployer ~= nil then
		local curSalary = currentEmployer:GetParam( "salary" )
		local interval = base.math.ceil( curSalary / 500 ) * 50
		base.moneySelect.Show( "Установить новую зарплату", curSalary, interval, _SetNewSalary )
	end
end

--нормально выплатили выходное пособие работнику при увольнении
local function _GetMoneyForFireEmp()
	--работник остается на рынке труда
	company:RemoveUser( currentEmployer.name )
	
	--но теряет уважение к компании игрока, и может не пойти 
	--в неё снова
	-- получим текущее значение отношения рабочего к компании
	local relation = currentEmployer:GetRelation( company.name ) 
	relation.value = relation.value * 0.8
	--также это вызовет некоторое снижение отношения к компании
	--у его друзей, чем выше показатель отношения между людьми
	--тем больше зависимость падения отношения к компании у друга

	-- больше к этому человеку нет доступа
	currentEmployer = nil
	messageBox:Remove()
	_ShowAvaibleEmployersToManage()
end

--не захотели выплатить выходное пособие работнику
local function _NoGetMoneyForFireEmp()
	company:RemoveUser( currentEmployer.name )
	
	local relation = currentEmployer:GetRelation( company.name ) 
	relation.value = relation.value * 0.3
	--расчет возможности что уволенный человек забрал
	--какие-то данные или испортил разрабатываемый модуль,
	--передал разработку в другую компанию, чем увеличил прогресс 
	--какого нибудть модуля
	--если это произошло, то его надо убрать с рынка
	applic:RemoveUser( currentEmployer )
	currentEmployer = nil
	messageBox:Remove()
	_ShowAvaibleEmployersToManage()
end

local function _RemoveUser()
	--увольняя человека надо выплатить ему зарплату за два месяца
	--иначе повышается шанс, что он чтонибудь стащит с конторы
	messageBox = guienv:MessageBox( "Работник хочет получить жалование \nза 2 месяца вперед ($" .. currentEmployer:GetParam( "salary" ) * 2 .. ")\nВыплатить???", 
								    true, true, _GetMoneyForFireEmp, _NoGetMoneyForFireEmp )
end

local function _ChangeUserType( name )
	modeUserView = mode[ name ] 
	currentEmployer = nil
	
	_ShowAvaibleEmployersToManage()
end

local function _GetWeekend()
	if currentEmployer ~= nil then
		currentEmployer:AddTimeWork( 7, "Отпуск", 14, "codeQuality", base.math.random( 0, 10 ) )
		local relation = currentEmployer:GetRelation( company.name ) 
		relation.value = relation.value * 1.01
		
		base.pda.Show( currentEmployer.name.." будет в отпуске 7 дней" )
	end
end

local function _Routine()

end

local function _Hide()
	base.package.loaded[ "moneySelect" ] = false
	base.package.loaded[ "userAward" ] = false
	base.package.loaded[ "userLearning" ] = false
	base.packege.loaded[ "userRecognition" ] = false
end

function Show()
	company = applic.playerCompany
	
	windowUserManager = window.fsWindow( "media/maps/director_cabinet_slider.png", _Hide )
	
	--Coder's button
	local layout = guienv:AddLayout( "25%", "5%", "50%+", "15%", 10, -1, windowUserManager )
	button.LayoutButton( "buttonCoders", layout, -1, base.STR_CODERS, function () _ChangeUserType( base.STR_CODERS ) end )
	button.LayoutButton( "buttonDesigners", layout, -1, base.STR_DESIGNERS, function () _ChangeUserType( base.STR_DESIGNERS ) end )
	button.LayoutButton( "buttonComposers", layout, -1, base.STR_COMPOSERS, function () _ChangeUserType( base.STR_COMPOSERS ) end )
	button.LayoutButton( "buttonTesters", layout, -1, base.STR_TESTERS, function () _ChangeUserType( base.STR_TESTERS ) end )
	
	listBoxCompanyEmployers = guienv:AddComponentListBox( "5%", "18%", "95%", "80e", -1, windowUserManager )
	listBoxCompanyEmployers.itemHeigth = 128
	listBoxCompanyEmployers.onLmbDblClick = _ShowUserInfo
	listBoxCompanyEmployers.onChangeSelect = function () 
													currentEmployer = base.CLuaUser( listBoxCompanyEmployers.selectedObject ) 
											 end
	
	_ShowAvaibleEmployersToManage()

	layout = guienv:AddLayout( "25%", "70e", "50%+", "5e", 10, -1, windowUserManager )
	button.LayoutButton( "fire", layout, -1, base.STR_FIRE_EMP, _RemoveUser )
	button.LayoutButton( "moneyup", layout, -1, base.STR_INC_SALARY, _ChangeSalary )
	--button.LayoutButton( "moneydown", layout, -1, base.STR_DEC_SALARY, _DownSalary )
	button.LayoutButton( "weekend", layout, -1, base.STR_GET_WEEKEND, _GetWeekend )
	button.LayoutButton( "premia", layout, -1, base.STR_GET_PRESENT, base.userAward.Show )
	button.LayoutButton( "learning", layout, -1, base.STR_SEND_TO_SCHOOL, base.userLearning.Show )	
	button.LayoutButton( "medal", layout, -1, base.STR_RECOGNITION, base.userRecognition.Show )	
	--button.LayoutButton( "task", layout, -1, base.STR_ROUTINE, _Routine )	
end