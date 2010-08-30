local windowIM = nil
local labelInvestiment = nil
local labelInventionSpeed = nil
local labelInventionPrognoseFinish = nil
local labelInventionMoneyPassed = nil 
local labelInventionDayLeft = nil
local listInventionStuff = CLuaListBox( nil )
local currentInvention = CLuaInvention( nil )
local selectedUser = nil
local windowUserSelect = nil
local windowInventionList = nil
local company = nil 
local picFlowInvention = nil

local width = scrWidth
local height = scrHeight

--обновлеяет текст на метках
local function localUpdateLabels()
	labelInvestiment:SetText( currentInvention:GetInvestiment() )
	labelInventionSpeed:SetText( currentInvention:GetSpeed() )
	local day, month, year = currentInvention:GetPrognoseDateFinish()
	labelInventionPrognoseFinish:SetText( day.."."..month.."."..year )
	labelInventionMoneyPassed:SetText( currentInvention:GetPassedMoney() )
	labelInventionDayLeft:SetText( currentInvention:GetDayLeft() )
end

--увеличивает затраты на исследования
function sworkInventionManagerIncreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() * 2 )
	currentInvention:CheckParams()
	localUpdateLabels()
end

--уменьшает затраты на исследования
function sworkInventionManagerDecreaseInvestiment( ptr )
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

function sworkInventionManagerSelectUser( ptr )
    local lbx = CLuaComponentListBox( ptr )
	selectedUser = CLuaUser( lbx:GetSelectedObject() )
end

function sworkInventionManagerAddPeopleToInvention( ptr )
	--создаем окно в центре экрана
	windowUserSelect = guienv:AddWindow( "", width / 2 - 300, height / 2 - 200, 
										     width / 2 + 300, height / 2 + 200, -1, guienv:GetRootGUIElement() )
	windowUserSelect:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "sworkInventionManagerSelectUser" )
													  
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
	btn:SetAction( "sworkInventionManagerAddSelectedUserToInvention" )
		
	btn = guienv:AddButton( wd / 2 + 10, hd - 35, wd - 10, hd - 5, windowUserSelect:Self(), -1, "Close")
	btn:SetAction( "sworkInventionManagerCloseWindowUserAdding" )
end

function sworkInventionManagerCloseWindowUserAdding( ptr )
	windowUserSelect:Remove()
end

function sworkInventionManagerAddSelectedUserToInvention( ptr )
	currentInvention:AddUser( selectedUser:Self() )
	localFillListInvnentionStuff()
end

--отображает окно управления исследованиями
function sworkShowInventionManager( techName, companyName )
	Log( {src=SCRIPT, dev=ODS|CON}, "Окрыто окно для исследования="..techName.. "  компания="..companyName )
	windowIM = guienv:AddWindow( "", 0, 0, width, height, -1, guienv:GetRootGUIElement() )
	currentInvention = applic:GetInvention( techName, companyName )

	--картинка с изображением технологии
	local btnWidth = width / 3
	local ypos = height / 3
	local img = guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" )
	img:SetScaleImage( true )
	img:SetImage( currentInvention:GetTexture() )

	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local btnPlus = guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+")
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
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
	listInventionStuff = guienv:AddComponentListBox( btnWidth + 10, 50, width, height - 45, -1, windowIM:Self())
	localFillListInvnentionStuff()
	
	--кнопка добавления людей к исследованию, по которой показывается список со служащими 
	--и возможность добавления выделенного человека
	--удаление людей из списка
	local btnAddPeople = guienv:AddButton( btnWidth + 10, 10, width - 10, 10 + 30, windowIM:Self(), -1, "Добавить" )
	btnAddPeople:SetAction( "sworkInventionManagerAddPeopleToInvention" )
	
	local btnRemPeople = guienv:AddButton( btnWidth + 10, height - 40, width - 10, height - 10, windowIM:Self(), -1, "Убрать" )
	btnRemPeople:SetAction( "sworkInventionManagerRemPeopleFromInvention" )	
end

function sworkInventionManagerRemPeopleFromInvention( ptr )
	currentInvention:RemoveUser( selectedUser:GetName() )	
end

function sworkCloseWindowInventionList( ptr )

end

function sworkCreateWindowCompanyInventionManager()
	windowInventionList = guienv:AddWindow( "", 0, 0, width, height, 
								            -1, guienv:GetRootGUIElement() ) 
	
	picFlowInvention = guienv:AddPictureFlow( 10, 10, width / 2, height - 60, -1, windowInventionList:Self() )
	picFlowInvention:SetPictureRect( 0, 0, 90, 90 )
	
	company = applic:GetPlayerCompany()
	for i=1, company:GetInventionNumber() do
		local inv = company:GetInvention( i-1 )
		picFlowInvention:AddItem( inv:GetTexture(), inv:GetName(), inv:Self() )
	end
	
	local btnOk = guienv:AddButton( 10, 240 - 40, 190, 240, windowInventionList:Self(), -1, "Выбрать" )
	btnOk:SetAction( "sworkOpenWindowForSelectInvention" )
	
	local btnCancel = guienv:AddButton( 210, 240 - 40, 390, 240, windowInventionList:Self(), -1, "Выход" )
	btnCancel:SetAction( "sworkCloseWindowInventionList" )
end