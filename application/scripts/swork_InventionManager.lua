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

	--�������� � ������������ ����������
	local btnWidth = width / 4
	local ypos = height / 4
	local img = CLuaImage( guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" ) )
	img:SetImage( driver:GetTexture( invention:GetTexture() ) )

	--������ ��������� �������������� � ����� � ������� ��������������� � �����
	local btnPlus = CLuaButton( guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+") )
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = CLuaButton( guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" ) )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
	labelInvestiment:SetObject( guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT:", 45, ypos, 
													    btnWidth - 45, ypos + 30, -1, windowIM:Self() ) )
	
	ypos = ypos + 35
	--����� � ������������ �������� ������������
	labelInventionSpeed:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self()) )
	
	ypos = ypos + 35
	--����� � ����� ���������� ���������� ����� ��� ������� ��������������
	labelInventionPrognoseFinish:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self() ) )
	
	--������ ������������ � ������� �����
	listInventionStuff:SetObject( guienv:AddListBox( btnWidth + 10, 50, width, height - 45, -1, windowIM:Self()) )
	
	--������ ���������� ����� � ������������, �� ������� ������������ ������ �� ��������� 
	--� ����������� ���������� ����������� ��������
	--�������� ����� �� ������
	local btnAddPeople = CLuaButton( guienv:AddButton( btnWidth + 10, 10, width - 10, 10 + 30, windowIM:Self(), -1, "��������" ) )
	btnAddPeople:SetAction( "sworkInventionManagerAddPeopleToInvention" )
	
	local btnRemPeople = CLuaButton( guienv:AddButton( btnWidth + 10, height - 40, width - 10, height - 10, windowIM:Self(), -1, "������"  ) )
	btnRemPeople:SetAction( "sworkInventionManagerRemPeopleFromInvention" )
	
end
