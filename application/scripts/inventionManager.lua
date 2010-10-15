local base = _G

module( "inventionManager" )

local windowIM = nil
local labelInvestiment = nil
local labelInventionSpeed = nil
local labelInventionPrognoseFinish = nil
local labelInventionMoneyPassed = nil 
local labelInventionDayLeft = nil
local listInventionStuff = nil
local currentInvention = nil

local selectedUser = nil
local windowUserSelect = nil
local windowInventionList = nil
local company = nil 
local picFlowInvention = nil
local applic = base.applic
local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

--обновлеяет текст на метках
local function localUpdateLabels()
    if currentInvention ~= nil and currentInvention:IsValid() then 
		labelInvestiment:SetText( currentInvention:GetInvestiment() )
		labelInventionSpeed:SetText( currentInvention:GetSpeed() )
		local day, month, year = currentInvention:GetPrognoseDateFinish()
		
		labelInventionPrognoseFinish:SetText( day.."."..month.."."..year )
		labelInventionMoneyPassed:SetText( currentInvention:GetPassedMoney() )
		labelInventionDayLeft:SetText( currentInvention:GetDayLeft() )
	else
	    currentInvention = nil
	end
end

--увеличивает затраты на исследования
function IncreaseInvestiment()
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() * 2 )
	currentInvention:CheckParams()
	localUpdateLabels()
end

--уменьшает затраты на исследования
function DecreaseInvestiment()
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() / 2 )
	currentInvention:CheckParams()
	localUpdateLabels()
end

local function localFillListInvnentionStuff()
	listInventionStuff:Clear()
	
	for i=1, currentInvention:GetUserNumber() do
		local user = currentInvention:GetUser( i-1 )
		listInventionStuff:AddItem( "Unknown", user:Self() )
	end
end

function SelectUser()
    local lbx = base.CLuaComponentListBox( base.NrpGetSender() )
	selectedUser = base.CLuaUser( lbx:GetSelectedObject() )
end

function AddPeopleToInvention()
	--создаем окно в центре экрана
	windowUserSelect = guienv:AddWindow( "", scrWidth / 2 - 300, scrHeight / 2 - 200, 
										     scrWidth / 2 + 300, scrHeight / 2 + 200, 
										 -1, guienv:GetRootGUIElement() )
	windowUserSelect:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./inventionManager.SelectUser()" )
													  
	local wd, hd = windowUserSelect:GetSize()												  
	local lbxUsers = guienv:AddComponentListBox( 10, 10, wd - 10, hd - 40, -1, windowUserSelect:Self() )
	lbxUsers:SetItemHeigth( 60 )
		
	company = applic:GetPlayerCompany()
	for i=1, company:GetUserNumber() do
		local user = company:GetUser( i-1 )
		
		if user:GetWorkNumber() == 0 then
			lbxUsers:AddItem( "Unknown", user:Self() )
		end
	end
	
	local btn = guienv:AddButton( 10, hd - 35, wd / 2 - 10, hd - 5, windowUserSelect:Self(), -1, "Select")
	btn:SetAction( "./inventionManager.AddSelectedUserToInvention()" )
		
	btn = guienv:AddButton( wd / 2 + 10, hd - 35, wd - 10, hd - 5, windowUserSelect:Self(), -1, "Close")
	btn:SetAction( "./inventionManager.CloseWindowUserAdding()" )
end

function CloseWindowUserAdding()
	windowUserSelect:Remove()
end

function AddSelectedUserToInvention()
	currentInvention:AddUser( selectedUser:Self() )
	localFillListInvnentionStuff()
end

