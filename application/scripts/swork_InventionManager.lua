local windowIM = CLuaWindow( nil )
local labelInvestiment = CLuaLabel( nil )
local labelInventionSpeed = CLuaLabel( nil )
local labelInventionPrognoseFinish = CLuaLabel( nil )
local listInventionStuff = CLuaListBox( nil )
local currentInvention = CLuaInvention( nil )

local width = 800
local height = 600

--���������� ����� �� ������
local function localUpdateLabels()
	labelInvestiment:SetText( currentInvention:GetInvestiment() )
	labelInventionSpeed:SetText( currentInvention:GetSpeed() )
	local day, month, year = currentInvention:GetPrognoseDateFinish()
	labelInventionPrognoseFinish:SetText( day.."."..month.."."..year )
end

--����������� ������� �� ������������
function sworkInventionManagerIncreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() * 2 )
	localUpdateLabels()
end

--��������� ������� �� ������������
function sworkInventionManagerDecreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() / 2 )
	localUpdateLabels()
end

--���������� ���� ���������� ��������������
function sworkShowInventionManager( techName, companyName )
	windowIM:SetObject( guienv:AddWindow( "", 0, 0, width, height, -1, guienv:GetRootGUIElement() ) )
	currentInvention:SetObject( applic:GetInvention( techName, companyName ) )

	--�������� � ������������ ����������
	local btnWidth = width / 3
	local ypos = height / 3
	local img = CLuaImage( guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" ) )
	img:SetScaleImage( true )
	img:SetImage( driver:GetTexture( currentInvention:GetTexture() ) )

	--������ ��������� �������������� � ����� � ������� ��������������� � �����
	local btnPlus = CLuaButton( guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+") )
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = CLuaButton( guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" ) )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
	guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT:", 45, ypos - 15, 
													    btnWidth - 45, ypos + 15, -1, windowIM:Self() )
	labelInvestiment:SetObject(  guienv:AddLabel(  currentInvention:GetInvestiment(), 45, ypos + 16, 
													    btnWidth - 45, ypos + 46, -1, windowIM:Self() ))
	
	ypos = ypos + 55
	--����� � ������������ �������� ������������
	labelInventionSpeed:SetObject( guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED:", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self()) )
	
	ypos = ypos + 55
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
