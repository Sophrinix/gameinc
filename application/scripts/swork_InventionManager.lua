local windowIM = CLuaWindow( nil )
local labelInvestiment = CLuaLabel( nil )
local labelInventionSpeed = CLuaLabel( nil )
local labelInventionPrognoseFinish = CLuaLabel( nil )
local listInventionStuff = CLuaListBox( nil )

local width = 800
local height = 600

function sworkShowInventionManager( techName, companyName )
	windowIM:SetObject( guienv:AddWindow( "", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	local invention = CLuaInvention( applic:GetInvention( techName, companyName ) )

	--картинка с изображением технологии
	local btnWidth = width / 4
	local ypos = height / 4
	local img = CLuaImage( guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" ) )
	img:SetImage( driver:GetTexture( invention:GetTexture() ) )

	--кнопки изменения финансирования и метка с текущим финансированием в месяц
	local btnPlus = CLuaButton( guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+") )
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = CLuaButton( guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" ) )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
	labelInvestiment:SetObject( guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT:", 45, ypos, 
													    btnWidth - 45, ypos + 30, -1, windowIM:Self() ) )
	
	ypos = ypos + 35
	--метка с отображением скорости исследований
	labelInventionSpeed:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self()) )
	
	ypos = ypos + 35
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
