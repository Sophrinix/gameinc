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

local selectedUser = nil
local windowUserSelect = nil
local company = nil 
local picFlowInvention = nil
local lbxUsers = nil

local applic = base.applic
local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

local function AddLabel( textr, x1, y1, x2, y2 )
	local lb = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONMONEYPASSED", x1, y1, x2, y2, -1, windowIM )
	lb:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )		
	lb.color = base.toColor( 0xff, 0xff, 0xff, 0xff )
	return lb
end

--обновлеяет текст на метках
local function localUpdateLabels()
    if currentInvention ~= nil and currentInvention.isValid then 
		labelInvestiment:SetText( currentInvention.investiment )
		labelInventionSpeed:SetText( currentInvention.speed )
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
function IncreaseInvestiment()
	currentInvention.investiment = currentInvention.investiment * 2
	currentInvention:CheckParams()
end

--уменьшает затраты на исследования
function DecreaseInvestiment()
	currentInvention.investiment = currentInvention.investiment / 2
	currentInvention:CheckParams()
end

local function localFillListInvnentionStuff()
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

function InventionSelect() 		
	currentInvention = base.CLuaInvention( picFlowInvention.selectedObject ) 
end


function UserSelect()	
	selectedUser = base.CLuaUser( lbxUsers.selectedObject )
end

local function localShowUserAvaibleForInvention( listbox )
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

function AddPeopleToInvention()
	--создаем окно в центре экрана
	windowUserSelect = guienv:AddWindow( "", 200, 200, "200e", "200e", -1, guienv.root )
													  
	local wd, hd = windowUserSelect:GetSize()												  
	lbxUsers = guienv:AddComponentListBox( 10, 10, wd - 10, hd - 40, -1, windowUserSelect )
	lbxUsers.itemHeigth = 80
		
	localShowUserAvaibleForInvention( lbxUsers )
	
	local btn = guienv:AddButton( 10, hd - 35, wd / 2 - 10, hd - 5, windowUserSelect, -1, "Select")
	btn.action = AddSelectedUserToInvention
		
	btn = guienv:AddButton( wd / 2 + 10, hd - 35, wd - 10, hd - 5, windowUserSelect, -1, "Close")
	btn.action = CloseWindowUserAdding
	
	--обработчик выбора в списке
	windowUserSelect:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./inventionManager.UserSelect()" )
end

function CloseWindowUserAdding()
	windowUserSelect:Remove()
	windowUserSelect = nil
end

function AddSelectedUserToInvention()
	currentInvention:AddUser( selectedUser )
	
	guienv:MessageBox( base.string.format( "%s будет участвовать в проекте %s исследовательского отдела", 
											selectedUser.name, currentInvention.name ), true, false,
											nil, nil )
											
	localFillListInvnentionStuff()
	localShowUserAvaibleForInvention( lbxUsers )
end

--отображает окно управления исследованиями
function Show( techName, companyName )
	company = applic:GetCompanyByName( companyName )
	
	base.LogScript( "Окрыто окно для исследования="..techName.. "  компания="..company.name )
	windowIM = guienv:AddWindow( "media/maps/laboratory_select.png", 0, 0, "0e", "0e", -1, guienv.root )
	windowIM.draggable = false
	
	picFlowInvention = guienv:AddPictureFlow( 60, 10, "10e", "66%", -1, windowIM )
	picFlowInvention:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
	picFlowInvention.drawBorder = false
	
	for index=1, company.inventionNumber do
		local invention = company:GetInvention( index-1 )
		picFlowInvention:AddItem( invention.texture, invention.name, invention.object )
			
		if techName == invention.uniq then
			currentInvention = invention
			picFlowInvention.selected = index-1
		end
	end
	
	local btnWidth = scrWidth / 3
	local ypos = scrHeight / 3

	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local btnPlus = guienv:AddButton( 10, ypos, "30+", "30+", windowIM, -1, "+")
	btnPlus.action = IncreaseInvestiment
	
	local btnMinus = guienv:AddButton( btnWidth - 30, ypos, "30+", "30+", windowIM, -1, "-" )
	btnMinus.action = DecreaseInvestiment
	
	AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT", 45, ypos - 15, btnWidth - 45, ypos + 15 )
	labelInvestiment = AddLabel( "", 45, ypos + 16, btnWidth - 45, ypos + 46 )													

	ypos = ypos + 55
	--метка с отображением скорости исследований
	labelInventionSpeed = AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED", 10, ypos, btnWidth, ypos + 30 )
	
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionPrognoseFinish = AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH", 10, ypos, btnWidth, ypos + 30 )
													
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionDayLeft = AddLabel( "#TRANSLATE_TEXT_INVENTIONDAYLEFT", 10, ypos, btnWidth, ypos + 30 )													    
												    
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionMoneyPassed = AddLabel( "#TRANSLATE_TEXT_INVENTIONMONEYPASSED", 10, ypos, btnWidth, ypos + 30 )										      
	
	ypos = ypos + 55
	--метка месячным финансированием
	labelInventionMonthMoney = AddLabel( "#TRANSLATE_TEXT_MONTHPAY", 10, ypos, btnWidth, ypos + 30 )

	labelInventionName = AddLabel( "", btnWidth + 10, scrHeight / 3 + 10, "0e", scrHeight / 3 + 40 )

	
	--список подключенных к проекту людей 
	listInventionStuff = guienv:AddComponentListBox( btnWidth + 10, scrHeight / 3 + 50, scrWidth - 10, scrHeight - 45, -1, windowIM )
	listInventionStuff.itemHeigth = 80
	localFillListInvnentionStuff()
	
	--показать данные по изобретению
	local btnShowInfo = guienv:AddButton( btnWidth + 10,  "40e", "120+", "10e",
	 									  windowIM, -1, "Инфо" )
	btnShowInfo.action = ShowInfoAboutInvention

	--кнопка добавления людей к исследованию, по которой показывается список со служащими 
	--и возможность добавления выделенного человека
	local btnAddPeople = guienv:AddButton( btnWidth + 130, "40e", "250+", "10e",  
										   windowIM, -1, "Добавить" )
	btnAddPeople.action = AddPeopleToInvention

	--удаление людей из списка
	local btnRemPeople = guienv:AddButton( btnWidth + 260, "40e", "10e",  "10e", 
										   windowIM, -1, "Убрать" )
	btnRemPeople.action = RemPeopleFromInvention
	
	--обновление надписей
	windowIM:AddLuaFunction( base.GUIELEMENT_AFTER_DRAW, "./inventionManager.UpdateLabelValue()" )
	--обработчик выбора технологии
	windowIM:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./inventionManager.InventionSelect()" )
end

local lastTimeUpdate = 0
function UpdateLabelValue()
	if base.os.clock() - lastTimeUpdate > 0 then
		lastTimeUpdate = base.os.clock()
		localUpdateLabels()
	end
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
