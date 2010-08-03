local company = CLuaCompany( applic:GetPlayerCompany() )

local windowLabor = CLuaWindow( nil )
local techMap = CLuaTechMap( nil )
local selectedTech = CLuaTech( nil )
local width = 800
local height = 600

local function CreateTechSequence( tech )
	for i=1, tech:GetFutureTechNumber() do
		local ftech = CLuaTech( tech:GetFutureTech( i-1 ) )
		techMap:AddTechnology( tech:Self(), ftech:Self() )
		CreateTechSequence( ftech )
	end
end

function sworkCreateGenreTechMapWindow( ptr )
	windowLabor:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() ) )
	
	techMap:SetObject( guienv:AddTechMap( 10, 20, width - 10, height - 10, -1, windowLabor:Self() ) )
	techMap:AddLuaFunction( TECHMAP_TECHSELECTED, "sworkTechMapWindowTechSelected" )
	
	local tech = CLuaTech( nil )
	for i=1, applic:GetTechNumber() do
	    tech:SetObject( applic:GetTech( i-1 ) )
		
		if tech:GetTechGroup() == PT_GENRE and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech:Self() )
			CreateTechSequence( tech )
		end
	end
end

function sworkTechMapWindowTechSelected( ptr )
	selectedTech:SetObject( ptr )
	--���������� ��� � ����
	browser:Show()
	browser:Navigate( tech:GetDescriptionLink() )

	if tech:IsRealy() then
	    local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "�������" ) )
	    btn:SetAction( "sworkTechMapWindowClose" )
	else--��� ���������� ������ ��������� ���������
	    --������ ��� ������������ ��� �� ������
	    if tech:PercentDone() == 0 then 
			local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "������ ������������" ) )
			btn:SetAction( "sworkTechMapWindowStartInvent" )
		else--������ ������� � ������� ������
			local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "������� ������������" ) )
			btn:SetAction( "sworkTechMapWindowStopInvent" )
		end
	end
end