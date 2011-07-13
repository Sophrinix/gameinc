local base = _G

module( "inventionManager" )

local windowIM = nil
local labelInvestiment = nil
local labelInventionSpeed = nil
local labelInventionPrognoseFinish = nil
local labelInventionMoneyPassed = nil 
local labelInventionDayLeft = nil
local listInventionStuff = nil
local labelInventionName = nil
local labelInventionMonthMoney = nil
local currentInvention = nil
local layoutActions = nil

local selectedUser = nil
local window_UserSelect = nil
local company = nil 
local picFlowInvention = nil
local lbxUsers = nil

local applic = base.applic
local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local function _AddLabel( textr, x1, y1, x2, y2 )
	local lb = guienv:AddLabel( textr, x1, y1, x2, y2, -1, windowIM )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )		
	lb.color = base.NrpARGB( 0xff, 0xff, 0xff, 0xff )
	return lb
end

--обновлеяет текст на метках
local function _UpdateLabels()
    if currentInvention ~= nil and currentInvention.valid then 
		labelInvestiment.text = currentInvention.investiment
		labelInventionSpeed.text = currentInvention.speed
		local day, month, year = currentInvention:GetPrognoseDateFinish()
		
		labelInventionPrognoseFinish.text = day.."."..month.."."..year
		labelInventionMoneyPassed.text = currentInvention.passedMoney
		labelInventionDayLeft.text = currentInvention.dayLeft
		labelInventionName.text = currentInvention.name
		labelInventionMonthMoney.text = currentInvention.monthPay
	else
	    currentInvention = nil
	end
end

--увеличивает затраты на исследования
local function _IncreaseInvestiment()
	currentInvention.investiment = currentInvention.investiment * 2
	currentInvention:CheckParams()
end

--уменьшает затраты на исследования
local function _DecreaseInvestiment()
	currentInvention.investiment = currentInvention.investiment / 2
	currentInvention:CheckParams()
end

local function _FillListInvnentionStuff()
	if currentInvention == nil then
		return
	end
	
	listInventionStuff:Clear()
	
	for i=1, currentInvention.userNumber do
		local user = currentInvention:GetUser( i-1 )
		local dd = listInventionStuff:AddItem( "Unknown", user.object )
		listInventionStuff:SetItemTextColor( dd, 0xff, 0xff, 0xff, 0xff )
	end
end

local function _InventionSelect() 		
	currentInvention = base.CLuaInvention( picFlowInvention.selectedObject ) 
end

local function _UserSelect()	
	selectedUser = base.CLuaUser( lbxUsers.selectedObject )
end

