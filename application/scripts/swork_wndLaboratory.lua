local company = nil

local techMap = CLuaTechMap( nil )
local selectedTech = CLuaTech( nil )
local btnOk = CLuaButton( nil )
local btnCancel = CLuaButton( nil )

local width = scrWidth
local height = scrHeight

local function CreateTechSequence( tech )
	if tech:Empty() == 1 then
		return 
	end
	
	for i=1, tech:GetFutureTechNumber() do
		local internalName = tech:GetFutureTechInternalName( i-1 )
		Log({src=SCRIPT, dev=ODS|CON}, "�������� �������w���="..internalName.." ������������ ����������="..tech:GetName() )

		local futureTech = applic:GetTech( internalName );
		--����� ���������� ��� �� �����
		if futureTech:Empty() == 1 then
			--����� � �������� ������� ���������� ���� ����������,
			--���� ��������� ����������� ����������
			local techInvention = applic:GetInvention( internalName, company:GetName() )
			
			--������������ �� �������
			if techInvention:Empty() == 1 then
				--��������� ��� ����������� ����������
				techMap:AddTechnology( tech:Self(), internalName )
			else 
				techMap:AddTechnology( tech:Self(), techInvention:Self() )			
			end
		else
			--����� ���������� ���� �� ����� � ���� ���������
			--������ ��������
			techMap:AddTechnology( tech:Self(), futureTech:Self() )		
			CreateTechSequence( futureTech )
		end
	end
end

local function sworkCreateWindow( typef )
	company = applic:GetPlayerCompany()
	windowLabor = guienv:AddWindow( "media/laboratory_select.png", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() )
	Log( {src=SCRIPT, dev=ODS|CON}, "sworkCreateGenreTechMapWindow="..company:GetName() )
	
	techMap:SetObject( guienv:AddTechMap( 10, 40, width - 10, height - 10, -1, windowLabor:Self() ) )
	techMap:AddLuaFunction( GUIELEMENT_SELECTED_AGAIN, "sworkTechMapWindowTechSelected" )
	techMap:SetDrawBack( false )
	
	local tech = nil
	for i=1, applic:GetTechNumber() do
	    tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == typef and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech:Self() )
			CreateTechSequence( tech )
		end
	end
end

function sworkCreateAdvancedMapWindow( ptr )
	sworkCreateWindow( PT_ADVTECH )	
end

function sworkCreateSoundTechMapWindow( ptr )
	sworkCreateWindow( PT_SOUNDTECH )	
end

function sworkCreateVideoTechMapWindow( ptr )
	sworkCreateWindow( PT_VIDEOTECH )	
end

function sworkCreateGenreTechMapWindow( ptr )
	sworkCreateWindow( PT_GENRE )	
end

function sworkTechMapWindowTechSelected( ptr ) 
    local type = techMap:GetSelectedObjectType()
    
	--���������� ��� � ����
	if type == TS_READY then
	    selectedTech = CLuaTech( techMap:GetSelectedObject() )
		Log({src=SCRIPT, dev=ODS|CON}, "������� ����������="..selectedTech:GetName()..
									   " ��������="..selectedTech:GetDescriptionLink()..
									   " ������="..selectedTech:GetStatus() )
	    
	    browser:Show()
	    browser:Navigate( selectedTech:GetDescriptionLink() )
	    --local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "�������" ) )
	    --btn:SetAction( "sworkTechMapWindowClose" )
	    return 0
	end
	
	--��� ���������� ������ ��������� ���������
	--�� ��������� ����� ������������ ������� ������������ ������ ������������...
	if type == TS_PROJECT then
		sworkTechMapWindowStartInvention()
		return 0
	end
	
	--
	if selectedTech:GetStatus() == TS_INDEVELOP then
		sworkShowInventionManager( techMap:GetSelectedObject(), company:GetName() )
		return 0
	end
end

function sworkTechMapWindowStartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( 10, 30, 190, 30 + 20, browser:GetWindow(), -1, "������ ������������" )
	btnOk:SetAction( "sworkTechMapWindowAssignInventionToCompany" )
	
	btnCancel = guienv:AddButton( 200, 30, 390, 30 + 20, browser:GetWindow(), -1, "�������" )
	btnCancel:SetAction( "sworkTechMapWindowCloseConfirmationWindow" )
end

function sworkTechMapWindowAssignInventionToCompany( ptr )
	local inventionName = techMap:GetSelectedObjectName()
	company:StartInvention( inventionName )
	
	sworkShowInventionManager( inventionName, company:GetName() )
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function sworkTechMapWindowCloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function sworkCloseLaboratoryWindow( ptr )
	if windowLabor ~= nil then
		cityWindow:SetVisible( true )
		windowLabor:Remove()
	end	
	windowLabor = nil
end