local windowIM = CLuaWindow( nil )
local labelInvestiment = CLuaLabel( nil )
local labelInventionSpeed = CLuaLabel( nil )
local labelInventionPrognoseFinish = CLuaLabel( nil )
local listInventionStuff = CLuaListBox( nil )
local currentInvention = CLuaInvention( nil )

local width = 800
local height = 600

--обновлеяет текст на метках
local function localUpdateLabels()
	labelInvestiment:SetText( currentInvention:GetInvestiment() )
	labelInventionSpeed:SetText( currentInvention:GetSpeed() )
	local day, month, year = currentInvention:GetPrognoseDateFinish()
	labelInventionPrognoseFinish:SetText( day.."."..month.."."..year )
end

--увеличивает затраты на исследования
function sworkInventionManagerIncreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() * 2 )
	localUpdateLabels()
end

--уменьшает затраты на исследования
function sworkInventionManagerDecreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() / 2 )
	localUpdateLabels()
end

--отображает окно управления исследованиями
function sworkShowInventionManager( techName, companyName )
	windowIM:SetObject( guienv:AddWindow( "", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	currentInvention:SetObject( applic:GetInvention( techName, companyName ) )

	--картинка с изображением технологии
	local btnWidth = width / 3
	local ypos = height / 3
	local img = CLuaImage( guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" ) )
	img:SetScaleImage( true )
	img:SetImage( driver:GetTexture( currentInvention:GetTexture() ) )

	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local btnPlus = CLuaButton( guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+") )
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = CLuaButton( guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" ) )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
	guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT:", 45, ypos - 15, 
													    btnWidth - 45, ypos + 15, -1, windowIM:Self() )
	labelInvestiment:SetObject(  guienv:AddLabel(  currentInvention:GetInvestiment(), 45, ypos + 16, 
													    btnWidth - 45, ypos + 46, -1, windowIM:Self() ))
	
	ypos = ypos + 55
	--метка с отображением скорости исследований
	labelInventionSpeed:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self()) )
	
	ypos = ypos + 55
	--метка с датой примерного завершения работ при текущем финансировании
	labelInventionPrognoseFinish:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self() ) )
	
	--список подключенных к проекту людей
	listInventionStuff:SetObject( guienv:AddListBox( btnWidth + 10, 50, width, height - 45, -1, windowIM:Self()) )
	
	--кнопка добавления людей к исследованию, по которой показывается список со служащими 
	--и возможность добавления выделенного человека
	--удаление людей из списка
	local btnAddPeople = CLuaButton( guienv:AddButton( btnWidth + 10, 10, width - 10, 10 + 30, windowIM:Self(), -1, "Добавить" ) )
	btnAddPeople:SetAction( "sworkInventionManagerAddPeopleToInvention" )
	
	local btnRemPeople = CLuaButton( guienv:AddButton( btnWidth + 10, height - 40, width - 10, height - 10, windowIM:Self(), -1, "Убрать"  ) )
	btnRemPeople:SetAction( "sworkInventionManagerRemPeopleFromInvention" )
	
end