local function _ShowUserAvaibleForInvention( listbox )
	listbox:Clear()
	company = applic.playerCompany
	
	for i=1, company.userNumber do
		local user = company:GetUser( i-1 )
		
		if not user.haveInvention then
			local dd = listbox:AddItem( "Unknown", user.object )
			listbox:SetItemTextColor( dd, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

local function _AddSelectedUserToInvention()
	currentInvention:AddUser( selectedUser )
	
	guienv:MessageBox( base.string.format( "%s будет участвовать в проекте %s исследовательского отдела", 
											selectedUser.name, currentInvention.name ), true, false,
											nil, nil )
											
	_FillListInvnentionStuff()
	_ShowUserAvaibleForInvention( lbxUsers )
end

local function _CloseWindowUserAdding()
	window_UserSelect:Remove()
	window_UserSelect = nil
end

local function _AddPeopleToInvention()
	--создаем окно в центре экрана
	window_UserSelect = guienv:AddWindow( "", 200, 200, "200e", "200e", -1, guienv.root )
													  
	local wd, hd = window_UserSelect:GetSize()												  
	lbxUsers = guienv:AddComponentListBox( 10, 10, wd - 10, hd - 40, -1, window_UserSelect )
	lbxUsers.itemHeigth = 80
		
	_ShowUserAvaibleForInvention( lbxUsers )
	
	local btn = guienv:AddButton( 10, hd - 35, wd / 2 - 10, hd - 5, window_UserSelect, -1, "Select")
	btn.action = _AddSelectedUserToInvention
		
	btn = guienv:AddButton( wd / 2 + 10, hd - 35, wd - 10, hd - 5, window_UserSelect, -1, "Close")
	btn.action = _CloseWindowUserAdding
	
	--обработчик выбора в списке
	window_UserSelect.onChangeSelect = _UserSelect
end

local function _ShowInfoAboutInvention()

end

local function _RemPeopleFromInvention()

end

--отображает окно управления исследованиями
function Show( techName, companyName )
	company = applic:GetCompanyByName( companyName )
	
	base.LogScript( "Окрыто окно для исследования="..techName.. "  компания="..company.name )
	local txsBlur = base.driver:CreateBlur( "laboratory.png", 2, 4 )
	windowIM = guienv:fsWindow( txsBlur.path, _Hide )
	
	picFlowInvention = guienv:AddPictureFlow( 60, 10, "10e", "66%", -1, windowIM )
	picFlowInvention:SetPictureRect( 0, 0, "33%", "33%" )
	picFlowInvention.drawBody = false
	
	for index=1, company.inventionNumber do
		local invention = company:GetInvention( index-1 )
		picFlowInvention:AddItem( invention.texture, invention.name, invention.object )
			
		if techName == invention.uniq then
			currentInvention = invention
			picFlowInvention.selected = index-1
		end
	end
	
	local stdSize = "30+"
	local lbWidth = "50%"
	local lbLeft = 45
	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local lb = _AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT", lbLeft, "36%", lbWidth, stdSize )
	labelInvestiment = _AddLabel( "", lbLeft, lb.bottom + 10, lbWidth, stdSize )													

	local btnPlus = guienv:AddButton( labelInvestiment.left, labelInvestiment.top, 
									  stdSize, labelInvestiment.bottom, windowIM, -1, "+")
	btnPlus.action = _IncreaseInvestiment
	
	local btnMinus = guienv:AddButton( labelInvestiment.left - 30, labelInvestiment.top, 
									   stdSize, labelInvestiment.bottom, windowIM, -1, "-" )
	btnMinus.action = _DecreaseInvestiment
	--метка с отображением скорости исследований
	labelInventionSpeed = _AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED", lbLeft, labelInvestiment.bottom+10, lbWidth, stdSize )
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionPrognoseFinish = _AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH", lbLeft, labelInventionSpeed.bottom + 10, lbWidth, stdSize )
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionDayLeft = _AddLabel( "#TRANSLATE_TEXT_INVENTIONDAYLEFT", lbLeft, labelInventionPrognoseFinish.bottom + 10, lbWidth, stdSize )													    												   
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionMoneyPassed = _AddLabel( "#TRANSLATE_TEXT_INVENTIONMONEYPASSED", lbLeft, labelInventionDayLeft.bottom + 10, lbWidth, stdSize )										      
	--метка месячным финансированием
	labelInventionMonthMoney = _AddLabel( "#TRANSLATE_TEXT_MONTHPAY", lbLeft, labelInventionMoneyPassed.bottom + 10, lbWidth, stdSize )

	labelInventionName = _AddLabel( "", "25%", "33%", "25%+", "40+" )
	
	--список подключенных к проекту людей 
	listInventionStuff = guienv:AddComponentListBox( "55%", labelInvestiment.top, "10e", "45e", -1, windowIM )
	listInventionStuff.itemHeigth = 80
	_FillListInvnentionStuff()
	
	
	layoutActions = guienv:AddLayout( labelInvestiment.left, labelInvestiment.bottom, 
										labelInvestiment.right, stdSize, 10, -1, windowIM )
	--показать данные по изобретению										
	button.LayoutButton( "", layoutActions, -1, "Инфо", _ShowInfoAboutInvention )
	--кнопка добавления людей к исследованию, по которой показывается список со служащими 
	--и возможность добавления выделенного человека
	button.LayoutButton( "", layoutActions, -1, "Добавить", _AddPeopleToInvention )
	--удаление людей из списка
	button.LayoutButton( "", layoutActions, -1, "Убрать", _RemPeopleFromInvention )
	
	--обновление надписей
	guienv:AddLoopTimer( 1000, _UpdateLabels, windowIM )
	--обработчик выбора технологии
	windowIM:Bind( base.GUIELEMENT_LBXITEM_SELECTED, _InventionSelect )
end

function RemoveUser()
	currentInvention:RemoveUser( selectedUser.name )	
end

function CloseWindowInventionList()
	windowInventionList:Remove()
	windowInventionList = nil
	windowIM:Remove()
	windowIM = nil
end
