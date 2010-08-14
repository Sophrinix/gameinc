local company = nil

local windowLabor = nil
local techMap = CLuaTechMap( nil )
local selectedTech = CLuaTech( nil )
local btnOk = CLuaButton( nil )
local btnCancel = CLuaButton( nil )

local width = 800
local height = 600

local function CreateTechSequence( tech )
	if tech:Empty() == 1 then
		return 
	end
	
	for i=1, tech:GetFutureTechNumber() do
		local ftech = CLuaTech( tech:GetFutureTech( i-1 ) )
		if ftech:Empty() == 0 then
			Log({src=SCRIPT, dev=ODS|CON}, "Дочерняя технология="..ftech:GetName().." Родительская технология="..tech:GetName() )
			techMap:AddTechnology( tech:Self(), ftech:Self() )
			CreateTechSequence( ftech )
		end
	end
end

function sworkCreateGenreTechMapWindow( ptr )
	company = applic:GetPlayerCompany()
	windowLabor:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() ) )
	
	techMap:SetObject( guienv:AddTechMap( 10, 20, width - 10, height - 10, -1, windowLabor:Self() ) )
	techMap:AddLuaFunction( GUIELEMENT_SELECTED_AGAIN, "sworkTechMapWindowTechSelected" )
	
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
	--технология уже в ходу
	Log({src=SCRIPT, dev=ODS|CON}, "Выбрана технология="..selectedTech:GetName().." Описание="..selectedTech:GetDescriptionLink().." Статус="..selectedTech:GetStatus() )

	if selectedTech:GetStatus() == TS_READY then
	    browser:Show()
	    browser:Navigate( selectedTech:GetDescriptionLink() )
	    --local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "Закрыть" ) )
	    --btn:SetAction( "sworkTechMapWindowClose" )
	    return 
	end--или технологию только предстоит изобрести
		--то повторный выбор предполагает желание пользователя начать исследование...
	if selectedTech:GetStatus() == TS_UNKNOWN or selectedTech:GetStatus() == TS_PROJECT then
		sworkTechMapWindowStartInvention( selectedTech )
		return
	end
	
	if selectedTech:GetStatus() == TS_INDEVELOP then
		sworkShowInventionManager( selectedTech:GetName(), company:GetName() )
		return
	end
end

function sworkTechMapWindowStartInvention( tech )
	browser:Show()
	browser:Navigate( tech:GetDescriptionLink() )
	
	btnOk:SetObject( guienv:AddButton( 10, 30, 190, 30 + 20, browser:GetWindow(), -1, "Начать исследования" ) )
	btnOk:SetAction( "sworkTechMapWindowAssignInventionToCompany" )
	btnCancel:SetObject( guienv:AddButton( 200, 30, 390, 30 + 20, browser:GetWindow(), -1, "Закрыть" ) )
	btnCancel:SetAction( "sworkTechMapWindowCloseConfirmationWindow" )
end

function sworkTechMapWindowAssignInventionToCompany( ptr )
	company:StartInvention( selectedTech:Self() )
	sworkShowInventionManager( selectedTech:GetName(), company:GetName() )
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function sworkTechMapWindowCloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end