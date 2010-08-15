local windowIM = nil
local labelInvestiment = nil
local labelInventionSpeed = nil
local labelInventionPrognoseFinish = nil
local listInventionStuff = CLuaListBox( nil )
local currentInvention = CLuaInvention( nil )
local windowUserSelect = nil
local company = applic:GetPlayerCompany()

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
	currentInvention:CheckParams()
	localUpdateLabels()
end

--��������� ������� �� ������������
function sworkInventionManagerDecreaseInvestiment( ptr )
	currentInvention:SetInvestiment( currentInvention:GetInvestiment() / 2 )
	currentInvention:CheckParams()
	localUpdateLabels()
end

local function localFillListInvnentionStuff()
	for i=1, currentInvention:GetUserNumber() do
		listInventionStuff:AddItem( "Unknown", currentInvention:GetUser( i-1 ) )
	end
end

function sworkInventionManagerAddPeopleToInvention( ptr )
	windowUserSelect:Remove()
	
	--������� ���� � ������ ������
	windowUserSelect = guienv:AddWindow( "", width / 2 - 300, height / 2 - 200, 
										     width / 2 + 300, height / 2 + 200, -1, guienv:GetRootGUIElement() )
													  
	local wd, hd = windowUserSelect:GetSize()												  
	local lbxUsers = CLuaComponentListBox( guienv:AddComponentListBox( 10, 10, wd - 10, hd - 40, -1, windowUserSelect:Self() ) )
	
	for i=1, company:GetUserNumber() do
		local user = CLuaUser( company:GetUser( i-1 ) )
		if not user:HaveWork() then
			lbxUsers:AddItem( "Unknown", user:Self() )
		end
	end
	
	local btn = guienv:AddButton( 10, hd - 35, wd / 2 - 10, hd - 5, windowUserSelect:Self(), -1, "Select")
	btn:SetAction( "sworkInventionManagerAddSelectedUserToInvention" )
		
	btn = guienv:AddButton( wd / 2 + 10, hd - 35, wd - 10, hd - 5, windowUserSelect:Self(), -1, "Closse")
	btn:SetAction( "sworkInventionManagerCloseWindowUserAdding" )
end

--���������� ���� ���������� ��������������
function sworkShowInventionManager( techName, companyName )
	windowIM = guienv:AddWindow( "", 0, 0, width, height, -1, guienv:GetRootGUIElement() )
	currentInvention:SetObject( applic:GetInvention( techName, companyName ) )

	--�������� � ������������ ����������
	local btnWidth = width / 3
	local ypos = height / 3
	local img = guienv:AddImage( 10, 10, btnWidth , ypos, windowIM:Self(), -1, "" )
	img:SetScaleImage( true )
	img:SetImage( currentInvention:GetTexture() )

	--������ ��������� �������������� � ����� � ������� ��������������� � �����
	local btnPlus = guienv:AddButton( 10, ypos, 10 + 30, ypos + 30, windowIM:Self(), -1, "+")
	btnPlus:SetAction( "sworkInventionManagerIncreaseInvestiment" )
	
	local btnMinus = guienv:AddButton( btnWidth - 30, ypos, btnWidth, ypos + 30, windowIM:Self(), -1, "-" )
	btnMinus:SetAction( "sworkInventionManagerDecreaseInvestiment" )
	
	guienv:AddLabel(  "#TRANSLATE_TEXT_INVESTIMENT", 45, ypos - 15, 
													    btnWidth - 45, ypos + 15, -1, windowIM:Self() )
	labelInvestiment = guienv:AddLabel(  currentInvention:GetInvestiment(), 45, ypos + 16, 
													    btnWidth - 45, ypos + 46, -1, windowIM:Self() )													
	
	ypos = ypos + 55
	--����� � ������������ �������� ������������
	labelInventionSpeed = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONSPEED", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self())
	
	ypos = ypos + 55
	--����� � ����� ���������� ���������� ����� ��� ������� ��������������
	labelInventionPrognoseFinish = guienv:AddLabel( "#TRANSLATE_TEXT_INVENTIONPROGNOSEFINISH", 10, ypos, 
													    btnWidth, ypos + 30, -1, windowIM:Self() )
	
	--������ ������������ � ������� �����
	listInventionStuff:SetObject( guienv:AddComponentListBox( btnWidth + 10, 50, width, height - 45, -1, windowIM:Self()) )
	localFillListInvnentionStuff()
	
	--������ ���������� ����� � ������������, �� ������� ������������ ������ �� ��������� 
	--� ����������� ���������� ����������� ��������
	--�������� ����� �� ������
	local btnAddPeople = guienv:AddButton( btnWidth + 10, 10, width - 10, 10 + 30, windowIM:Self(), -1, "��������" )
	btnAddPeople:SetAction( "sworkInventionManagerAddPeopleToInvention" )
	
	local btnRemPeople = guienv:AddButton( btnWidth + 10, height - 40, width - 10, height - 10, windowIM:Self(), -1, "������" )
	btnRemPeople:SetAction( "sworkInventionManagerRemPeopleFromInvention" )	
end