--отображает окно управления исследованиями
function Show( techName, companyName )
	company = applic:GetCompanyByName( companyName )
	
	base.LogScript( "Окрыто окно для исследования="..techName.. "  компания="..company:GetName() )
	windowIM = guienv:AddWindow( "media/maps/laboratory_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	
	picFlowInvention = guienv:AddPictureFlow( 60, 10, scrWidth - 10, scrHeight / 3, -1, windowIM:Self() )
	picFlowInvention:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
	picFlowInvention:SetDrawBorder( false )
	
	for index=1, company:GetInventionNumber() do
		local invention = company:GetInvention( index-1 )
		picFlowInvention:AddItem( invention:GetTexture(), invention:GetName(), invention:Self() )
			
		if techName == invention:GetInternalName() then
			currentInvention = invention
			picFlowInvention:SetSelected( index-1 )	
		end
	end
	
	local btnWidth = scrWidth / 3
	local ypos = scrHeight / 3

	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local btnPlus = guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+")
	btnPlus:SetAction( "./inventionManager.IncreaseInvestiment()" )
	
	local btnMinus = guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" )
	btnMinus:SetAction( "./inventionManager.DecreaseInvestiment()" )
	
	guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT", 45, ypos - 15, 
													    btnWidth - 45, ypos + 15, -1, windowIM:Self() )
	labelInvestiment = guienv:AddLabel(  currentInvention:GetInvestiment(), 45, ypos + 16, 
													    btnWidth - 45, ypos + 46, -1, windowIM:Self() )													
	
	ypos = ypos + 55
	--метка с отображением скорости исследований
	labelInventionSpeed = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self())
	
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionPrognoseFinish = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self() )
													
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionDayLeft = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONDAYLEFT", 10, ypos, 
										     btnWidth, ypos + 30, -1, windowIM:Self() )
													    
													    
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionMoneyPassed = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONMONEYPASSED", 10, ypos, 
											      btnWidth, ypos + 30, -1, windowIM:Self() )
	
	--список подключенных к проекту людей
	listInventionStuff = guienv:AddComponentListBox( btnWidth + 10, scrHeight / 3 + 20, scrWidth, scrHeight - 45, -1, windowIM:Self())
	localFillListInvnentionStuff()
	
	--кнопка добавления людей к исследованию, по которой показывается список со служащими 
	--и возможность добавления выделенного человека
	--удаление людей из списка
	local btnAddPeople = guienv:AddButton( btnWidth + 10,  scrHeight - 40, 
										   btnWidth + 120, scrHeight - 10, 
										   windowIM:Self(), -1, "Добавить" )
	btnAddPeople:SetAction( "/inventionManager.AddPeopleToInvention()" )
	
	local btnRemPeople = guienv:AddButton( btnWidth + 130, scrHeight - 40, 
										   scrWidth - 10,  scrHeight - 10, 
										   windowIM:Self(), -1, "Убрать" )
	btnRemPeople:SetAction( "./inventionManager.RemPeopleFromInvention()" )	
	
	windowIM:AddLuaFunction( base.GUIELEMENT_AFTER_DRAW, "./inventionManager.UpdateLabelValue()" )
end

local lastTimeUpdate = 0
function UpdateLabelValue()
	if base.GetTickCount() - lastTimeUpdate > 1000 then
		lastTimeUpdate = base.GetTickCount()
		localUpdateLabels()
	end
end

function RemPeopleFromInvention()
	currentInvention:RemoveUser( selectedUser:GetName() )	
end

function OpenWindowForSelectInvention()
	local inv = base.CLuaInvention( picFlowInvention:GetSelectedObject() )
	Show( inv:GetName(), company:GetName() )
end

function CloseWindowInventionList()
	windowInventionList:Remove()
	windowInventionList = nil
	windowIM:Remove()
	windowIM = nil
end

function CreateWindowCompanyInventionManager()
	windowInventionList = guienv:AddWindow( "", 0, 0, scrWidth, scrHeight, 
								            -1, guienv:GetRootGUIElement() ) 
	
	picFlowInvention = guienv:AddPictureFlow( 10, 10, scrWidth / 2, scrHeight - 60, -1, windowInventionList:Self() )
	picFlowInvention:SetPictureRect( 0, 0, 90, 90 )
	
	company = applic:GetPlayerCompany()
	for i=1, company:GetInventionNumber() do
		local inv = company:GetInvention( i-1 )
		picFlowInvention:AddItem( inv:GetTexture(), inv:GetName(), inv:Self() )
	end
	
	local btnOk = guienv:AddButton( 10, scrHeight - 60, 
	                                scrWidth / 2 - 10, scrHeight - 10, 
	                                windowInventionList:Self(), -1, "Выбрать" )
	btnOk:SetAction( "./inventionManager.OpenWindowForSelectInvention()" )
	
	local btnCancel = guienv:AddButton( scrWidth / 2 + 10, scrHheight - 60,
	                                    scrWidth - 10, scrHeight - 10, 
	                                    windowInventionList:Self(), -1, "Выход" )	                                    
	btnCancel:SetAction( "./inventionManager.CloseWindowInventionList()" )
end